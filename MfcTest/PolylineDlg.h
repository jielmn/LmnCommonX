#pragma once


// PolylineDlg �Ի���
#define POINTS_COUNT 20

class PolylineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PolylineDlg)

public:
	PolylineDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PolylineDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	POINT        m_points[POINTS_COUNT];
	float        m_fRatio;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void OnPaint();
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()
};
