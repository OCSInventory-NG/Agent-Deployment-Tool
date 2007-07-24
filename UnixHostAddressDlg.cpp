// UnixHostAddressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AgentSettings.h"
#include "ocs_deploy_tool.h"
#include "UnixHostAddressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnixHostAddressDlg dialog


CUnixHostAddressDlg::CUnixHostAddressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnixHostAddressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnixHostAddressDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUnixHostAddressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnixHostAddressDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUnixHostAddressDlg, CDialog)
	//{{AFX_MSG_MAP(CUnixHostAddressDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnixHostAddressDlg message handlers

BOOL CUnixHostAddressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try
	{
		CString	csMessage;
		
		csMessage.LoadString( IDS_OCS_DEPLOY_TOOL);
		SetDlgItemText( IDC_STATUS, csMessage);
		SetDlgItemText( IDC_EDIT_LOGIN, m_csHost);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUnixHostAddressDlg::OnOK() 
{
	// TODO: Add extra validation here
	try
	{
		GetDlgItemText( IDC_EDIT_LOGIN, m_csHost);
		CDialog::OnOK();
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
}

void CUnixHostAddressDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	try
	{
		GetDlgItemText( IDC_EDIT_LOGIN, m_csHost);
		CDialog::OnCancel();
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
}

LPCTSTR CUnixHostAddressDlg::GetHost()
{
	return m_csHost;
}
