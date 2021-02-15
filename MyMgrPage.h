// <copyright file="MyMgrPage.h" company="CNC Software, Inc.">
// Copyright (c) 2017 CNC Software, Inc. All rights reserved.
// </copyright>
// <summary>Declares the MyMgrPage class</summary>

#pragma once

#include "resource.h"
#include "UITreeCtrl.h"
#include "Toolbar.h"

// These are for the optional customize menu for the tree
#define ID_ATTRIBUTES_BACKGROUNDCOLOR          33299
#define ID_ATTRIBUTES_LINECOLOR                33301
#define ID_ATTRIBUTES_FONT                     33303
#define ID_ATTRIBUTES_RESTOREDEFAULTATTRIBUTES 33305

/////////////////////////////////////////////////////////////////////////////
// MyMgrPage

class MyMgrPage : public MCDialogBar
	{
	DECLARE_DYNAMIC (MyMgrPage)

public:

	/// <summary> Initializes a new instance of the MyMgrPage class. </summary>
	MyMgrPage ();

	/// <summary> Finalizes an instance of the MyMgrPage class. </summary>
	virtual ~MyMgrPage ();

	/// <summary> Load some sample data into the tree. </summary>
	void LoadTree ();

	/// <summary> Gets the caption text of this dialog. </summary>
	CString GetCaption ();

	/// <summary> Sets the caption text of this dialog. </summary>
	void SetCaption (LPCTSTR caption);

	/// <summary> Sets the non-toolbar tooltips. </summary>
	void InitToolTip ();

	/// <summary> Executes the menu selection action. </summary>
	///
	/// <param name="selection"> The selection. </param>
	virtual void OnMenuSelection (const int selection);
	
	/// <summary> Adjust the layout of the dialog bar. </summary>
	virtual void AdjustLayout ();

	virtual BOOL PreTranslateMessage (MSG* pMsg);

// Dialog Data
	//{{AFX_DATA(MyMgrPage)
	UINT m_MyBarID; // The unique ID number of this DialogBar
	//}}AFX_DATA
	CToolTipCtrl m_tooltip;
	UITreeCtrl m_Tree;
	CButton m_CheckBox1;
	CButton m_CheckBox2;
	BOOL m_Option1_State;
	BOOL m_Option2_State;
	CEdit m_EditCtrl1;
	CEdit m_EditCtrl2;
	CString m_Text1;
	CString m_Text2;
	CStatic m_wndToolbarLocation;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(MyMgrPage)

protected:

	/// <summary> Called by the framework to exchange and validate dialog data. </summary>
	///
	/// <remarks> See CWnd::DoDataExchange (). </remarks>
	///
	/// <param name="pDX"> A pointer to a CDataExchange object. </param>
	virtual void DoDataExchange (CDataExchange *pDX); // DDX/DDV support

	/// <summary> Handle WM_COMMAND messages. </summary>
	///
	/// <remarks> Overridden to process button presses on embedded toolbar. </remarks>
	///
	/// <param name="wParam"> WMAPARM for WM_COMMAND. </param>
	/// <param name="lParam"> LPARAM for WM_COMMAND </param>
	///
	/// <returns> true if handled, false otherwise. </returns>
	virtual BOOL OnCommand (WPARAM wParam, LPARAM lParam);

	///// <summary> Overridden to allow control over the loading of the dialog bar's state information. </summary>
	/////
	///// <remarks> See CBCGPDialogBar::LoadState (). </remarks>
	/////
	///// <param name="lpszProfileName"> Optional profile name. </param>
	///// <param name="nIndex">          Optional index. </param>
	///// <param name="uiID">            Optional ID. </param>
	/////
	///// <returns> TRUE if successful, FALSE otherwise. </returns>
	//virtual BOOL LoadState (LPCTSTR lpszProfileName = nullptr, int nIndex = -1, UINT uiID = (UINT) -1);

	///// <summary> Overridden to allow control over the saving of the dialog bar's state information. </summary>
	/////
	///// <remarks> See CBCGPDialogBar::SaveState (). </remarks>
	/////
	///// <param name="lpszProfileName"> Optional profile name. </param>
	///// <param name="nIndex">          Optional index. </param>
	///// <param name="uiID">            Optional ID. </param>
	/////
	///// <returns> TRUE if successful, FALSE otherwise. </returns>
	//virtual BOOL SaveState (LPCTSTR lpszProfileName = nullptr, int nIndex = -1, UINT uiID = (UINT) -1);
	//}}AFX_VIRTUAL

// Generated message map functions
	//{{AFX_MSG(MyMgrPage)

protected:

	/// <summary> Executes the set focus action. </summary>
	///
	/// <param name="pOldWnd"> If non-null, the old window. </param>
	afx_msg void OnSetFocus (CWnd *pOldWnd);

	/// <summary> Executes the kill focus action. </summary>
	///
	/// <param name="pNewWnd"> If non-null, the new window. </param>
	afx_msg void OnKillFocus (CWnd *pNewWnd);

	/// <summary> Handles the WM_INITDIALOG message. </summary>
	///
	/// <remarks> Overridden to allow initialization of dialog bar.
	///
	/// <param name="wParam"> A handle to the control to receive the default keyboard focus.  </param>
	/// <param name="lParam"> Additional initialization data. </param>
	///
	/// <returns> Always zero. </returns>
	afx_msg LRESULT OnInitDialog (WPARAM wParam, LPARAM lParam);

	/// <summary> Handle WM_SIZE messages. </summary>
	///
	/// <remarks> Overridden to allow resizing of tree control. </remarks>
	///
	/// <param name="nType"> The type of resizing requested. </param>
	/// <param name="cx">    New width of client area. </param>
	/// <param name="cy">    New height of client area. </param>
	afx_msg void OnSize (UINT nType, int cx, int cy);

	/// <summary> Executes the checkbox clicked action. </summary>
	afx_msg void OnBnClickedCheck1 ();

	/// <summary> Executes the checkbox clicked action. </summary>
	afx_msg void OnBnClickedCheck2 ();

	/// <summary> Handles Tool Tip Text actions. </summary>
	afx_msg BOOL OnToolTipText (UINT, NMHDR *pNMHDR, LRESULT *pResult);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP ()

	/// <summary> Gets the toolbar. </summary>
	///
	/// <returns> null if it fails, else the toolbar. </returns>
	MgrToolBar *GetToolbar ();

	/// <summary> Update UI Command handler for the toolbar buttons. </summary>
	///
	/// <param name="pCmdUI"> [in,out] The command UI pointer. </param>
	void OnUpdateToolbarButtons (CCmdUI *pCmdUI);

private:

	enum { IDD = IDD_MYMGRBAR };

	MgrToolBar m_ToolBar; //!< Our toolbar

	// Resizer stuff
	CResizer m_resizer;
	bool m_bResizerInit;

	/// <summary> Expand  or collapse a tree branch. </summary>
	///
	/// <param name="tree">  The tree. </param>
	/// <param name="hItem"> Handle of the item to begin working from. </param>
	/// <param name="nCode"> The action code. (TVE_EXPAND or TVE_COLLAPSE). </param>
	void ExpandCollapseTree (CTreeCtrl &tree, HTREEITEM hItem, UINT nCode);

	/// <summary> Expand or collapse the entire tree . </summary>
	///
	/// <param name="treeCtrl"> The tree. </param>
	/// <param name="nCode"> The action code. (TVE_EXPAND or TVE_COLLAPSE). </param>
	void TreeExpandCollapseAll (CTreeCtrl &tree, UINT nCode);

	/// <summary> Sets up the dialog resizing. </summary>
	void SetupResizing ();

	// *NOT USED*
	BOOL SaveState (LPCTSTR lpszProfileName, int nIndex, UINT uiID);
	BOOL LoadState (LPCTSTR lpszProfileName, int nIndex, UINT uiID);
	public:
		afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnTvnSelchangingTree(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
