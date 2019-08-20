
// MfcTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MfcTest.h"
#include "MfcTestDlg.h"
#include "afxdialogex.h"
#include "LmnGdi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMfcTestDlg �Ի���



CMfcTestDlg::CMfcTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfcTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMfcTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMfcTestDlg ��Ϣ�������

BOOL CMfcTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMfcTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		//::TextOut(dc, 0, 0, "Hello", 5);
		HPEN hRedPen = ::CreatePen(PS_DASH, 1, RGB(0xFF, 0, 0));
		HBRUSH hBlueBrush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));

		POINT  s, e;
		s.x = e.x = 100;
		s.y = 100;
		e.y = 200;
		DashLineTo(dc, hRedPen, s, e, 8, 3);

		POINT  s1, e1;
		s1.x = 100;
		s1.y = 100;
		e1.x = 200;
		e1.y = 200;
		DashLineTo(dc, hRedPen, s1, e1, 8, 3);

		POINT  s2, e2;
		s2.x = 200;
		s2.y = 200;
		e2.x = 200;
		e2.y = 100;
		DashLineTo(dc, hRedPen, s2, e2, 8, 3, 2, hGreenBrush);

		POINT  s3, e3;
		s3.x = 200;
		s3.y = 100;
		e3.x = 300;
		e3.y = 100;
		DashLineTo(dc, hRedPen, s3, e3, 8, 3, 3, hBlueBrush);

		POINT  s4, e4;
		s4.x = 310;
		s4.y = 100;
		e4.x = 210;
		e4.y = 200;
		DashLineTo(dc, hRedPen, s4, e4, 8, 3, 4, hBlueBrush);

		::MoveToEx(dc, 0, 100, 0);
		::LineTo(dc, 600, 100);

		::MoveToEx(dc, 0, 200, 0);
		::LineTo(dc, 600, 200);

		//::MoveToEx(dc, 200, 100, 0);
		//::LineTo(dc, 200, 200);

		::SelectObject(dc, hBlueBrush);
		::Rectangle(dc, 0, 0, 200, 30);

		//::MoveToEx(dc, 0, 5, 0);
		//::LineTo(dc, 200, 5);
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMfcTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

