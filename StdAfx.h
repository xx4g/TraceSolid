// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once
#pragma comment(lib, "bnci.lib")
#pragma comment(lib, "Math.lib")
#pragma comment(lib, "mastercam.lib")
#pragma comment(lib, "mccore.lib")
#pragma comment(lib, "UICtrls.lib")
#pragma comment(lib, "MCCAD.lib")
#pragma comment(lib, "MCCADUtil.lib")
#pragma comment(lib, "MCCE.lib")
#pragma comment(lib, "MCControl.lib")
#pragma comment(lib, "MCCoreBase.lib")
#pragma comment(lib, "MCCore.lib")
#pragma comment(lib, "MCCurveSurfGeom.lib")
#pragma comment(lib, "MCGeomSld.lib")
#pragma comment(lib, "MCGuiBase.lib")
#pragma comment(lib, "MCIk.lib")
#pragma comment(lib, "McIPC.lib")
#pragma comment(lib, "MCKernel.lib")
#pragma comment(lib, "MCLathe.lib")
#pragma comment(lib, "MCMachineDef.lib")
#pragma comment(lib, "MCMill.lib")
#pragma comment(lib, "MCMpParameters.lib")
#pragma comment(lib, "MCMSurf.lib")
#pragma comment(lib, "MCMultiax.lib")
#pragma comment(lib, "MCOReader.lib")
#pragma comment(lib, "MCPost.lib")
#pragma comment(lib, "MCProbe.lib")
#pragma comment(lib, "MCPrvVerFile.lib")
#pragma comment(lib, "MCRouter.lib")
#pragma comment(lib, "MCRtClasses.lib")
#pragma comment(lib, "MCTool.lib")
#pragma comment(lib, "MCUI5Ax.lib")
#pragma comment(lib, "MCVerify.lib")
#pragma comment(lib, "MCWire.lib")
#pragma comment(lib, "RibbonUI.lib")
#pragma comment(lib, "SysTypes.lib")
#pragma comment(lib, "TlCore.lib")
#pragma comment(lib, "ToolNetApi.lib")
#pragma comment(lib, "TPFiveAx.lib")
#pragma comment(lib, "TPGeom.lib")
#pragma comment(lib, "TPIFTools.lib")
#pragma comment(lib, "MCCAD.lib")
#pragma comment(lib, "UICtrls.lib")
#pragma comment(lib, "VistaFileDialogs.lib")

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"
#include "3DVarsDefines_CH.h" // for C_H_VERSION

#if C_H_VERSION >= 2100 // Mastercam 2019 or later
#ifndef _UNICODE
#define _UNICODE
#endif
#pragma message("Building for Mastercam with _UNICODE Character Set")
#else // Mastercam prior to 2019
#ifndef _MBCS
#define _MBCS
#endif
#pragma message("Building for Mastercam _MBCS Character Set")
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#include <afxwin.h>             // MFC core and standard components
#include <afxext.h>             // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>             // MFC OLE classes
#include <afxodlgs.h>           // MFC OLE dialog classes
#include <afxdisp.h>            // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>              // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>             // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// Mastercam specific items:
// These standard includes are required here starting with Mastercam 2018
#include <array>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

// This will cause the proper BCGCBPRO#######.lib to be linked in.
#include "BCGCBProInc.h"

// Include these "main" Mastercam header files.
#include "m_core.h"
#include "m_mastercam.h"
#if C_H_VERSION >= 2200 // Mastercam 2020 or later

#pragma comment(lib, "MCCoreBase.lib")
#include "m_corebase.h"
#include "m_guibase.h"
#endif

// Optional additional 'master' headers that include groups of related headers ->
#include "m_mill.h"
#include "m_solids.h"
// #include "m_msurf.h"
// #include "m_post.h"
// #include "m_solids.h"
#include "m_tool.h"
// #include "m_wire.h"
// #include "m_bnci.h"
// #include "m_cad.h"
// #include "m_cadutil.h"
// #include "m_control.h"
// #include "m_corebase.h"
// #include "m_curvesurfgeom.h"
// #include "m_guibase.h"
// #include "m_lathe.h"
// #include "m_machinedef.h"
// #include "m_math.h"

#include <vector>
// The "BNciBdry_CH.h" after 2018 no longer contains "chains_intersect".
// For 2019 and on we need to use the "ChainsIntersect" and related imported functions
// in MastercamImports.h
#if C_H_VERSION < 2100 // If prior to Mastercam 2019
#include "BNciBdry_CH.h"
#else
#endif

