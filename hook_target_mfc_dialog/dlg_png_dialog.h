#pragma once


// dlg_png_dialog dialog

class dlg_png_dialog : public CDialogEx
{
	DECLARE_DYNAMIC(dlg_png_dialog)

public:
	dlg_png_dialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~dlg_png_dialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_PNG_LAYER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);

public:
	// from mfc_window1Dlg
	void SetParentDlg(CWnd *dlg){ parentDlg = dlg; }

	// from mfc_window1's dlg_parent
	void setImage();
	void ReSetChildDlg();
	void DrawTXBar();

	void set_target_dlg(CWnd *pcwnd)
	{
		m_pTargetDlg = pcwnd;
	}

	CRect get_crect()
	{
		return m_CenterArea;
	}

private:
	// form mfc_window1Dlg
	CWnd *parentDlg;
	CBrush m_brush;
	CDC *m_pMemDC;
	CBitmap *m_pBitmap;
	int m_nScreenX;
	int m_nScreenY;

	// from mfc_window1's dlg_parent
	BLENDFUNCTION m_blend;
	Gdiplus::Image *m_pImage;
	CRect m_CenterArea;
	CWnd *m_pTargetDlg;
};
