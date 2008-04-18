//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

#if !defined(AFX_DEPLOYINGDLG_H__6C58DF2D_FA08_4888_84DA_DCC397DFF7F3__INCLUDED_)
#define AFX_DEPLOYINGDLG_H__6C58DF2D_FA08_4888_84DA_DCC397DFF7F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DeployingDlg.h : header file
//

// Number max of deployment threads
#define OPTIONS_MAX_SIMULTANEOUS_CONNECTIONS _T( "MaxThreads")
#define DEFAULT_MAX_SIMULTANEOUS_CONNECTIONS 10
// Default TimeOut for command execution
#define OPTIONS_COMMAND_TIMEOUT				 _T( "TimeOut")
#define DEFAULT_COMMAND_TIMEOUT				 300000

/////////////////////////////////////////////////////////////////////////////
// CDeployingDlg dialog

class CDeployingDlg : public CDialog
{
// Construction
public:
	CDeployingDlg(CWnd* pParent = NULL);   // standard constructor

	// Set agent settings to use for deploying, either Windows or Unix settings
	void SetAgentSetting( CAgentSettings *pSettings);
	// Set list of computer to setup
	void SetComputerList( CStringList *pList);

// Dialog Data
	//{{AFX_DATA(CDeployingDlg)
	enum { IDD = IDD_DEPLOYING_DIALOG };
	CSliderCtrl	m_SliderThreads;
	CListBox	m_ListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeployingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Message Handler allowing thread to update status and listbox content
	LRESULT OnMessageHandlerListbox( LPARAM lParam);
	LRESULT OnMessageHandlerStatus( LPARAM lParam);
	// Message Handler to allow launcher thread retreiving number of allowed threads
	LRESULT OnMessageHandlerGetMaxThreads();
	// Agent settings to use for deploying, either Windows or Unix settings
	CAgentSettings * m_pSettings;
	// List of computer to setup
	CStringList * m_pComputerList;
	// List of computer where setup failed
	CStringList m_FailedList;
	// Thread used to launch setup thread for each computer
	CWinThread* m_pThreadLauncher;
	// Max simultaneous connections
	UINT m_uMaxThreads;

	// Generated message map functions
	//{{AFX_MSG(CDeployingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnWizback();
	afx_msg void OnWizfinish();
	virtual void OnCancel();
	afx_msg void OnReleasedcaptureSliderThreads(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStopThreads();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonSaveLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEPLOYINGDLG_H__6C58DF2D_FA08_4888_84DA_DCC397DFF7F3__INCLUDED_)
