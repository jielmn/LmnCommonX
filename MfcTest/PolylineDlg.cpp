// PolylineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MfcTest.h"
#include "PolylineDlg.h"
#include "afxdialogex.h"
#include "LmnCommon.h"
#include "LmnGdi.h"

// PolylineDlg 对话框

IMPLEMENT_DYNAMIC(PolylineDlg, CDialogEx)

PolylineDlg::PolylineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	int index = 0;
	m_fRatio = 1.0f;

	m_points[index].x = 2;
	m_points[index].y = 131;
	index++;

	m_points[index].x = 2;
	m_points[index].y = 127;
	index++;

	m_points[index].x = 8;
	m_points[index].y = 124;
	index++;

	m_points[index].x = 8;
	m_points[index].y = 129;
	index++;

	m_points[index].x = 8;
	m_points[index].y = 127;
	index++;

	m_points[index].x = 14;
	m_points[index].y = 136;
	index++;

	m_points[index].x = 20;
	m_points[index].y = 130;
	index++;

	m_points[index].x = 26;
	m_points[index].y = 125;
	index++;

	m_points[index].x = 32;
	m_points[index].y = 129;
	index++;

	m_points[index].x = 38;
	m_points[index].y = 130;
	index++;    // 10

	m_points[index].x = 42;
	m_points[index].y = 135;
	index++;

	m_points[index].x = 40;
	m_points[index].y = 140;
	index++;

	m_points[index].x = 38;
	m_points[index].y = 142;
	index++;

	m_points[index].x = 38;
	m_points[index].y = 144;
	index++;

	m_points[index].x = 32;
	m_points[index].y = 148;
	index++;

	m_points[index].x = 26;
	m_points[index].y = 150;
	index++;

	m_points[index].x = 26;
	m_points[index].y = 153;
	index++;

	m_points[index].x = 20;
	m_points[index].y = 148;
	index++;

	m_points[index].x = 14;
	m_points[index].y = 152;
	index++;

	m_points[index].x = 8;
	m_points[index].y = 150;
	index++;  // 20
}

PolylineDlg::~PolylineDlg()
{
}

void PolylineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PolylineDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void OnDrawPolygon(void * pArg, const POINT * pts, int cpt) {
	void ** args = (void **)pArg;
	HDC  hDC  = (HDC)args[0];
	HPEN hPen = (HPEN)args[1];

	::Polyline(hDC, pts, cpt);

	for (int i = 0; i < cpt; i++) {
		const POINT * pt = &pts[i];
		Ellipse(hDC, pt->x - 3, pt->y - 3, pt->x + 3, pt->y + 3);
	}
}

void PolylineDlg::OnPaint() {	
	CPaintDC dc(this);

	POINT   points[POINTS_COUNT];
	for (int i = 0; i < POINTS_COUNT; i++) {
		points[i].x = (int)( m_points[i].x * m_fRatio );
		points[i].y = m_points[i].y;
	}

	HPEN hBluePen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	void * args[2] = { 0 };
	args[0] = dc.m_hDC;
	args[1] = hBluePen;

	LmnPolyline( points, POINTS_COUNT, 0, (void *)args, OnDrawPolygon);

	DeleteObject(hBluePen);
	CDialogEx::OnPaint();
}

BOOL PolylineDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	if ( zDelta > 0 ) {
		m_fRatio += 1.0f;
	}
	else {
		m_fRatio -= 1.0f;
	}

	if (m_fRatio < 1.0f) {
		m_fRatio = 1.0f;
	}
	else if (m_fRatio > 10.0f) {
		m_fRatio = 10.0f;
	}

	this->Invalidate(TRUE);
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

// PolylineDlg 消息处理程序
