#pragma once

#include "ocs_deploy_tool.h"		// main symbols

// CToolsDialog dialog

class CToolsDialog : public CDialog
{
	DECLARE_DYNAMIC(CToolsDialog)

public:
	CToolsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CToolsDialog();

// Dialog Data
	enum { IDD = IDD_TOOLS_DIALOG };
	CMyHyperLink	m_PsExecLink;
	CMyHyperLink	m_PuttyLink;
	CMyHyperLink	m_PscpLink;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPsexec();
	afx_msg void OnBnClickedButtonPutty();
	afx_msg void OnBnClickedButtonPscp();
};
