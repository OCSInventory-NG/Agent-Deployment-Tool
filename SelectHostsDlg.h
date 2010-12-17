//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

#if !defined(AFX_SELECTHOSTSDLG_H__5367C094_F489_4D6F_ABBA_70C4D916E269__INCLUDED_)
#define AFX_SELECTHOSTSDLG_H__5367C094_F489_4D6F_ABBA_70C4D916E269__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectHostsDlg.h : header file
//

#define MAX_IP_RANGE	255

/////////////////////////////////////////////////////////////////////////////
// CSelectHostsDlg dialog

class CSelectHostsDlg : public CDialog
{
// Construction
public:
	CSelectHostsDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Set pointer to a string list where to store name/address of computer to setup
	void SetComputerList( CStringList *pList, UINT uOS = AGENT_OS_WINDOWS);

// Dialog Data
	//{{AFX_DATA(CSelectHostsDlg)
	enum { IDD = IDD_SELECT_HOSTS_DIALOG };
	CListBox	m_List;
	CIPAddressCtrl	m_IpTo;
	CIPAddressCtrl	m_IpFrom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectHostsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CStringList * m_pComputerList;
	UINT		  m_uOS;

	void AddUnixComputer();
	void AddWindowsComputer();

	// Generated message map functions
	//{{AFX_MSG(CSelectHostsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemove();
	afx_msg void OnRadioIp();
	afx_msg void OnRadioList();
	virtual void OnCancel();
	afx_msg void OnWiznext();
	afx_msg void OnDblclkListComputers();
	afx_msg void OnButtonImport();
	afx_msg void OnButtonSelectAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL AddNetwork( BYTE nIpFromA, BYTE nIpFromB, BYTE nIpFromC, BYTE nIpFromD, BYTE nIpToA, BYTE nIpToB, BYTE nIpToC, BYTE nIpToD);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTHOSTSDLG_H__5367C094_F489_4D6F_ABBA_70C4D916E269__INCLUDED_)
