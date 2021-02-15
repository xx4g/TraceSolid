#pragma once
#include "StdAfx.h"
class DialogHookMgr
{
public:

	DialogHookMgr()
	{
		hhookCBTProc = SetWindowsHookEx(WH_CBT,pfnCBTMsgBoxHook,0, GetCurrentThreadId());
	}

	~DialogHookMgr()
	{
		UnhookWindowsHookEx(hhookCBTProc);
	}
	static HHOOK hhookCBTProc;
	static LRESULT CALLBACK pfnCBTMsgBoxHook(int nCode, WPARAM wParam, LPARAM lParam);
private:
};