//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

#if !defined(AFX_CREDENTIALSDLG_H__6696CBD0_3628_4D61_BF21_983CDD9CD854__INCLUDED_)
#define AFX_CREDENTIALSDLG_H__6696CBD0_3628_4D61_BF21_983CDD9CD854__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CredentialsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCredentialsDlg dialog

class CCredentialsDlg : public CDialog
{
// Construction
public:
	CCredentialsDlg(CWnd* pParent = NULL);   // standard constructor

	// Set agent settings to use for deploying, either Windows or Unix settings
	void SetAgentSetting( CAgentSettings *pSettings);

// Dialog Data
	//{{AFX_DATA(CCredentialsDlg)
	enum { IDD = IDD_CREDENTIALS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCredentialsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Agent settings to use for deploying, either Windows or Unix settings
	CAgentSettings * m_pSettings;

	BOOL Save();

	// Generated message map functions
	//{{AFX_MSG(CCredentialsDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnWiznext();
	afx_msg void OnWizback();
	afx_msg void OnButtonExe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREDENTIALSDLG_H__6696CBD0_3628_4D61_BF21_983CDD9CD854__INCLUDED_)
