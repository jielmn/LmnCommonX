#include <math.h>
#include "LmnGdi.h"

static void LineTo_( POINT start, POINT end, int nWidth, void * pArg, 
	                 DrawLineCb pfnLine, DrawPolygonCb pfnPolygon) {
	HPEN  hOldPen = 0;
	HBRUSH hOldBrush = 0;

	if (1 == nWidth) {
		if ( pfnLine ) {
			pfnLine(pArg, start, end);
		}
	}
	// ��ȴ���1
	else {
		if (0 == pfnPolygon)
			return;

		POINT  pts[4];
		// ��������һ����ֱ����
		if (start.x == end.x) {
			if (nWidth % 2 == 0) {
				int r = nWidth / 2;

				pts[0].x = start.x - r;
				pts[0].y = start.y;				
				pts[2].x = end.x + r + 1;
				pts[2].y = end.y;
				pts[1].x = pts[0].x;
				pts[1].y = pts[2].y;
				pts[3].x = pts[2].x;
				pts[3].y = pts[0].y;

				pfnPolygon(pArg, pts, 4);
			}
			else {
				int r = nWidth / 2;

				pts[0].x = start.x - r;
				pts[0].y = start.y;
				pts[2].x = end.x + r + 2;
				pts[2].y = end.y;
				pts[1].x = pts[0].x;
				pts[1].y = pts[2].y;
				pts[3].x = pts[2].x;
				pts[3].y = pts[0].y;

				pfnPolygon(pArg, pts, 4);
			}
		}
		// ��������һ��ˮƽ����
		else if (start.y == end.y) {
			if (nWidth % 2 == 0) {
				int r = nWidth / 2;

				pts[0].x = start.x;
				pts[0].y = start.y - r;
				pts[2].x = end.x;
				pts[2].y = end.y + r + 1;
				pts[1].x = pts[0].x;
				pts[1].y = pts[2].y;
				pts[3].x = pts[2].x;
				pts[3].y = pts[0].y;

				pfnPolygon(pArg, pts, 4);

			}
			else {
				int r = nWidth / 2;

				pts[0].x = start.x;
				pts[0].y = start.y - r;
				pts[2].x = end.x;
				pts[2].y = end.y + r + 2;
				pts[1].x = pts[0].x;
				pts[1].y = pts[2].y;
				pts[3].x = pts[2].x;
				pts[3].y = pts[0].y;

				pfnPolygon(pArg, pts, 4);
			}
		}
		else {
			// �������(�յ�)�߶ε�б��
			double k = -(double)(end.x - start.x) / (double)(end.y - start.y);
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

				pfnPolygon(pArg, pts, 4);
			}
			else {
				int r = nWidth / 2;

				double deltaX = sqrt(pow(r, 2.0) / (1 + pow(k, 2.0)));
				double deltaY = k * deltaX;

				double deltaX1 = sqrt(pow(r + 1, 2.0) / (1 + pow(k, 2.0)));
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

				pfnPolygon(pArg, pts, 4);
			}
		}
	}
}

// ����һ������
// ������
//      nWidth:  ���ߵĴ�ϸ
//      nSegmentLength: �����У������ʵ�ߵ���󳤶�
//      nGapLength: �����У�����ĳ���
BOOL   DashLineTo( POINT start, POINT end, int nSegmentLength, int nGapLength,
	               int nWidth, void * pArg, DrawLineCb pfnLine,
	               DrawPolygonCb pfnPolygon /*= 0*/ ) {

	int  nLineLength = (int)round(sqrt(pow(end.x - start.x, 2.0) + pow(end.y - start.y, 2.0)));

	// ������Ȳ���һ����϶�ģ�ֱ��ʵ������
	if (nLineLength <= nGapLength) {
		LineTo_(start, end, nWidth, pArg, pfnLine, pfnPolygon);
	}
	else {
		// ������ n*nGapLength+(n-1)*nSegmentLength + ���˱�Ե�ĳ������
		// ���˵ĳ���������nSegmentLength/2
		int n = nLineLength / (nSegmentLength + nGapLength);
		int nSideLength = (nLineLength - n * nGapLength - (n - 1) * nSegmentLength) / 2;
		POINT  p1, p2;
		// ��������յ���һ����ֱ���ϣ����㲻��б��
		if (start.x == end.x) {
			int nDirectory = (end.y > start.y ? 1 : -1);
			p1 = start;
			p2 = start;
			p2.y += nSideLength * nDirectory;
			// �Ȼ���һ����
			LineTo_(p1, p2, nWidth, pArg, pfnLine, pfnPolygon);
			p1 = p2;

			// �ٻ��м�ļ�϶���߶Σ���϶��...
			for (int i = 0; i < n; i++) {
				if (i == n - 1) {
					p1.y += nGapLength * nDirectory;
				}
				else {
					p1.y += nGapLength * nDirectory;
					p2 = p1;
					p2.y += nSegmentLength * nDirectory;
					// ���߶�
					LineTo_(p1, p2, nWidth, pArg, pfnLine, pfnPolygon);
					p1 = p2;
				}
			}

			// ����β��һ��
			LineTo_(p1, end, nWidth, pArg, pfnLine, pfnPolygon);
		}
		// ��������յ㲻��һ����ֱ���ϣ����Լ����б��
		else {
			int nDirectoryX = (end.x > start.x ? 1 : -1);

			// ����б��
			double k = (double)(end.y - start.y) / (double)(end.x - start.x);
			p1 = start;
			p2 = start;

			// ����ˮƽ�ʹ�ֱ�����ֵ 
			double deltaX = sqrt(pow(nSideLength, 2.0) / (1 + pow(k, 2.0))) * nDirectoryX;
			double deltaY = deltaX * k;

			p2.x += (int)round(deltaX);
			p2.y += (int)round(deltaY);

			// �Ȼ���һ����
			LineTo_(p1, p2, nWidth, pArg, pfnLine, pfnPolygon);
			p1 = p2;

			double deltaX1 = sqrt(pow(nSegmentLength, 2.0) / (1 + pow(k, 2.0))) * nDirectoryX;
			double deltaY1 = deltaX1 * k;

			double deltaX2 = sqrt(pow(nGapLength, 2.0) / (1 + pow(k, 2.0))) * nDirectoryX;
			double deltaY2 = deltaX2 * k;

			// �ٻ��м�ļ�϶���߶Σ���϶��...
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
					// ���߶�
					LineTo_(p1, p2, nWidth, pArg, pfnLine, pfnPolygon);
					p1 = p2;
				}
			}

			// ����β��һ��
			LineTo_(p1, end, nWidth, pArg, pfnLine, pfnPolygon);
		}
	}

	return TRUE;
}