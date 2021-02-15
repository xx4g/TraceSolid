// <copyright file="Toolbar.cpp" company="CNC Software, Inc.">
// Copyright (c) 2017 CNC Software, Inc. All rights reserved.
// </copyright>
// <summary>Implements the Toolbar class</summary>

#include "stdafx.h"
#include "resource.h"
#include "Toolbar.h"
#include "OpMgrPage.h"  // GetChookResourceHandle()

MgrToolBar::MgrToolBar () : UIToolbar ()
	{
	pParentWnd = nullptr;

	// Don't show toolbar's of this type in customization dialog
	SetAllowShowOnList (false);

	// Don't scale the locked images
	CBCGPToolBar::m_bDontScaleLocked = true;
	}

MgrToolBar::~MgrToolBar ()
	{
	}

BEGIN_MESSAGE_MAP (MgrToolBar, UIToolbar)
	//{{AFX_MSG_MAP(MgrToolBar)
	//ON_UPDATE_COMMAND_UI_RANGE (ID_OPMGR_SELECT_ALL, ID_OPMGR_HELP, OnUpdateToolbarButtons)
	//ON_UPDATE_COMMAND_UI (IDR_TOOLBAR, OnUpdateToolbarButtons)
	//ON_WM_MOUSEMOVE ()
	//ON_WM_CONTEXTMENU ()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP ()

BOOL MgrToolBar::OnUserToolTip (CBCGPToolbarButton *pButton, CString &tipText) const
	{
	if (pButton)
		{
		// Make sure we have a valid command ID
		if (pButton->m_nID > 0)
			{
			ChangeResCl resource (GetChookResourceHandle ());
			tipText.LoadString (pButton->m_nID);
			return TRUE;
			}
		}

	return UIToolbar::OnUserToolTip (pButton, tipText);
	}

void MgrToolBar::AdjustLayout ()
	{
	// Call base class AdjustLayout ()
	UIToolbar::AdjustLayout ();

	// If necessary call parent's AdjustLayout ()
	if (pParentWnd && pParentWnd->IsKindOf (RUNTIME_CLASS (MCDialogBar)))
		{
		MCDialogBar *pDialogBar = dynamic_cast <MCDialogBar *> (pParentWnd);
		if (pDialogBar)
			{
			pDialogBar->AdjustLayout ();
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
// MgrToolBar message handlers

// Purpose: Handles ON_UPDATE_COMMAND_UI_RANGE messages.
//
// Category: Message handler.

//void MgrToolBar::OnUpdateToolbar (CCmdUI* pCmdUI)
//	{
//	TRACE1 (_T ("\nOnUpdateToolbar-> %i"), pCmdUI->m_nID);
//
//	pCmdUI->Enable (TRUE);
//	}
//
//void MgrToolBar::OnUpdateToolbarButtons (CCmdUI* pCmdUI)
//	{
//	TRACE1 (_T ("\nOnUpdateToolbarButtons-> %i"), pCmdUI->m_nID);
//
//	pCmdUI->Enable (TRUE);
//	}

/////////////////////////////////////////////////////////////////////////////
// MgrToolBar message handlers

// Purpose: Handles ON_UPDATE_COMMAND_UI_RANGE messages.
//
// Category: Message handler.

//void MgrToolBar::OnUpdateToolbar (CCmdUI* pCmdUI)
//	{
//	TRACE1 (_T ("\nOnUpdateToolbar-> %i"), pCmdUI->m_nID);
//
//	pCmdUI->Enable (TRUE);
//	}
//
//void MgrToolBar::OnUpdateToolbarButtons (CCmdUI* pCmdUI)
//	{
//	TRACE1 (_T ("\nOnUpdateToolbarButtons-> %i"), pCmdUI->m_nID);
//
//	pCmdUI->Enable (TRUE);
//	}


BOOL MgrToolBar::OnButtonPress (UINT ctrlID)
	{
	// For the LoadString calls below...
	ChangeResCl resource (GetChookResourceHandle ());

	CString msg;
	switch (ctrlID)
		{
		case ID_OPMGR_SELECT_ALL:
			post_FT_command (_T("Mastercam"), _T("OMselectAllOperations"));
			//msg.LoadString (ID_OPMGR_SELECT_ALL);
			break;
		case ID_OPMGR_SELECT_DIRTY:
			post_FT_command (_T("Mastercam"), _T("OMselectDirtyOperations"));
			//msg.LoadString (ID_OPMGR_SELECT_DIRTY);
			break;
		case ID_OPMGR_REGEN_SELECTED:
			post_FT_command (_T("Mastercam"), _T("OMregenSelectedOperations"));
			//msg.LoadString (ID_OPMGR_REGEN_SELECTED);
			break;
		case ID_OPMGR_REGEN_DIRTY:
			post_FT_command (_T("Mastercam"), _T("OMregenDirtyOperations"));
			//msg.LoadString (ID_OPMGR_REGEN_DIRTY);
			break;
		case ID_OPMGR_BACKPLOT:
			post_FT_command (_T("Mastercam"), _T("OMbackplotSelectedOperations"));
			//msg.LoadString (ID_OPMGR_BACKPLOT);
			break;
		case ID_OPMGR_VERIFY:
			post_FT_command (_T("Mastercam"), _T("OMverifySelectedOperations"));
			//msg.LoadString (ID_OPMGR_VERIFY);
			break;
		case ID_OPMGR_UBVS_OPTIONS:
			post_FT_command (_T("Mastercam"), _T("OMUBVSOptions"));
			//msg.LoadString (ID_OPMGR_UBVS_OPTIONS);
			break;
		case ID_OPMGR_POST:
			post_FT_command (_T("Mastercam"), _T("OMpostSelectedOperations"));
			//msg.LoadString (ID_OPMGR_POST);
			break;
		case ID_OPMGR_HIFEED:
			post_FT_command (_T("Mastercam"), _T("OMregenDirtyOperations"));
			//msg.LoadString (ID_OPMGR_HIFEED);
			break;
		case ID_OPMGR_NEW:
			post_FT_command (_T("Mastercam"), _T("OMdeleteAllOperationsAndTools"));
			//msg.LoadString (ID_OPMGR_NEW);
			break;
		case ID_OPMGR_HELP:
			post_FT_command (_T("Mastercam"), _T("OMhelp"));
			//msg.LoadString (ID_OPMGR_HELP);
			break;
		}

	if (!msg.IsEmpty ())
		{
		AfxMessageBox (+_T("MgrToolBar -> ") + msg);
		return TRUE; // if we handle the click event here
		}

	return FALSE;
	}

BOOL MgrToolBar::CreateMgrToolBar (CWnd* pWnd, UINT tbID, UINT tbIDLarge)
	{
	pParentWnd = nullptr;
	if (pWnd)
		{
		ChangeResCl resource (GetChookResourceHandle ());
		pParentWnd = pWnd;
		DWORD style = WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_SIZE_FIXED | CBRS_TOOLTIPS | CBRS_FLYBY;
		DWORD ctrlStyle = TBSTYLE_FLAT;

		// We are now using a different size than FT Buttons.
		// NOTE: Legacy CFTToolBar objects won't scale properly after this line, but they are no longer displayed, and 
		// will be removed entirely from source eventually.
		CBCGPToolBar::m_dblLargeImageRatio = (double)24 / (double)16;

		BOOL ok = CreateEx (pParentWnd, ctrlStyle, style);
		if (ok)
			{
			CBCGPToolBarParams tbParams;
			tbParams.m_uiColdResID = tbParams.m_uiLargeColdResID = 0;
			tbParams.m_uiDisabledResID = tbParams.m_uiLargeDisabledResID = 0;
			tbParams.m_uiMenuResID = tbParams.m_uiMenuDisabledResID = 0;
			tbParams.m_uiHotResID = tbID;
			tbParams.m_uiLargeHotResID = tbIDLarge;
			ok = LoadToolBarEx (tbID, tbParams, true);
			if (ok)
				{
				// Remove this code below once BCG addresses this defect.
				// Don't scale the locked images
				CBCGPToolBar::m_bDontScaleLocked = true;

				SetBarStyle (
					GetBarStyle () &
					~(CBRS_GRIPPER | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

				SetOwner (pParentWnd);

				// Setup the command routing via this dialog, not the parent frame.
				SetRouteCommandsViaFrame (FALSE);

				// Disable customize button
				EnableCustomizeButton (false, (UINT)-1, _T (""));
				}
			}

		return ok;
		}

	return false;
	}
