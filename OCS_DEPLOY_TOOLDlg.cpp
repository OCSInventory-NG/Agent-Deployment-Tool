//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// OCS_DEPLOY_TOOLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MyHyperlink.h"
#include "OCS_DEPLOY_TOOL.h"
#include "OCS_DEPLOY_TOOLDlg.h"
#include "AgentSettings.h"
#include "SelectHostsDlg.h"
#include "WinAgentSettings.h"
#include "UnixAgentSettings.h"
#include "WindowsSetupDlg.h"
#include "UnixSetupDlg.h"
#include "CredentialsDlg.h"
#include "DeployingDlg.h"
#include "ToolsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CMyHyperLink	m_OcsLink;
	CMyHyperLink	m_PsExecLink;
	CMyHyperLink	m_PuttyLink;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_WEB_OCS, m_OcsLink);
	DDX_Control(pDX, IDC_WEB_PSEXEC, m_PsExecLink);
	DDX_Control(pDX, IDC_WEB_PUTTY, m_PuttyLink);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString csMessage, csStatus;

	csMessage.LoadString( IDS_OCS_DEPLOY_TOOL);
	csStatus.Format( _T ("%s\n\nBuilt %S %S"), csMessage, __DATE__, __TIME__);
	SetDlgItemText( IDC_STATUS, csStatus);
	m_OcsLink.SetLinkUrl( _T( "http://www.ocsinventory-ng.org"));
	m_PsExecLink.SetLinkUrl( _T( "http://technet.microsoft.com/en-us/sysinternals"));
	m_PuttyLink.SetLinkUrl( _T( "http://www.chiark.greenend.org.uk/~sgtatham/putty/"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// COCS_DEPLOY_TOOLDlg dialog

COCS_DEPLOY_TOOLDlg::COCS_DEPLOY_TOOLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COCS_DEPLOY_TOOLDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COCS_DEPLOY_TOOLDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COCS_DEPLOY_TOOLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COCS_DEPLOY_TOOLDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COCS_DEPLOY_TOOLDlg, CDialog)
	//{{AFX_MSG_MAP(COCS_DEPLOY_TOOLDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_WIN_DEPLOY, OnWinDeployButton)
	ON_BN_CLICKED(IDC_BUTTON_UNIX_DEPLOY, OnUnixDeployButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &COCS_DEPLOY_TOOLDlg::OnBnClickedButtonOption)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COCS_DEPLOY_TOOLDlg message handlers

BOOL COCS_DEPLOY_TOOLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CString	csMessage;
	
	csMessage.LoadString( IDS_OCS_DEPLOY_TOOL);
	SetDlgItemText( IDC_STATUS, csMessage);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COCS_DEPLOY_TOOLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COCS_DEPLOY_TOOLDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COCS_DEPLOY_TOOLDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void COCS_DEPLOY_TOOLDlg::OnWinDeployButton() 
{
	// TODO: Add your control notification handler code here
	try
	{
		CWinAgentSettings mySettings;
		CSelectHostsDlg cHostsDlg;
		CWindowsSetupDlg cSetupDlg;
		CCredentialsDlg cCredentialsDlg;
		CDeployingDlg cDeployingDlg;

OCS_WIN_AGENT_COMPUTER_LIST:
		// First, ask computer to install
		cHostsDlg.SetComputerList( &m_ComputerList, mySettings.GetTargetOS());
		if (cHostsDlg.DoModal() != ID_WIZNEXT)
			// Cancel
			return;

OCS_WIN_AGENT_PROPERTIES:
		// Next, ask install properties
		cSetupDlg.SetAgentSetting( &mySettings);
		switch (cSetupDlg.DoModal())
		{
		case ID_WIZBACK:
			// Back to selecting hosts
			goto OCS_WIN_AGENT_COMPUTER_LIST;
			break;
		case ID_WIZNEXT:
			break;
		default:
			// Error or cancel
			return;
		}

OCS_WIN_AGENT_CREDENTIALS:
		// Ask user credentials to connect to computers
		cCredentialsDlg.SetAgentSetting( &mySettings);
		switch (cCredentialsDlg.DoModal())
		{
		case ID_WIZBACK:
			// Back to selecting hosts
			goto OCS_WIN_AGENT_PROPERTIES;
			break;
		case ID_WIZNEXT:
			break;
		default:
			// Error or cancel
			return;
		}
		// Start deploying
		cDeployingDlg.SetComputerList( &m_ComputerList);
		cDeployingDlg.SetAgentSetting( &mySettings);
		switch (cDeployingDlg.DoModal())
		{
		case ID_WIZBACK:
			// Back to selecting hosts
			goto OCS_WIN_AGENT_CREDENTIALS;
			break;
		case ID_WIZNEXT:
			break;
		default:
			// Error or cancel
			return;
		}
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void COCS_DEPLOY_TOOLDlg::OnUnixDeployButton() 
{
	// TODO: Add your control notification handler code here
	try
	{
		CUnixAgentSettings mySettings;
		CSelectHostsDlg cHostsDlg;
		CUnixSetupDlg cSetupDlg;
		CCredentialsDlg cCredentialsDlg;
		CDeployingDlg cDeployingDlg;

OCS_UNIX_AGENT_COMPUTER_LIST:
		// First, ask computer to install
		cHostsDlg.SetComputerList( &m_ComputerList, mySettings.GetTargetOS());
		if (cHostsDlg.DoModal() != ID_WIZNEXT)
			// Cancel
			return;

OCS_UNIX_AGENT_PROPERTIES:
		// Next, ask install properties
		cSetupDlg.SetAgentSetting( &mySettings);
		switch (cSetupDlg.DoModal())
		{
		case ID_WIZBACK:
			// Back to selecting hosts
			goto OCS_UNIX_AGENT_COMPUTER_LIST;
			break;
		case ID_WIZNEXT:
			break;
		default:
			// Error or cancel
			return;
		}

OCS_UNIX_AGENT_CREDENTIALS:
		// Ask user credentials to connect to computers
		cCredentialsDlg.SetAgentSetting( &mySettings);
		switch (cCredentialsDlg.DoModal())
		{
		case ID_WIZBACK:
			// Back to selecting hosts
			goto OCS_UNIX_AGENT_PROPERTIES;
			break;
		case ID_WIZNEXT:
			break;
		default:
			// Error or cancel
			return;
		}
		// Start deploying
		cDeployingDlg.SetComputerList( &m_ComputerList);
		cDeployingDlg.SetAgentSetting( &mySettings);
		switch (cDeployingDlg.DoModal())
		{
		case ID_WIZBACK:
			// Back to selecting hosts
			goto OCS_UNIX_AGENT_CREDENTIALS;
			break;
		case ID_WIZNEXT:
			break;
		default:
			// Error or cancel
			return;
		}
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void COCS_DEPLOY_TOOLDlg::OnBnClickedButtonOption()
{
	// TODO: Add your control notification handler code here
	CToolsDialog cDlg;

	cDlg.DoModal();

}
