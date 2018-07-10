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
};
