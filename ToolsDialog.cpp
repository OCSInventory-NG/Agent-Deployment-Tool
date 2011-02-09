// ToolsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MyHyperlink.h"
#include "ToolsDialog.h"
#include "FileVersion.h"


// CToolsDialog dialog

IMPLEMENT_DYNAMIC(CToolsDialog, CDialog)

CToolsDialog::CToolsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CToolsDialog::IDD, pParent)
{

}

CToolsDialog::~CToolsDialog()
{
}

void CToolsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEB_PSEXEC, m_PsExecLink);
	DDX_Control(pDX, IDC_WEB_PUTTY, m_PuttyLink);
	DDX_Control(pDX, IDC_WEB_PSCP, m_PscpLink);
}


BEGIN_MESSAGE_MAP(CToolsDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PSEXEC, &CToolsDialog::OnBnClickedButtonPsexec)
	ON_BN_CLICKED(IDC_BUTTON_PUTTY, &CToolsDialog::OnBnClickedButtonPutty)
	ON_BN_CLICKED(IDC_BUTTON_PSCP, &CToolsDialog::OnBnClickedButtonPscp)
END_MESSAGE_MAP()


// CToolsDialog message handlers

BOOL CToolsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString csMessage;
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
	m_PsExecLink.SetLinkUrl( _T( "http://technet.microsoft.com/en-us/sysinternals"));
	m_PuttyLink.SetLinkUrl( _T( "http://www.chiark.greenend.org.uk/~sgtatham/putty/"));
	m_PscpLink.SetLinkUrl( _T( "http://www.chiark.greenend.org.uk/~sgtatham/putty/"));
	// Set PsExec, PuTTY and Pscp path
	csMessage = AfxGetApp()->GetProfileString( SETTINGS_SECTION, SETTING_PSEXEC_PATH, _T( ""));
	SetDlgItemText( IDC_EDIT_PSEXEC, csMessage);
	csMessage = AfxGetApp()->GetProfileString( SETTINGS_SECTION, SETTING_PUTTY_PATH, _T( ""));
	SetDlgItemText( IDC_EDIT_PUTTY, csMessage);
	csMessage = AfxGetApp()->GetProfileString( SETTINGS_SECTION, SETTING_PSCP_PATH, _T( ""));
	SetDlgItemText( IDC_EDIT_PSCP, csMessage);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolsDialog::OnBnClickedButtonPsexec()
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;

	try
	{
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "Windows SysInternals PsExec|PsExec.exe|Executable files|*.exe||"));
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
		if (!csMessage.LoadString( IDS_SELECT_PSEXEC_FILE))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle        = csMessage.GetBuffer( csMessage.GetLength());
		if (dlgOpenFile.DoModal() != IDOK)
			return;
		SetDlgItemText( IDC_EDIT_PSEXEC, dlgOpenFile.GetPathName());
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

void CToolsDialog::OnBnClickedButtonPutty()
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;

	try
	{
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "PuTTY Telnet/SSH Client|PuTTY.exe|Executable files|*.exe||"));
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
		if (!csMessage.LoadString( IDS_SELECT_PUTTY_FILE))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle        = csMessage.GetBuffer( csMessage.GetLength());
		if (dlgOpenFile.DoModal() != IDOK)
			return;
		SetDlgItemText( IDC_EDIT_PUTTY, dlgOpenFile.GetPathName());
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

void CToolsDialog::OnBnClickedButtonPscp()
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;

	try
	{
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "PuTTY SCP Client|Pscp.exe|Executable files|*.exe||"));
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
		if (!csMessage.LoadString( IDS_SELECT_PSCP_FILE))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle        = csMessage.GetBuffer( csMessage.GetLength());
		if (dlgOpenFile.DoModal() != IDOK)
			return;
		SetDlgItemText( IDC_EDIT_PSCP, dlgOpenFile.GetPathName());
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

void CToolsDialog::OnOK()
{
	// TODO: Add your control notification handler code here
	CString csMessage;

	if (GetDlgItemText( IDC_EDIT_PSEXEC, csMessage) == 0)
	{
		// No data
		AfxMessageBox( IDS_ERROR_INVALID_VALUE, MB_ICONINFORMATION);
		GetDlgItem( IDC_EDIT_PSEXEC)->SetFocus();
		return;
	}
	AfxGetApp()->WriteProfileString( SETTINGS_SECTION, SETTING_PSEXEC_PATH, csMessage);
	if (GetDlgItemText( IDC_EDIT_PUTTY, csMessage) != 0)
		AfxGetApp()->WriteProfileString( SETTINGS_SECTION, SETTING_PUTTY_PATH, csMessage);
	if (GetDlgItemText( IDC_EDIT_PSCP, csMessage) != 0)
		AfxGetApp()->WriteProfileString( SETTINGS_SECTION, SETTING_PSCP_PATH, csMessage);
	CDialog::OnOK();
}

void CToolsDialog::OnCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}
