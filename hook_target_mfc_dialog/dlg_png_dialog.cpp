// dlg_png_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "hook_target_mfc_dialog.h"
#include "dlg_png_dialog.h"
#include "afxdialogex.h"
#include "util.h"

// dlg_png_dialog dialog

IMPLEMENT_DYNAMIC(dlg_png_dialog, CDialogEx)

dlg_png_dialog::dlg_png_dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(dlg_png_dialog::IDD, pParent)
{

}

dlg_png_dialog::~dlg_png_dialog()
{
}

void dlg_png_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL dlg_png_dialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OutputDebugString(L"dlg_png_dialog::OnInitDialog()");

	// from mfc_window1Dlg
	COLORREF transColor = RGB(0, 255, 0);
	m_brush.CreateSolidBrush(transColor);

	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | WS_EX_LAYERED);

	::SetLayeredWindowAttributes(m_hWnd, transColor, 0, 1);

	// from mfc_window1's dlg_parent
	dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);

	VERIFY((dwStyle & WS_POPUP) != 0);
	VERIFY((dwStyle & WS_CHILD) == 0);

	m_blend.BlendOp = 0;
	m_blend.BlendFlags = 0;
	m_blend.AlphaFormat = 1;
	m_blend.SourceConstantAlpha = 255;

	setImage();

	return TRUE;
}

BEGIN_MESSAGE_MAP(dlg_png_dialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
END_MESSAGE_MAP()


// dlg_png_dialog message handlers


void dlg_png_dialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialogEx::OnPaint() for painting messages

	CDC *pDC = GetDC();
	CRect rect;
	GetClientRect(rect);
	CBitmap *oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
	pDC->BitBlt(rect.left, rect.top, m_nScreenX, m_nScreenY, m_pMemDC, rect.left, rect.top, SRCCOPY);
	m_pMemDC->SelectObject(oldBitmap);
}


HBRUSH dlg_png_dialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void dlg_png_dialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);

	CDialogEx::OnLButtonDown(nFlags, point);
}


int dlg_png_dialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	// from mfc_window1Dlg
	CDC *pDC = GetDC();
	m_pMemDC = new CDC();
	m_pBitmap = new CBitmap();
	m_nScreenX = 556;
	m_nScreenY = 397;
	m_pMemDC->CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_nScreenX, m_nScreenY);
	CBitmap *oldBitmap = m_pMemDC->SelectObject(m_pBitmap);
	CBrush brush(RGB(255, 255, 255));
	CRect rect;
	GetClientRect(&rect);
	m_pMemDC->FillRect(CRect(rect.left, rect.top, m_nScreenX, m_nScreenY), &brush);
	m_pMemDC->SelectObject(oldBitmap);
	ReleaseDC(pDC);

	// from mfc_window1's dlg_parent
	/*m_pMainDlg = new dlg_test0(this);
	if (m_pMainDlg)
	{
		m_pMainDlg->Create(IDD_DIALOG_TEST0, this);
		m_pMainDlg->ShowWindow(SW_SHOW);
		m_pMainDlg->SetParentDlg(this);
	}*/

	return 0;
}


void dlg_png_dialog::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here

	// from mfc_window1Dlg
	if (m_pMemDC)
	{
		delete m_pMemDC;
		m_pMemDC = NULL;
	}

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	// from mfc_window1's dlg_parent
	/*if (m_pMainDlg)
	{
		delete m_pMainDlg;
		m_pMainDlg = NULL;
	}*/

	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}


void dlg_png_dialog::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: Add your message handler code here
	ReSetChildDlg();
}

void dlg_png_dialog::setImage()
{
	GdiImageFromResource(&m_pImage, IDB_PNG_BK, L"PNG");
	if (m_pImage)
	{
		MoveWindow(CRect(0, 0, m_pImage->GetWidth(), m_pImage->GetHeight()));
		this->CenterWindow();
		m_CenterArea = CRect(31, 24, m_pImage->GetWidth() - 27, m_pImage->GetHeight() - 23);
		DrawTXBar();
		ReSetChildDlg();
		/*m_pMainDlg->SetDrawRect(m_CenterArea);*/
	}
}

void dlg_png_dialog::ReSetChildDlg()
{
	CRect rcWin;
	GetWindowRect(&rcWin);
	rcWin.left += m_CenterArea.left;
	rcWin.top += m_CenterArea.top;
	rcWin.right = rcWin.left + m_CenterArea.Width();
	rcWin.bottom = rcWin.top + m_CenterArea.Height();
	if (m_pTargetDlg)
	{
		m_pTargetDlg->MoveWindow(rcWin);
	}
}

void dlg_png_dialog::DrawTXBar()
{
	OutputDebugString(L"dlg_png_dialog::DrawTXBar()");

	RECT rct;
	GetWindowRect(&rct);

	HDC hdcTemp = GetDC()->m_hDC;
	HDC m_hdcMemory = CreateCompatibleDC(hdcTemp);
	HBITMAP hBitMap = CreateCompatibleBitmap(hdcTemp, rct.right, rct.bottom);
	SelectObject(m_hdcMemory, hBitMap);

	POINT ptWinPos = { rct.left, rct.top };

	Gdiplus::Graphics graphics(m_hdcMemory);
	RECT rcClient;
	GetClientRect(&rcClient);

	graphics.DrawImage(m_pImage, Gdiplus::Rect(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom));

	SIZE sizeWindow = { rct.right, rct.bottom };
	POINT ptSrc = { 0, 0 };

	DWORD dwExStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if ((dwExStyle & 0x80000) != 0x80000)
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle ^ 0x80000);

	HDC hdcScreen = ::GetDC(m_hWnd);
	::UpdateLayeredWindow(m_hWnd, hdcScreen, &ptWinPos, &sizeWindow, m_hdcMemory, &ptSrc, 0, &m_blend, 2);
	graphics.ReleaseHDC(m_hdcMemory);
	::ReleaseDC(m_hWnd, hdcScreen);
	hdcScreen = NULL;
	::ReleaseDC(m_hWnd, hdcTemp);
	hdcTemp = NULL;
	DeleteObject(hBitMap);
	DeleteDC(m_hdcMemory);
	m_hdcMemory = NULL;
}
