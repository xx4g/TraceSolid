// <copyright file="MgrPageDialogBar.cpp" company="CNC Software, Inc.">
// Copyright (c) 2017 CNC Software, Inc. All rights reserved.
// </copyright>
// <summary>Implements the MgrPageDialogBar class</summary>

#include "stdafx.h"
#include "CGui_ch.h"    // AddDialogBar() / RemoveDialogBar()
// Project includes...
#include "MyMgrPage.h"
#include "MgrPageDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#pragma region Construction / Destruction

MgrPageDialogBar::MgrPageDialogBar ()
	{
	PaneAdded = false;
	}

MgrPageDialogBar::~MgrPageDialogBar ()
	{
	RemoveDialogBarPane ();
	}

#pragma endregion

#pragma region Methods (public)

bool MgrPageDialogBar::AddDialogBarPane (LPCTSTR title)
	{
	bool result = false;

	// Don't add it more than once.
	if (DialogBarPane != nullptr && PaneAdded)
		{
		TRACE (DialogBarPane->GetCaption () + _T (" already exists!\n"));
		return true;
		}

	PaneAdded = false;

	// Retrieve Mastercam's built-in Toolpath Manager.
	MCDialogBar *pToolpathMgrBar = GetDialogBar (MCDialogBarType::MC_TOOLPATHMGRBAR);
	if (pToolpathMgrBar == nullptr)
		{
		return false;
		}

	DialogBarPane = std::make_shared<MyMgrPage> ();

	if (DialogBarPane)
		{
		DialogBarPane->m_MyBarID = AddDialogBar (DialogBarPane.get ());
		result = DialogBarPane->m_MyBarID > 0;

		if (result)
			{
			// This extra check isn't really required, but...
			// If we "really" want to be absolutely sure that the system knows about our DialogBar,
			// we can try to retrieve the bar we just created.
			if (GetDialogBar (MCDialogBarType::MC_EXTERNALBAR, DialogBarPane->m_MyBarID) == nullptr)
				{
				RemoveDialogBarPane ();
				return false;
				}

			DialogBarPane->SetAddinBar (true);
			DialogBarPane->EnableDocking (CBRS_ALIGN_ANY);

			DialogBarPane->SetCaption (title != nullptr ? title : _T ("Trace Solid Add-In"));

			if (pToolpathMgrBar->IsAutoHideMode ())
				{
				DialogBarPane->SetAutoHideMode (TRUE, pToolpathMgrBar->GetCurrentAlignment ());
				}
			else
				{
				DialogBarPane->AttachToTabWnd (pToolpathMgrBar, BCGP_DM_SHOW, FALSE);
				}
			}
		}

	PaneAdded = result;

	// If something has gone wrong, clean up now.
	if (!result)
		{
		RemoveDialogBarPane ();
		}

	return PaneAdded;
	}


bool MgrPageDialogBar::RemoveDialogBarPane ()
	{
	bool result = true;

	if (DialogBarPane != nullptr)
		{
		result = RemoveDialogBar (DialogBarPane.get ());
		}

	PaneAdded = false; // assume?
	return result;
	}


bool MgrPageDialogBar::SaveState ()
	{
	// TODO: Save some "state" information?
	return false;
	}


bool MgrPageDialogBar::RestoreState (LPCTSTR eventSource)
	{
	bool success = false;

	if (DialogBarPane != nullptr)
		{
		if (eventSource != nullptr)
			{
			TRACE (_T ("RestoreState - %s\n"), eventSource);
			}

		CMastercamInterface *pMastercamInterface = GetApp ();

		if (pMastercamInterface != nullptr)
			{
			MCDialogBar *pToolpathMgrBar = GetDialogBar (MC_TOOLPATHMGRBAR);

			if (pToolpathMgrBar != nullptr)
				{
				if (pToolpathMgrBar->IsAutoHideMode ())
					{
					DialogBarPane->SetAutoHideMode (TRUE, pToolpathMgrBar->GetCurrentAlignment ());
					}
				else
					{
					DialogBarPane->AttachToTabWnd (pToolpathMgrBar, BCGP_DM_SHOW, FALSE);
					}

				success = DialogBarPane != nullptr;
				}
			}
		}

	return success;
	}

#pragma endregion
