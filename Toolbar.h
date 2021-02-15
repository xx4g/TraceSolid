// <copyright file="Toolbar.h" company="CNC Software, Inc.">
// Copyright (c) 2017 CNC Software, Inc. All rights reserved.
// </copyright>
// <summary>Declares the Toolbar class</summary>

#pragma once
#include "UIToolbar.h"

class MgrToolBar : public UIToolbar
	{

	friend class MyMgrPage;

public:
	/// <summary> Initializes a new instance of the MgrToolBar class. </summary>
	MgrToolBar ();
	BOOL CreateMgrToolBar (CWnd *pWnd, /*COpMgrTree *pOpMgrTree,*/ UINT tbID, UINT tbIDLarge);
	BOOL OnButtonPress (UINT ctrlID);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MgrToolBar)

	/// <summary> Finalizes an instance of the MgrToolBar class. </summary>
	virtual ~MgrToolBar ();

	/// <summary> Check to see if tooltip info can be found for given toolbar button. </summary>
	///
	/// <param name="pButton">  Points to a toolbar button, whose tooltip should be displayed. </param>
	/// <param name="tipText">  A reference to string object that receives the tooltip's text. </param>
	///
	/// <returns> Should return true if a tooltip was displayed; otherwise false. </returns>
	virtual BOOL OnUserToolTip (CBCGPToolbarButton *pButton, CString &tipText) const;

	/// <summary> Adjust the layout of the toolbar. </summary>
	virtual void AdjustLayout ();

	// All commands should be routed via owner window, not via frame:
	virtual void OnUpdateCmdUI (CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
		{
		CBCGPToolBar::OnUpdateCmdUI ((CFrameWnd*)GetOwner (), bDisableIfNoHndler);
		}

	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(MgrToolBar)
	//afx_msg void OnUpdateToolbarButtons (CCmdUI *pCmdUI);
	//afx_msg void OnUpdateToolbar (CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP ()

private:
	/// <summary> The parent window of this toolbar. </summary>
	CWnd *pParentWnd;   //!< The parent window
	};
