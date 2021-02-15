// <copyright file="main.cpp" company="CNC Software, Inc.">
// Copyright (c) 2017 CNC Software, Inc. All rights reserved.
// </copyright>
// <summary>Implements the main startup functions for this C-Hook</summary>

#include "stdafx.h"
#include "CGui_ch.h"    // AddDialogBar() / RemoveDialogBar()
// Project includes...
#include "OpMgrPage.h"  // GetChookResourceHandle()
#include "MyMgrPage.h"
#include "MgrPageDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define SHOWEVENTS // Show Event "hits" in a MessageBox

// Our DialogBar (Manager docked style pane)
MgrPageDialogBar mgrPageDialogBar;

//--------------------------------------------------------------

#pragma region Debugging messages

/// <summary> Shows the event message. </summary>
///
/// <param name="msg"> The message. </param>
static void ShowEventMsg (LPCTSTR msg)
	{
	TRACE (msg);
#ifdef SHOWEVENTS
	AfxMessageBox (msg);
#endif
	}

/// <summary> Shows the error message. </summary>
///
/// <param name="msg"> The message. </param>
static void ShowErrorMsg (LPCTSTR msg)
	{
	TRACE (msg);
	AfxMessageBox (msg);
	}

#pragma endregion

/// <summary> Creates the dialog bar panes. </summary>
///
/// <returns> true if it succeeds, false if it fails. </returns>
bool CreateDialogBarPanes ()
	{
	bool createdPane1 = true;
	bool createdPane2 = true;

	// Add the docked type pane.
	createdPane1 = mgrPageDialogBar.AddDialogBarPane ();
	if (!createdPane1)
		{
		ShowErrorMsg (_T ("FALSE! -> mgrPageDialogBarAdded"));
		}

	return createdPane1 && createdPane2;
	}

#pragma region Standard C-Hook methods

/// <summary> The REQUIRED m_version function. </summary>
///
/// <remarks> Mastercam calls this function first, when your C-Hook is about to be run.
///           It passes in a Version# that represents the current version of Mastercam
///           that is about to attempt to run the C-Hook.
///           This allows you to check if your C-Hook can run on this version of Mastercam.
///           If so, just return (to Mastercam) the same version number that was supplied,
///           else some other value to abort the running of the C-Hook.
///           </remarks>
///
/// <param name="version"> The version (supplied by Mastercam). </param>
///
/// <returns> The supplied version if OK to run, else any other value to abort. </returns>
extern "C" __declspec (dllexport) int m_version (int version)
	{
	int ret = C_H_VERSION;

	// This allows the C-Hook to run in any version of Mastercam
	// that has the same major version
	if ((version / 100) == (C_H_VERSION / 100))
		ret = version;

	return ret;
	}

/// <summary> The entry point function referenced in an associated Function Table (FT) file. </summary>
///
/// <remarks> If a C-Hook Add-In has an associated Function Table (FT) file,
///           you can specific one (or more) entry points into the Add-In.
///           The FT file also allows (requires) you to specify (2) icon images and a string
///           tooltip description for the Add-In. These resources will appear in Mastercam's
///           File-Options allowing a user to add the Add-In's icon to Mastercam.
///           This function's name must appear in a FUNCTION CPP line in the FT file in order to get called.
///           </remarks>
///
/// <param name="optional"> An optional integer value passed via a 'PARAM #' statement in the FT file. 
///                         (Default value is always 0) </param>
///
/// <returns> The MC_RETURN state flag(s) back to Mastercam. </returns>
extern "C" __declspec (dllexport) int TraceSolidEntry (int optional)
	{
	// Must call this prior to accessing any Resources in the C-Hook DLL !
	ChangeResCl res (GetChookResourceHandle ());

	// You do NOT want to add the Dialog Panes here, if they've already activated via the 'm_open' function of the add-in!
	// ... do else something here? ...

	return MC_NOERROR;
	}

/// <summary> The REQUIRED m_main entry point function. </summary>
///
/// <remarks> This is the main entry point function of the C-Hook Add-In.
///           It is called by Mastercam to start the C-Hook, when...
///           The C-Hook is started via the Home Tab - Run Add-in command.
///           - or -
///           The if this C-Hook has an associated Function Table (FT) file that references
///           this 'm_main' in a FUNCTION CPP line.
///           </remarks>
/// <param name="not_used"> not used. </param>
///
/// <returns> The MC_RETURN state flag(s) back to Mastercam. </returns>
extern "C" __declspec (dllexport) int m_main (int not_used)
	{
	// You do NOT want to add the Dialog Panes here, if they've already activated via the 'm_open' function of the add-in!
	//OpMgrPageEntry (0);

	return MC_NOERROR;
	}

#pragma endregion

#pragma region C-Hook Open/Close methods

/// <summary> Called automatically by Mastercam when this C-Hook is being loaded. </summary>
///
/// <param name="not_used"> not used. </param>
///
/// <returns> Just return MC_NOERROR (this return value is not really used by Mastercam). </returns>
extern "C" __declspec (dllexport) int m_open (int not_used)
	{
	// Must call this prior to accessing any Resources in the C-Hook DLL !
	ChangeResCl res (GetChookResourceHandle ());

	// Add our pane(s)...
	bool result = CreateDialogBarPanes ();

	return MC_NOERROR;
	}

/// <summary> Called automatically by Mastercam when this C-Hook is being unloaded. </summary>
///
/// <param name="not_used"> not used. </param>
///
/// <returns> Just return MC_NOERROR (this return value is not really used by Mastercam). </returns>
extern "C" __declspec (dllexport) int m_close (int not_used)
	{
	// ... Do else something here? Like some cleanup that may be needed by this add-in? ...
	return MC_NOERROR;
	}

/// <summary> (Optional) Receives notifications of events from within Mastercam </summary>
///
/// <param name="notify_code"> The event code. </param>
///
/// <returns> Just return MC_NOERROR (this return value is not really used by Mastercam). </returns>
extern "C" __declspec (dllexport) int m_notify (int event_code)
	{
	switch (event_code)
		{

		case MCEVENT_DIALOGBAR_DETACH: // When a dialog bar needs to be detached
			ShowEventMsg (_T ("MCEVENT_DIALOGBAR_DETACH\n"));
			break;

		case MCEVENT_DIALOGBAR_REATTACH: // When a dialog bar needs to be re-attached
			mgrPageDialogBar.RestoreState (_T ("MCEVENT_DIALOGBAR_REATTACH\n"));
			break;

		case MCEVENT_WORKSPACE_PRELOAD: // Called before a workspace is loaded
			ShowEventMsg (_T ("MCEVENT_WORKSPACE_PRELOAD\n"));
			break;

		case MCEVENT_WORKSPACE_POSTLOAD: // Called after a workspace is loaded
			mgrPageDialogBar.RestoreState (_T ("MCEVENT_WORKSPACE_POSTLOAD\n"));
			break;

		case MCEVENT_WORKSPACE_PRESAVE: // Called before a workspace is saved
			ShowEventMsg (_T ("MCEVENT_WORKSPACE_PRESAVE\n"));
			break;

		case MCEVENT_WORKSPACE_POSTSAVE: // Called after a workspace is saved
			mgrPageDialogBar.RestoreState (_T ("MCEVENT_WORKSPACE_POSTSAVE\n"));
			break;

		case MCEVENT_REPAINT:
			// ...
			break;

		case MCEVENT_FIT:
			// ...
			break;
			// etc...
		}

	return MC_NOERROR;
	}

#pragma endregion
