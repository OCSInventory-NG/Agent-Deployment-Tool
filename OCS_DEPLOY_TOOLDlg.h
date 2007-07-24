//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// OCS_DEPLOY_TOOLDlg.h : header file
//

#if !defined(AFX_OCS_DEPLOY_TOOLDLG_H__48063A1C_7DD8_4918_8F8F_30EA88D9DBBF__INCLUDED_)
#define AFX_OCS_DEPLOY_TOOLDLG_H__48063A1C_7DD8_4918_8F8F_30EA88D9DBBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COCS_DEPLOY_TOOLDlg dialog

class COCS_DEPLOY_TOOLDlg : public CDialog
{
// Construction
public:
	COCS_DEPLOY_TOOLDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COCS_DEPLOY_TOOLDlg)
	enum { IDD = IDD_OCS_DEPLOY_TOOL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCS_DEPLOY_TOOLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringList m_ComputerList;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COCS_DEPLOY_TOOLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWinDeployButton();
	afx_msg void OnUnixDeployButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OCS_DEPLOY_TOOLDLG_H__48063A1C_7DD8_4918_8F8F_30EA88D9DBBF__INCLUDED_)
