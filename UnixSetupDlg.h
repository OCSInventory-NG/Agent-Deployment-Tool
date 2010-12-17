//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

#if !defined(AFX_UNIXSETUPDLG_H__2D4FA500_F064_44B5_8B9E_0FDF16DCCEB0__INCLUDED_)
#define AFX_UNIXSETUPDLG_H__2D4FA500_F064_44B5_8B9E_0FDF16DCCEB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UnixSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUnixSetupDlg dialog

class CUnixSetupDlg : public CDialog
{
// Construction
public:
	CUnixSetupDlg(CWnd* pParent = NULL);   // standard constructor

	// Set pointer to an object where to store agent settings filled in in this dialogbox
	void SetAgentSetting( CAgentSettings *pSettings);

// Dialog Data
	//{{AFX_DATA(CUnixSetupDlg)
	enum { IDD = IDD_UNIX_AGENT_SETUP_DIALOG };
	CComboBox	m_EtcCombo;
	CListBox	m_ListFiles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnixSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CAgentSettings * m_pSettings;

	// Save agent settings into object and user registry
	BOOL Save( BOOL bDisplayError = FALSE);
	// Remove from registry all files stored under Windows Agent Other Files key
	// This allow storing new ones, without searching if value already exists
	BOOL RemoveOtherFilesFromRegistry();

	// Generated message map functions
	//{{AFX_MSG(CUnixSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonExe();
	afx_msg void OnButtonRemove();
	afx_msg void OnWiznext();
	afx_msg void OnWizback();
	virtual void OnCancel();
	afx_msg void OnDblclkListFiles();
	afx_msg void OnButtonChangeDirectory();
	afx_msg void OnCheckEtcDirectory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNIXSETUPDLG_H__2D4FA500_F064_44B5_8B9E_0FDF16DCCEB0__INCLUDED_)
