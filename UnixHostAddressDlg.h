//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

#if !defined(AFX_UNIXHOSTADDRESSDLG_H__A305F207_D0B1_46CD_8FE0_1B460F8FD938__INCLUDED_)
#define AFX_UNIXHOSTADDRESSDLG_H__A305F207_D0B1_46CD_8FE0_1B460F8FD938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UnixHostAddressDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUnixHostAddressDlg dialog

class CUnixHostAddressDlg : public CDialog
{
// Construction
public:
	LPCTSTR GetHost();
	CUnixHostAddressDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUnixHostAddressDlg)
	enum { IDD = IDD_HOST_ADDRESS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnixHostAddressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_csHost;

	// Generated message map functions
	//{{AFX_MSG(CUnixHostAddressDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNIXHOSTADDRESSDLG_H__A305F207_D0B1_46CD_8FE0_1B460F8FD938__INCLUDED_)
