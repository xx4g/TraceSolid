// <copyright file="MgrPageDialogBar.h" company="CNC Software, Inc.">
// Copyright (c) 2017 CNC Software, Inc. All rights reserved.
// </copyright>
// <summary>Declares the MgrPageDialogBar class</summary>

#pragma once

#include <memory>
#include "MyMgrPage.h"

class MgrPageDialogBar
	{
public:

	/// <summary> Initializes a new instance of the MgrPageDialogBar class. </summary>
	MgrPageDialogBar ();

	/// <summary> Finalizes an instance of the MgrPageDialogBar class. </summary>
	virtual ~MgrPageDialogBar ();

	/// <summary> Was our DialogBar added to Mastercam's list of "user created" bars?. </summary>
	bool PaneAdded;

	/// <summary> Adds a pane to Mastercam's (Toolpath/Solids) Manager. </summary>
	///
	/// <param name="title"> (Optional) The caption title. </param>
	///
	/// <returns> true if it succeeds, false if it fails. </returns>
	bool AddDialogBarPane (LPCTSTR title = nullptr);

	/// <summary> Removes the pane (DialogBar) that we've created from Mastercam's
	///           list of "user created" DialogBars. </summary>
	///
	/// <returns> true if it succeeds, false if it fails. </returns>
	bool RemoveDialogBarPane ();

	/// <summary> Saves the 'display state' of the DialogBar. </summary>
	///
	/// <returns> true if it succeeds, false if it fails. </returns>
	bool SaveState ();

	/// <summary> Restores the 'display state' of the DialogBar. </summary>
	///
	/// <param name="eventSource"> (Optional) the event source name. </param>
	///
	/// <returns> true if it succeeds, false if it fails. </returns>
	bool RestoreState (LPCTSTR eventSource = nullptr);

protected:

	/// <summary> The pointer to the DialogBar (pane) we create which is part of
	///           Mastercam's (Toolpaths/Solids/Planes/Levels) Manager pane list. </summary>
	std::shared_ptr<MyMgrPage> DialogBarPane;
	};
