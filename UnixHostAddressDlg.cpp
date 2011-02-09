//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// UnixHostAddressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AgentSettings.h"
#include "ocs_deploy_tool.h"
#include "UnixHostAddressDlg.h"
#include "FileVersion.h"

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
		// Get tool version
		CFileVersion fileVer;
		CString		 csVersion;
		// Get application path	
		if (GetModuleFileName( AfxGetInstanceHandle(), csVersion.GetBuffer( 4*_MAX_PATH+1), 4*_MAX_PATH) == 0)
		{
			csVersion.Empty();
		}
		else
		{
			csVersion.ReleaseBuffer();
			// Open application file to get version from file
			if (fileVer.Open( csVersion))
			{
				csVersion = fileVer.GetProductVersion();
				csVersion.Remove( ' ');
				csVersion.Replace( ',', '.');
				fileVer.Close();
			}
			else
				csVersion.Empty();
		}
		csMessage.FormatMessage( IDS_OCS_DEPLOY_TOOL, csVersion);
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
