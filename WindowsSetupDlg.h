//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

#if !defined(AFX_WINDOWSSETUPDLG_H__16957595_BAD8_41CD_B495_3D45DF467B55__INCLUDED_)
#define AFX_WINDOWSSETUPDLG_H__16957595_BAD8_41CD_B495_3D45DF467B55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WindowsSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWindowsSetupDlg dialog

class CWindowsSetupDlg : public CDialog
{
// Construction
public:
	CWindowsSetupDlg(CWnd* pParent = NULL);   // standard constructor

	// Set pointer to an object where to store agent settings filled in in this dialogbox
	void SetAgentSetting( CAgentSettings *pSettings);

// Dialog Data
	//{{AFX_DATA(CWindowsSetupDlg)
	enum { IDD = IDD_WIN_AGENT_SETUP_DIALOG };
	CListBox	m_ListFiles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindowsSetupDlg)
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
	//{{AFX_MSG(CWindowsSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnWiznext();
	afx_msg void OnWizback();
	virtual void OnCancel();
	afx_msg void OnButtonExe();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnDblclkListFiles();
	afx_msg void OnButtonChangeDirectory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOWSSETUPDLG_H__16957595_BAD8_41CD_B495_3D45DF467B55__INCLUDED_)
