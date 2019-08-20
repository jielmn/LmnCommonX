#include <math.h>
#include "LmnGdi.h"

static void LineTo_( HDC hDC, HPEN hPen, HBRUSH hBrush, POINT start, POINT end, int nWidth) {
	HPEN  hOldPen = 0;
	HBRUSH hOldBrush = 0;

	if (1 == nWidth) {
		hOldPen = (HPEN)::SelectObject(hDC, hPen);
		::MoveToEx(hDC, start.x, start.y, 0);
		::LineTo(hDC, end.x, end.y);
		SelectObject(hDC, hOldPen);
	}
	// 宽度大于1
	else {
		hOldBrush = (HBRUSH)::SelectObject(hDC, hBrush);
		hOldPen   = (HPEN)::SelectObject(hDC, GetStockObject(NULL_PEN) );
		// 两条线在一条垂直线上
		if (start.x == end.x) {
			if (nWidth % 2 == 0) {
				int r = nWidth / 2;
				Rectangle(hDC, start.x - r, start.y, end.x + r + 1, end.y);
			}
			else {
				int r = nWidth / 2;
				Rectangle(hDC, start.x - r, start.y, end.x + r + 2, end.y);
			}
		}
		// 两条线在一条水平线上
		else if (start.y == end.y) {
			if (nWidth % 2 == 0) {
				int r = nWidth / 2;
				Rectangle(hDC, start.x, start.y - r, end.x, end.y + r + 1);
			}
			else {
				int r = nWidth / 2;
				Rectangle(hDC, start.x, start.y - r, end.x, end.y + r + 2);
			}
		}
		else {
			// 计算起点(终点)线段的斜率
			double k  = -(double)(end.x - start.x) / (double)(end.y - start.y) ;
			if (nWidth % 2 == 0) {
				int r = nWidth / 2;
				double deltaX = sqrt(pow(r, 2.0) / (1 + pow(k, 2.0)));
				double deltaY = k * deltaX;

				POINT  pts[4];
				pts[0].x = (int)(start.x - deltaX);
				pts[0].y = (int)(start.y - deltaY);
				pts[1].x = (int)(start.x + deltaX);
				pts[1].y = (int)(start.y + deltaY);
				pts[2].x = (int)(end.x + deltaX);
				pts[2].y = (int)(end.y + deltaY);
				pts[3].x = (int)(end.x - deltaX);
				pts[3].y = (int)(end.y - deltaY);
				::Polygon(hDC, pts, 4);
			}
			else {
				int r = nWidth / 2;

				double deltaX  = sqrt(pow(r, 2.0) / (1 + pow(k, 2.0)));
				double deltaY  = k * deltaX;

				double deltaX1 = sqrt(pow(r+1, 2.0) / (1 + pow(k, 2.0)));
				double deltaY1 = k * deltaX1;

				POINT  pts[5];
				pts[0].x = (int)(start.x - deltaX);
				pts[0].y = (int)(start.y - deltaY);
				pts[1].x = (int)(start.x + deltaX1);
				pts[1].y = (int)(start.y + deltaY1);
				pts[2].x = (int)(end.x + deltaX1);
				pts[2].y = (int)(end.y + deltaY1);
				pts[3].x = (int)(end.x - deltaX);
				pts[3].y = (int)(end.y - deltaY);
				::Polygon(hDC, pts, 4);
			}
		}
		::SelectObject(hDC, hOldBrush);
		::SelectObject(hDC, hOldPen);
	}
}

// 画出一条虚线
// 参数：
//      nWidth:  虚线的粗细
//      nSegmentLength: 虚线中，间隔的实线的最大长度
//      nGapLength: 虚线中，间隔的长度
BOOL  DashLineTo( HDC  hDC, HPEN  hPen, POINT start, POINT end,
	              int nSegmentLength, int nGapLength,
	              int nWidth /*= 1*/, HBRUSH  hBrush /*= 0*/ ) {

	int  nLineLength = (int)round(sqrt(pow(end.x - start.x, 2.0) + pow(end.y - start.y, 2.0)));

	// 如果长度不够一个间隙的，直接实线连接
	if (nLineLength <= nGapLength) {
		LineTo_(hDC, hPen, hBrush, start, end, nWidth);
	}
	else {
		// 虚线由 n*nGapLength+(n-1)*nSegmentLength + 两端边缘的长度组成
		// 两端的长度至少是nSegmentLength/2
		int n = nLineLength/ (nSegmentLength + nGapLength);
		int nSideLength = ( nLineLength - n * nGapLength - (n - 1) * nSegmentLength ) / 2;
		POINT  p1, p2;
		// 如果起点和终点在一条垂直线上，计算不出斜率
		if (start.x == end.x) {
			int nDirectory = (end.y > start.y ? 1 : -1);
			p1 = start;
			p2 = start;
			p2.y += nSideLength * nDirectory;
			// 先画第一条边
			LineTo_(hDC, hPen, hBrush, p1, p2, nWidth );
			p1 = p2;

			// 再画中间的间隙，线段，间隙，...
			for (int i = 0; i < n; i++) {
				if (i == n - 1) {
					p1.y += nGapLength * nDirectory;
				}
				else {
					p1.y += nGapLength * nDirectory;
					p2 = p1;
					p2.y += nSegmentLength * nDirectory;
					// 画线段
					LineTo_(hDC, hPen, hBrush, p1, p2, nWidth);
					p1 = p2;
				}
			}

			// 画结尾的一边
			LineTo_(hDC, hPen, hBrush, p1, end, nWidth);
		}
		// 如果起点和终点不在一条垂直线上，可以计算出斜率
		else {
			int nDirectoryX = (end.x > start.x ? 1 : -1);

			// 计算斜率
			double k = (double)(end.y - start.y) / (double)(end.x - start.x);
			p1 = start;
			p2 = start;

			// 计算水平和垂直方向差值 
			double deltaX = sqrt( pow(nSideLength, 2.0) / ( 1 + pow(k, 2.0) ) ) * nDirectoryX;
			double deltaY = deltaX * k;

			p2.x += (int)round(deltaX);
			p2.y += (int)round(deltaY);

			// 先画第一条边
			LineTo_(hDC, hPen, hBrush, p1, p2, nWidth);
			p1 = p2;

			double deltaX1 = sqrt(pow(nSegmentLength, 2.0) / (1 + pow(k, 2.0))) * nDirectoryX;
			double deltaY1 = deltaX1 * k;

			double deltaX2 = sqrt(pow(nGapLength, 2.0) / (1 + pow(k, 2.0))) * nDirectoryX;
			double deltaY2 = deltaX2 * k;

			// 再画中间的间隙，线段，间隙，...
			for (int i = 0; i < n; i++) {
				if (i == n - 1) {
					p1.x += (int)round(deltaX2);
					p1.y += (int)round(deltaY2);
				}
				else {
					p1.x += (int)round(deltaX2);
					p1.y += (int)round(deltaY2);
					p2 = p1;
					p2.x += (int)round(deltaX1);
					p2.y += (int)round(deltaY1);
					// 画线段
					LineTo_(hDC, hPen, hBrush, p1, p2, nWidth);
					p1 = p2;
				}
			}

			// 画结尾的一边
			LineTo_(hDC, hPen, hBrush, p1, end, nWidth);
		}
	}

	return TRUE;
}