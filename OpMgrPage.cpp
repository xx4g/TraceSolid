// OpMgrPage
// (c) Copyright 2017, CNC Software, Inc.
// All rights reserved.
//
// The initialization routines for the DLL.

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HINSTANCE resHandle = nullptr;
HINSTANCE GetChookResourceHandle()
{
	return resHandle;
}

static AFX_EXTENSION_MODULE OpMgrPage = { FALSE, nullptr };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE("TraceSolid.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(OpMgrPage, hInstance))
			return 0;

		// We MUST save the DLL instance handle for later use with ChangeResCl.
		// If you use a separate resource DLL you must load it here using AfxLoadLibrary 
		// and save that handle here and in the extension module structure.
		//resHandle = AfxLoadLibrary(_T("Resources\\OpMgrPageRes.dll"));

		// If our Resources are in the [CHookName].DLL
		// The 'hInstance' pass in is the handle
		resHandle = hInstance;

		if (resHandle == nullptr)
			return 0;

		OpMgrPage.hResource = resHandle;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE("TraceSolid.DLL Terminating!\n");

		// This is required because CHooks are explicitly loaded by Mastercam.
		if (resHandle)
			AfxFreeLibrary(resHandle);

		// Terminate the library before destructors are called
		AfxTermExtensionModule(OpMgrPage);
	}
	return 1; // ok
}

#ifdef _MANAGED
#pragma managed(pop)
#endif
