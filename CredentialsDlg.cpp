//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// CredentialsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OCS_DEPLOY_TOOL.h"
#include "AgentSettings.h"
#include "WinAgentSettings.h"
#include "UnixAgentSettings.h"
#include "CredentialsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCredentialsDlg dialog


CCredentialsDlg::CCredentialsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCredentialsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCredentialsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCredentialsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCredentialsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCredentialsDlg, CDialog)
	//{{AFX_MSG_MAP(CCredentialsDlg)
	ON_BN_CLICKED(ID_WIZNEXT, OnWiznext)
	ON_BN_CLICKED(ID_WIZBACK, OnWizback)
	ON_BN_CLICKED(IDC_BUTTON_EXE, OnButtonExe)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCredentialsDlg message handlers

BOOL CCredentialsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try
	{
		CString	csMessage;
		
		csMessage.LoadString( IDS_OCS_DEPLOY_TOOL);
		SetDlgItemText( IDC_STATUS, csMessage);
		switch (m_pSettings->GetTargetOS())
		{
		case AGENT_OS_UNIX:
			// Set root login
			csMessage = AfxGetApp()->GetProfileString( AGENT_SECTION, OPTION_UNIX_ROOT_LOGIN, _T( "root"));
			m_pSettings->SetLoginName( csMessage);
			SetDlgItemText( IDC_EDIT_LOGIN, m_pSettings->GetLoginName());
			// Set SSH key
			csMessage = AfxGetApp()->GetProfileString( AGENT_SECTION, OPTION_UNIX_SSH_KEY, _T( ""));
			m_pSettings->SetSshKey( csMessage);
			SetDlgItemText( IDC_EDIT_SSH_KEY, m_pSettings->GetSshKey());
			// Set help message
			csMessage.LoadString( IDS_STATUS_HELP_UNIX_CREDENTIALS);
			SetDlgItemText( IDC_EDIT_HELP_MESSAGE, csMessage);
			break;
		case AGENT_OS_WINDOWS:
			csMessage = AfxGetApp()->GetProfileString( AGENT_SECTION, OPTION_WIN_ADMIN_LOGIN, _T( ""));
			m_pSettings->SetLoginName( csMessage);
			SetDlgItemText( IDC_EDIT_LOGIN, m_pSettings->GetLoginName());
			// Set help message
			csMessage.LoadString( IDS_STATUS_HELP_WIN_CREDENTIALS);
			SetDlgItemText( IDC_EDIT_HELP_MESSAGE, csMessage);
			// Disable SSH
			GetDlgItem( IDC_EDIT_SSH_KEY)->EnableWindow( FALSE);
			GetDlgItem( IDC_BUTTON_EXE)->EnableWindow( FALSE);
			break;
		}
		// Set password
		SetDlgItemText( IDC_EDIT_PASSWORD, m_pSettings->GetPassword());
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCredentialsDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CCredentialsDlg::OnWiznext() 
{
	// TODO: Add your control notification handler code here
	try
	{
		Save();
		EndDialog( ID_WIZNEXT);	
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
}

void CCredentialsDlg::OnWizback() 
{
	// TODO: Add your control notification handler code here
	try
	{
		Save();
		EndDialog( ID_WIZBACK);	
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
}

void CCredentialsDlg::SetAgentSetting( CAgentSettings *pSettings)
{
	m_pSettings = pSettings;
}

BOOL CCredentialsDlg::Save()
{
	CString csLoginName, csPassword, csSshKeyFile;

	GetDlgItemText( IDC_EDIT_LOGIN, csLoginName);
	m_pSettings->SetLoginName( csLoginName);
	GetDlgItemText( IDC_EDIT_PASSWORD, csPassword);
	m_pSettings->SetPassword( csPassword);
	switch( m_pSettings->GetTargetOS())
	{
	case AGENT_OS_UNIX:
		AfxGetApp()->WriteProfileString( AGENT_SECTION, OPTION_UNIX_ROOT_LOGIN, csLoginName);
		GetDlgItemText( IDC_EDIT_SSH_KEY, csSshKeyFile);
		m_pSettings->SetSshKey( csSshKeyFile);
		AfxGetApp()->WriteProfileString( AGENT_SECTION, OPTION_UNIX_SSH_KEY, csSshKeyFile);
	case AGENT_OS_WINDOWS:
	default:
		AfxGetApp()->WriteProfileString( AGENT_SECTION, OPTION_WIN_ADMIN_LOGIN, csLoginName);
		break;
	}
	return TRUE;
}

void CCredentialsDlg::OnButtonExe() 
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;

	try
	{
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "PuTTY SSH Private key|*.ppk|All files|*.*||"));
		TCHAR			szInitialFolder[4*_MAX_PATH+1];
		CString			csMessage;

		// Get User Desktop path
		if (SHGetMalloc( &pIMalloc) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			return;
		}
		if (SHGetSpecialFolderLocation( m_hWnd, CSIDL_DESKTOP, &pMyIdList) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			return;
		}
		if (!SHGetPathFromIDList( pMyIdList, szInitialFolder))
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			pIMalloc->Free( pMyIdList);
			return;
		}
		pIMalloc->Free( pMyIdList);

		// Fill in the OPENFILENAME structure to support a template and hook.
		dlgOpenFile.m_ofn.lpstrInitialDir   = szInitialFolder;
		if (!csMessage.LoadString( IDS_SELECT_PUTTY_KEY))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle        = csMessage.GetBuffer( csMessage.GetLength());
		if (dlgOpenFile.DoModal() != IDOK)
			return;
		SetDlgItemText( IDC_EDIT_SSH_KEY, dlgOpenFile.GetPathName());
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		if (pMyIdList != NULL)
			pIMalloc->Free( pMyIdList);
		return;
	}
}
