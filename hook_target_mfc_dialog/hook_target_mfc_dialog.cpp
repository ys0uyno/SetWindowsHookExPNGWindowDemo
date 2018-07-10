// hook_target_mfc_dialog.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "hook_target_mfc_dialog.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HHOOK g_hhook_wnd_ret = NULL;
HHOOK g_hhook_msg = NULL;

HWND g_hwnd = NULL;

WNDPROC g_old_proc;
static bool g_subclassed = false;

class resource_handle
{
public:
	resource_handle()
	{
		m_hinstance = NULL;
	}

	~resource_handle()
	{
		if (m_hinstance)
			FreeLibrary(m_hinstance);
	}

public:
	bool load(const CString &module)
	{
		m_hinstance = LoadLibrary(module);
		return (m_hinstance == NULL ? false : true);
	}

	HINSTANCE get_hinstance()
	{
		return m_hinstance;
	}

private:
	HINSTANCE m_hinstance;
};

resource_handle g_rh;

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// Chook_target_mfc_dialogApp

BEGIN_MESSAGE_MAP(Chook_target_mfc_dialogApp, CWinApp)
END_MESSAGE_MAP()


// Chook_target_mfc_dialogApp construction

Chook_target_mfc_dialogApp::Chook_target_mfc_dialogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Chook_target_mfc_dialogApp object

Chook_target_mfc_dialogApp theApp;


// Chook_target_mfc_dialogApp initialization

BOOL Chook_target_mfc_dialogApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

LRESULT CALLBACK new_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_CTLCOLORBTN)
	{
		HBRUSH hbrush;
		hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		SetBkMode((HDC)wParam, TRANSPARENT);
		return ((LRESULT)hbrush);
	}

	if (Msg == WM_DESTROY)
	{
		g_subclassed = false;
	}

	if (Msg == WM_MOVE)
	{
	}

	return CallWindowProc(g_old_proc, hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK GetMsgProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSG *p = (MSG *)lParam;

	switch (p->message)
	{
	case WM_PAINT:
		{
		}
		break;
	case WM_LBUTTONDOWN:
		{
		}
		break;
	}

	return CallNextHookEx(g_hhook_msg, nCode, wParam, lParam);
}

LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	CWPRETSTRUCT *p = (CWPRETSTRUCT *)lParam;

	switch (p->message)
	{
	case WM_INITDIALOG:
		{
			if (g_subclassed)
			{
				break;
			}
			g_subclassed = true;

			OutputDebugString(L"WM_INITDIALOG");

			Gdiplus::GdiplusStartupInput input;
			ULONG_PTR token;
			GdiplusStartup(&token, &input, NULL);

			HWND hwnd = FindWindow(NULL, L"target_mfc_dialog");
			if (NULL == hwnd)
			{
				OutputDebugString(L"FindWindow target_mfc_dialog error");
				break;
			}

			g_hwnd = hwnd;

			DWORD dwstyle = GetWindowLong(hwnd, GWL_STYLE);
			DWORD dwstyle_new = WS_OVERLAPPED
				| WS_VISIBLE
				| WS_SYSMENU
				| WS_MINIMIZEBOX
				| WS_MAXIMIZEBOX
				| WS_CLIPCHILDREN
				| WS_CLIPSIBLINGS;
			dwstyle_new &= dwstyle; // & will remove style
			SetWindowLong(hwnd, GWL_STYLE, dwstyle_new);

			DWORD dwexstyle = GetWindowLong(hwnd, GWL_EXSTYLE);
			DWORD dwexstyle_new = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
			dwexstyle_new &= dwexstyle;
			SetWindowLong(hwnd, GWL_EXSTYLE, dwexstyle_new);

			SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

			// g_old_proc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)new_proc);
		}
		break;
	}

	return CallNextHookEx(g_hhook_wnd_ret, nCode, wParam, lParam);
}

extern "C" __declspec(dllexport) void BegDialogHook(HWND hwnd)
{
	DWORD tid = GetWindowThreadProcessId(hwnd, NULL);
	if (0 == tid)
	{
		OutputDebugString(L"GetWindowThreadProcessId return 0");
		return;
	}

	g_hhook_wnd_ret =
		SetWindowsHookEx(WH_CALLWNDPROCRET, CallWndRetProc, GetModuleHandle(L"hook_target_mfc_dialog"), tid);
	g_hhook_msg =
		SetWindowsHookEx(WH_GETMESSAGE, GetMsgProc, GetModuleHandle(L"hook_target_mfc_dialog"), tid);
}

extern "C" __declspec(dllexport) void EndDialogHook()
{
	if (NULL != g_hhook_wnd_ret)
		UnhookWindowsHookEx(g_hhook_wnd_ret);

	if (NULL != g_hhook_msg)
		UnhookWindowsHookEx(g_hhook_msg);
}
