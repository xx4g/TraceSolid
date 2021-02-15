#include "StdAfx.h"
#include "DialogHookMgr.h"


LRESULT DialogHookMgr::pfnCBTMsgBoxHook(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode == HCBT_CREATEWND)
	{
		CREATESTRUCT *pcs = ((CBT_CREATEWND *)lParam)->lpcs;
		if ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP))
		{
			//Get the window handle
			HWND hwnd = (HWND)wParam;
			if (hwnd)
			{
				if (hwnd)
				{

					::SendMessage(hwnd, WM_CLOSE, NULL, NULL);
				}
			}
		}
	}


	return (CallNextHookEx(hhookCBTProc, nCode, wParam, lParam));
}
HHOOK DialogHookMgr::hhookCBTProc = nullptr;