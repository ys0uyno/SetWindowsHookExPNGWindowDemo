// dlg_png_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "hook_target_mfc_dialog.h"
#include "dlg_png_dialog.h"
#include "afxdialogex.h"


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


BEGIN_MESSAGE_MAP(dlg_png_dialog, CDialogEx)
END_MESSAGE_MAP()


// dlg_png_dialog message handlers
