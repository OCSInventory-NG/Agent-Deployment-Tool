// UnixSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ocs_deploy_tool.h"
#include "AgentSettings.h"
#include "UnixAgentSettings.h"
#include "UnixSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUnixSetupDlg dialog


CUnixSetupDlg::CUnixSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnixSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnixSetupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUnixSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnixSetupDlg)
	DDX_Control(pDX, IDC_COMBO_ETC_DIRECTORY, m_EtcCombo);
	DDX_Control(pDX, IDC_LIST_FILES, m_ListFiles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUnixSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CUnixSetupDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EXE, OnButtonExe)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(ID_WIZNEXT, OnWiznext)
	ON_BN_CLICKED(ID_WIZBACK, OnWizback)
	ON_LBN_DBLCLK(IDC_LIST_FILES, OnDblclkListFiles)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_DIRECTORY, OnButtonChangeDirectory)
	ON_BN_CLICKED(IDC_CHECK_ETC_DIRECTORY, OnCheckEtcDirectory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnixSetupDlg message handlers

BOOL CUnixSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try
	{
		CString	csMessage, csSection, csNumber;
		BOOL bNoMoreValue = FALSE;
		int nIndex = 0;
		UINT uValue;
		
		csSection.Format( _T("%s\\%s"), SETTING_SECTION, OPTION_UNIX_AGENT_OTHER_FILES);
		csMessage.LoadString( IDS_OCS_DEPLOY_TOOL);
		SetDlgItemText( IDC_STATUS, csMessage);
		// Set Agent setup file
		csMessage = AfxGetApp()->GetProfileString( SETTING_SECTION, OPTION_UNIX_AGENT_SETUP_FILE, _T( ""));
		SetDlgItemText( IDC_EDIT_EXE, csMessage);
		// Set list of other files to copy
		while (!bNoMoreValue)
		{
			csNumber.Format( _T( "%d"), nIndex);
			csMessage = AfxGetApp()->GetProfileString( csSection, csNumber, _T( ""));
			if (!csMessage.IsEmpty())
				m_ListFiles.AddString( csMessage);
			else
				bNoMoreValue = TRUE;
			nIndex ++;
		}
		// Set OCS server address
		csMessage = AfxGetApp()->GetProfileString( SETTING_SECTION, OPTION_SERVER_ADDRESS, DEFAULT_SERVER_ADDRESS);
		SetDlgItemText( IDC_EDIT_SERVER_IP, csMessage);
		// Set TAG value
		csMessage = AfxGetApp()->GetProfileString( SETTING_SECTION, OPTION_UNIX_AGENT_TAG_VALUE, _T( ""));
		SetDlgItemText( IDC_EDIT_TAG_VALUE, csMessage);
		// Set if immediate launch of agent required
		uValue = AfxGetApp()->GetProfileInt( SETTING_SECTION, OPTION_UNIX_AGENT_LAUNCH_NOW, 0);
		CheckDlgButton( IDC_CHECK_LAUNCH_INVENTORY, uValue);
		// Set agent's var directory
		csMessage = AfxGetApp()->GetProfileString( SETTING_SECTION, OPTION_UNIX_AGENT_VAR_DIRECTORY, DEFAULT_UNIX_AGENT_VAR_DIRECTORY);
		if (csMessage.Compare( DEFAULT_UNIX_AGENT_VAR_DIRECTORY) != 0)
		{
			// Not default setup folder selected, so enable checkbox
			CheckDlgButton( IDC_BUTTON_CHANGE_DIRECTORY, TRUE);
			GetDlgItem( IDC_EDIT_INSTALL)->EnableWindow( TRUE);
		}
		SetDlgItemText( IDC_EDIT_INSTALL, m_pSettings->GetAgentSetupDirectory());
		// Set agent etc directory to use
		csMessage = AfxGetApp()->GetProfileString( SETTING_SECTION, OPTION_UNIX_AGENT_ETC_DIRECTORY, DEFAULT_UNIX_AGENT_ETC_DIRECTORY);
		if (csMessage.Compare( DEFAULT_UNIX_AGENT_ETC_DIRECTORY) != 0)
		{
			CheckDlgButton( IDC_CHECK_ETC_DIRECTORY, 1);
			m_EtcCombo.SelectString( 0, csMessage);
			m_EtcCombo.EnableWindow( TRUE);
		}
		else
		{
			CheckDlgButton( IDC_CHECK_ETC_DIRECTORY, 0);
			m_EtcCombo.SelectString( 0, csMessage);
			m_EtcCombo.EnableWindow( FALSE);
		}
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUnixSetupDlg::OnButtonExe() 
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;

	try
	{
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "OCS Inventory NG Agent for Unix Archive|Ocsinventory-Agent*.tar.gz|TAR GZ files|*.tar.gz||"));
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
		if (!csMessage.LoadString( IDS_SELECT_AGENT_FILE))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle        = csMessage.GetBuffer( csMessage.GetLength());
		if (dlgOpenFile.DoModal() != IDOK)
			return;
		SetDlgItemText( IDC_EDIT_EXE, dlgOpenFile.GetPathName());
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

void CUnixSetupDlg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;

	try
	{
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "All files|*.*||"));
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
		pMyIdList = NULL;

		// Fill in the OPENFILENAME structure to support a template and hook.
		dlgOpenFile.m_ofn.lpstrInitialDir   = szInitialFolder;
		if (!csMessage.LoadString( IDS_SELECT_OTHER_FILE))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle        = csMessage.GetBuffer( csMessage.GetLength());
		if (dlgOpenFile.DoModal() != IDOK)
			return;
		m_ListFiles.AddString( dlgOpenFile.GetPathName());
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

void CUnixSetupDlg::OnButtonRemove() 
{
	// TODO: Add your control notification handler code here
	try
	{
		int		nCurSel;

		if ((nCurSel = m_ListFiles.GetCurSel()) == LB_ERR )
			// No item selected
			return;
		m_ListFiles.DeleteString( nCurSel);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void CUnixSetupDlg::OnDblclkListFiles() 
{
	// TODO: Add your control notification handler code here
	OnButtonRemove();
}

void CUnixSetupDlg::OnWiznext() 
{
	// TODO: Add your control notification handler code here
	try
	{
		if (!Save( TRUE))
			return;
		EndDialog( ID_WIZNEXT);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void CUnixSetupDlg::OnWizback() 
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
		return;
	}
}

void CUnixSetupDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CUnixSetupDlg::Save( BOOL bDisplayError)
{
	int nIndex, nCount = 0;
	CString csMessage, csSection, csNumber;
	CStringList *pList;
	UINT uValue;

	csSection.Format( _T("%s\\%s"), SETTING_SECTION, OPTION_UNIX_AGENT_OTHER_FILES);
	// Get Agent setup file
	if (GetDlgItemText( IDC_EDIT_EXE, csMessage) == 0)
	{
		// No data
		bDisplayError && AfxMessageBox( IDS_ERROR_INVALID_VALUE, MB_ICONINFORMATION);
		GetDlgItem( IDC_EDIT_EXE)->SetFocus();
		return FALSE;
	}
	AfxGetApp()->WriteProfileString( SETTING_SECTION, OPTION_UNIX_AGENT_SETUP_FILE, csMessage);
	m_pSettings->SetAgentSetupFile( csMessage);
	// Remove registry stored files, before saving new ones
	RemoveOtherFilesFromRegistry();
	// Get list of other files to copy
	pList = m_pSettings->GetAgentOtherFiles();
	if (((nCount = m_ListFiles.GetCount()) == LB_ERR) || (nCount == 0))
	{
		// No items in List => NO FILES TO COPY
		pList->RemoveAll();
	}
	else
	{
		// Empty current list
		pList->RemoveAll();
		// Fill in file list with each file from listbox
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			m_ListFiles.GetText( nIndex, csMessage);
			pList->AddTail( csMessage);
			csNumber.Format( _T( "%d"), nIndex);
			AfxGetApp()->WriteProfileString( csSection, csNumber, csMessage);
		}
	}
	// Get OCS server address
	if (GetDlgItemText( IDC_EDIT_SERVER_IP, csMessage) == 0)
	{
		// No data
		bDisplayError && AfxMessageBox( IDS_ERROR_INVALID_VALUE, MB_ICONINFORMATION);
		GetDlgItem( IDC_EDIT_SERVER_IP)->SetFocus();
		return FALSE;
	}
	AfxGetApp()->WriteProfileString( SETTING_SECTION, OPTION_SERVER_ADDRESS, csMessage);
	m_pSettings->SetServerAddress( csMessage);
	// Get TAG value
	GetDlgItemText( IDC_EDIT_TAG_VALUE, csMessage);
	AfxGetApp()->WriteProfileString( SETTING_SECTION, OPTION_UNIX_AGENT_TAG_VALUE, csMessage);
	m_pSettings->SetTagValue( csMessage);
	// Get if immediate launch of agent required
	uValue = IsDlgButtonChecked( IDC_CHECK_LAUNCH_INVENTORY);
	AfxGetApp()->WriteProfileInt( SETTING_SECTION, OPTION_UNIX_AGENT_LAUNCH_NOW, uValue);
	m_pSettings->SetLaunchNowRequired( uValue);
	// Get agent's var directory
	if (GetDlgItemText( IDC_EDIT_INSTALL, csMessage) == 0)
	{
		// No data
		bDisplayError && AfxMessageBox( IDS_ERROR_INVALID_VALUE, MB_ICONINFORMATION);
		GetDlgItem( IDC_EDIT_INSTALL)->SetFocus();
		return FALSE;
	}
	AfxGetApp()->WriteProfileString( SETTING_SECTION, OPTION_UNIX_AGENT_VAR_DIRECTORY, csMessage);
	m_pSettings->SetAgentSetupDirectory( csMessage);
	// Get agent's etc directory
	if (GetDlgItemText( IDC_COMBO_ETC_DIRECTORY, csMessage) == 0)
	{
		// No data
		bDisplayError && AfxMessageBox( IDS_ERROR_INVALID_VALUE, MB_ICONINFORMATION);
		GetDlgItem( IDC_COMBO_ETC_DIRECTORY)->SetFocus();
		return FALSE;
	}
	AfxGetApp()->WriteProfileString( SETTING_SECTION, OPTION_UNIX_AGENT_ETC_DIRECTORY, csMessage);
	m_pSettings->SetAgentEtcDirectory( csMessage);
	return TRUE;
}

void CUnixSetupDlg::SetAgentSetting( CAgentSettings *pSettings)
{
	m_pSettings = pSettings;
}

void CUnixSetupDlg::OnButtonChangeDirectory() 
{
	// TODO: Add your control notification handler code here
	try
	{
		if (IsDlgButtonChecked( IDC_BUTTON_CHANGE_DIRECTORY))
			GetDlgItem( IDC_EDIT_INSTALL)->EnableWindow( TRUE);
		else
		{
			GetDlgItem( IDC_EDIT_INSTALL)->EnableWindow( FALSE);
			SetDlgItemText( IDC_EDIT_INSTALL, DEFAULT_UNIX_AGENT_VAR_DIRECTORY);
		}
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

BOOL CUnixSetupDlg::RemoveOtherFilesFromRegistry()
{
	HKEY	hKey;
	CString csKey, csValue;
	DWORD	dwErr, dwLength;
	UINT	uIndex = 0;

	csKey.Format( _T( "SOFTWARE\\OCS_DEPLOY_TOOL\\%s\\%s"), SETTING_SECTION, OPTION_UNIX_AGENT_OTHER_FILES);
	if ((dwErr = RegOpenKeyEx( HKEY_CURRENT_USER, csKey, 0, KEY_READ|KEY_WRITE, &hKey)) != ERROR_SUCCESS)
		return FALSE;
	while (dwErr == ERROR_SUCCESS)
	{
		csValue.Format( _T( "%u"), uIndex);
		// Check if value exists
		dwErr = REG_SZ;
		dwLength = 0;
		if ((dwErr = RegQueryValueEx( hKey, csValue, NULL, &dwErr, NULL, &dwLength)) == ERROR_SUCCESS)
			// Key exists, delete it
			RegDeleteValue( hKey, csValue);
		// Continue
		uIndex ++;
	}
	RegCloseKey( hKey);
	return TRUE;
}

void CUnixSetupDlg::OnCheckEtcDirectory() 
{
	// TODO: Add your control notification handler code here
	try
	{
		if (IsDlgButtonChecked( IDC_CHECK_ETC_DIRECTORY))
			m_EtcCombo.EnableWindow( TRUE);
		else
		{
			m_EtcCombo.SelectString( 0, DEFAULT_UNIX_AGENT_ETC_DIRECTORY);
			m_EtcCombo.EnableWindow( FALSE);
		}
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}
