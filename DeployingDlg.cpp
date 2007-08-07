//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// DeployingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ocs_deploy_tool.h"
#include "AgentSettings.h"
#include "WinAgentSettings.h"
#include "UnixAgentSettings.h"
#include "LauncherThreadParam.h"
#include "WorkerThreadParam.h"
#include "DeployingDlg.h"

#include <stdio.h>
#include <winsvc.h>
#include <lmcons.h>
#include <lmshare.h>
#include <lmerr.h>
#include <lmapibuf.h>
#include <lmaccess.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Launcher thread to setup on computers list
UINT InstallComputerList( LPVOID pParam);
// Worker thread to setup on one computer
UINT InstallComputer( LPVOID pParam);
// Install Windows agent on a remote computer
BOOL WindowsRemoteInstall( CWorkerThreadParam *pParam);
// Allow retreiving from system error message associated to an error code
CString LookupError( DWORD Err);
// Get filename from a file path
LPCTSTR GetFileName( CString csFile);
// Get folder name from a file path
LPCTSTR GetFolderName( CString csFile);
// Execute command lpstrCommand on remote Windows host lpstrComputer
// Return ERROR_SUCCESS if OK
DWORD WinRemoteExec( LPCTSTR lpstrComputer, LPCTSTR lpstrCommand);
// Copy source file to remote Unix host lpstrComputer using SCP
// Return ERROR_SUCCESS if OK
DWORD CopyToUnix( LPCTSTR lpstrComputer, CAgentSettings *pSettings, LPCTSTR lpstrLocalFile, LPCTSTR lpstrRemoteFile, BOOL bHide = TRUE);
// Copy source file from remote Unix host lpstrComputer using SCP
// Return ERROR_SUCCESS if OK
DWORD CopyFromUnix( LPCTSTR lpstrComputer, CAgentSettings *pSettings, LPCTSTR lpstrLocalFile, LPCTSTR lpstrRemoteFile, BOOL bHide = TRUE);
// Execute command lpstrCommand on remote Unix host lpstrComputer using SSH
// Return ERROR_SUCCESS if OK
DWORD UnixRemoteExec( LPCTSTR lpstrComputer, CAgentSettings *pSettings, LPCTSTR lpstrCommand, BOOL bHide = TRUE);
// Install Unix agent on a remote computer
BOOL UnixRemoteInstall( CWorkerThreadParam *pParam);

/////////////////////////////////////////////////////////////////////////////
// CDeployingDlg dialog


CDeployingDlg::CDeployingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeployingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeployingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDeployingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeployingDlg)
	DDX_Control(pDX, IDC_SLIDER_THREADS, m_SliderThreads);
	DDX_Control(pDX, IDC_LIST_COMPUTERS, m_ListBox);
	//}}AFX_DATA_MAP
}


LRESULT CDeployingDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((message == WM_SETTEXT)	&& (wParam == IDC_MESSAGE_HANDLER_STATUS))
		// Add message to status 
		return OnMessageHandlerStatus( lParam);
	if ((message == WM_SETTEXT)	&& (wParam == IDC_MESSAGE_HANDLER_LISTBOX))
		// Add message to listbox 
		return OnMessageHandlerListbox( lParam);
	if ((message == WM_SETTEXT)	&& (wParam == IDC_MESSAGE_GET_MAX_THREADS))
		// Add message to listbox 
		return OnMessageHandlerGetMaxThreads();
	return CDialog::DefWindowProc(message, wParam, lParam);
}

BEGIN_MESSAGE_MAP(CDeployingDlg, CDialog)
	//{{AFX_MSG_MAP(CDeployingDlg)
	ON_BN_CLICKED(ID_WIZBACK, OnWizback)
	ON_BN_CLICKED(ID_WIZFINISH, OnWizfinish)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_THREADS, OnReleasedcaptureSliderThreads)
	ON_BN_CLICKED(IDC_STOP_THREADS, OnStopThreads)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LOG, OnButtonSaveLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeployingDlg message handlers

BOOL CDeployingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try
	{
		CString	csMessage;
		RECT Rect;
		DWORD dwWinStyle;
		
		csMessage.LoadString( IDS_OCS_DEPLOY_TOOL);
		SetDlgItemText( IDC_STATUS, csMessage);
		csMessage.LoadString( IDS_STATUS_START_DEPLOYMENT);
		SetDlgItemText( IDC_STATUS_THREADS, csMessage);
		// Enable horizontal scrolling
		dwWinStyle = GetDlgItem( IDC_LIST_COMPUTERS)->GetStyle();
		GetDlgItem( IDC_LIST_COMPUTERS)->GetClientRect ( &Rect);
		if (dwWinStyle & WS_HSCROLL)
			Rect.bottom += GetSystemMetrics( SM_CYHSCROLL);
		if (dwWinStyle & WS_VSCROLL)
			Rect.right  += GetSystemMetrics( SM_CXVSCROLL);
		SendDlgItemMessage( IDC_LIST_COMPUTERS, LB_SETHORIZONTALEXTENT, (WPARAM) (3*(Rect.right - Rect.left)), 0L);
		// Threads slider between 1 and MAX_SIMULTANEOUS_CONNECTIONS, by default 1
		m_uMaxThreads = AfxGetApp()->GetProfileInt( SETTING_SECTION, OPTIONS_MAX_SIMULTANEOUS_CONNECTIONS, DEFAULT_MAX_SIMULTANEOUS_CONNECTIONS);
		m_SliderThreads.SetRange( 1, m_uMaxThreads, TRUE);
		m_SliderThreads.SetPos( 1);
		csMessage.Format( _T( "%d simultaneous connection(s) (max. %u)"), 1, m_uMaxThreads);
		SetDlgItemText( IDC_MAX_THREADS, csMessage);
		m_pThreadLauncher = NULL;
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeployingDlg::OnWizback() 
{
	// TODO: Add your control notification handler code here
	EndDialog( ID_WIZBACK);
}

void CDeployingDlg::OnWizfinish() 
{
	// TODO: Add your control notification handler code here
	try
	{
		DWORD		dwErr;
		CWaitCursor cWait;
		CString		csMessage, csTotal;
		static CLauncherThreadParam myParam;

		csTotal.Format( _T( "%d"), m_pComputerList->GetCount());
		csMessage.FormatMessage( IDS_STATUS_RUNNING_DEPLOYMENT, _T( "0"), csTotal);
		SetDlgItemText( IDC_STATUS_THREADS, csMessage);
		// Disabling buttons
		GetDlgItem( ID_WIZFINISH)->EnableWindow( FALSE);
		GetDlgItem( ID_WIZBACK)->EnableWindow( FALSE);
		GetDlgItem( IDCANCEL)->EnableWindow( FALSE);
		GetDlgItem( IDC_BUTTON_SAVE_LOG)->EnableWindow( FALSE);
		// Enable stop button
		GetDlgItem( IDC_STOP_THREADS)->EnableWindow( TRUE);
		// Remove ListBox content
		m_ListBox.ResetContent();
		// Empty failed computers list
		m_FailedList.RemoveAll();
		// Ensure no launcher thread is running
		if (m_pThreadLauncher != NULL)
		{
			// Previous thread not cleaned
			GetExitCodeThread( m_pThreadLauncher->m_hThread, &dwErr);
			if(dwErr == STILL_ACTIVE)
			{
				// Thread already working, stop it !
				m_pThreadLauncher->SuspendThread();
			}
			// Clean thread
			delete m_pThreadLauncher;
			m_pThreadLauncher = NULL;
		}
		// Launch new thread to setup computer list
		myParam.SetParam( m_hWnd, m_pSettings, m_pComputerList, &m_FailedList);
		m_pThreadLauncher = AfxBeginThread((AFX_THREADPROC)InstallComputerList, &myParam, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		m_pThreadLauncher->m_bAutoDelete = FALSE;
		m_pThreadLauncher->ResumeThread();
		// Launch timer to wait for thread end
		SetTimer( 1, 500, NULL);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
}


void CDeployingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	OnStopThreads();
	m_FailedList.RemoveAll();
	CDialog::OnCancel();
}

void CDeployingDlg::OnReleasedcaptureSliderThreads(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int		nPos;
	CString	csThreads;

	nPos = m_SliderThreads.GetPos();
	csThreads.Format( _T( "%d simultaneous connection(s) (max. %u)"), nPos, m_uMaxThreads);
	SetDlgItemText( IDC_MAX_THREADS, csThreads);
	*pResult = 0;
}

void CDeployingDlg::OnStopThreads() 
{
	// TODO: Add your control notification handler code here
	try
	{
		DWORD		dwErr;
		CString		csMessage;
		CWaitCursor cWait;

		if (m_pThreadLauncher == NULL)
			// No launcher thread
			return;
		GetExitCodeThread( m_pThreadLauncher->m_hThread, &dwErr);
		if(dwErr == STILL_ACTIVE)
		{
			// Thread already working, stop it !
			m_pThreadLauncher->SuspendThread();
		}
		// Clean thread
		delete m_pThreadLauncher;
		m_pThreadLauncher = NULL;
		csMessage.LoadString( IDS_STATUS_STOP_DEPLOYMENT);
		SetDlgItemText( IDC_STATUS_THREADS, csMessage);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
}

void CDeployingDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	try
	{
		DWORD	dwErr;
		CString csMessage;

		if (m_pThreadLauncher != NULL)
			GetExitCodeThread( m_pThreadLauncher->m_hThread, &dwErr);
		if ((m_pThreadLauncher == NULL) || (dwErr != STILL_ACTIVE))
		{
			if (m_pThreadLauncher != NULL)
			{
				// Thread terminated but not deleted, clean it
				delete m_pThreadLauncher;
				m_pThreadLauncher = NULL;
			}
			// Disable stop button
			GetDlgItem( IDC_STOP_THREADS)->EnableWindow( FALSE);
			((CButton *) GetDlgItem(IDC_STOP_THREADS))->SetButtonStyle(BS_PUSHBUTTON,TRUE);
			// Enabling buttons and Set "Close" default button
			GetDlgItem( ID_WIZFINISH)->EnableWindow( TRUE);
			((CButton *) GetDlgItem(ID_WIZFINISH))->SetButtonStyle(BS_PUSHBUTTON,TRUE);
			GetDlgItem( ID_WIZBACK)->EnableWindow( TRUE);
			((CButton *) GetDlgItem(ID_WIZBACK))->SetButtonStyle(BS_PUSHBUTTON,TRUE);
			GetDlgItem( IDCANCEL)->EnableWindow( TRUE);
			((CButton *) GetDlgItem(IDC_BUTTON_SAVE_LOG))->SetButtonStyle(BS_PUSHBUTTON,TRUE);
			GetDlgItem( IDC_BUTTON_SAVE_LOG)->EnableWindow( TRUE);
			((CButton *) GetDlgItem(IDCANCEL))->SetButtonStyle(BS_DEFPUSHBUTTON,TRUE);
			GetDlgItem( IDCANCEL)->SetFocus();
			KillTimer( 1);
		}
		CDialog::OnTimer(nIDEvent);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
}

LRESULT CDeployingDlg::OnMessageHandlerStatus(LPARAM lParam)
{
	try
	{
		SetDlgItemText( IDC_STATUS_THREADS, (LPCTSTR) lParam);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
	return 0;
}

LRESULT CDeployingDlg::OnMessageHandlerListbox(LPARAM lParam)
{
	try
	{
		int nIndex;

		if (((nIndex = m_ListBox.AddString( (LPCTSTR) lParam)) != LB_ERR) && (nIndex != LB_ERRSPACE))
			m_ListBox.SetCurSel( nIndex);
		m_ListBox.RedrawWindow();
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
	return 0;
}

LRESULT CDeployingDlg::OnMessageHandlerGetMaxThreads()
{
	static LRESULT lResult;

	lResult = m_SliderThreads.GetPos();
	return lResult;
}

void CDeployingDlg::OnButtonSaveLog() 
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;
	CStdioFile		myFile;		// To write CSV text file

	try
	{
		CString			csLine;	
		// To select where to store failed computer list CSV file
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T( "Log Files|*.log|All files|*.*||"));
		TCHAR			szInitialFolder[4*_MAX_PATH+1];
		int				nIndex = 0;	// Number of computer launched
		
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
		if (!csLine.LoadString( IDS_SELECT_LOG_FILE))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle  = csLine.GetBuffer( csLine.GetLength());
		dlgOpenFile.m_ofn.lpstrDefExt = _T( "log");
		if (dlgOpenFile.DoModal() != IDOK)
		{
			// Cancel
			return;
		}
		if (!myFile.Open( dlgOpenFile.GetPathName(), CFile::modeWrite|CFile::modeCreate|CFile::typeText))
		{
			csLine.FormatMessage( IDS_ERROR_OPENING_FILE, dlgOpenFile.GetPathName());
			AfxMessageBox( csLine, MB_ICONSTOP|MB_OK);
			return;
		}
		// Write header
		myFile.WriteString( _T("======================================================================\n"));
		csLine.LoadString( IDS_OCS_DEPLOY_TOOL);
		myFile.WriteString( csLine);
		csLine.Format( _T( "\nStarted on %s\n"), COleDateTime::GetCurrentTime().Format( _T( "%A %d %B %Y %H:%M:%S")));
		myFile.WriteString( csLine);
		myFile.WriteString( _T("======================================================================\n"));
		// Write log line 
		for (nIndex=0; nIndex<m_ListBox.GetCount(); nIndex++)
		{
			m_ListBox.GetText( nIndex, csLine);
			myFile.WriteString( csLine);
			myFile.WriteString( _T( "\n"));
		}
		myFile.Close();
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		if (pMyIdList != NULL)
			pIMalloc->Free( pMyIdList);
		myFile.Abort();
	}
}

void CDeployingDlg::SetComputerList(CStringList *pList)
{
	m_pComputerList = pList;
}

void CDeployingDlg::SetAgentSetting( CAgentSettings *pSettings)
{
	m_pSettings = pSettings;
}

UINT InstallComputerList( LPVOID pParam)
{
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;
	CStdioFile		myFile;		// To write CSV text file
	CObArray		threadHandles; // List of running threads

	try
	{
		HWND			hWnd = ((CLauncherThreadParam *)pParam)->GetHwnd();
		CAgentSettings	*pSettings = ((CLauncherThreadParam *)pParam)->GetSettings();
		CStringList		*pComputers = ((CLauncherThreadParam *)pParam)->GetComputersList();
		CStringList		*pFailed = ((CLauncherThreadParam *)pParam)->GetFailedList();
		CString			csComputer,	// Computer to setup
						csSuccess,	// String to format number of sucess or failure
						csFailure;	
		POSITION		pos;		// Position into string list
		// To select where to store failed computer list CSV file
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T( "CSV Files|*.csv|All files|*.*||"));
		TCHAR			szInitialFolder[4*_MAX_PATH+1];
		int				nIndex = 0,	// Number of computer launched
						nThread;	// Number of threads
		DWORD			dwErr;
		CWorkerThreadParam myParam;	// Parameters to pass to working threads
		CWinThread*		pThread;	// One working thread


		// Launch setup for each computer
		pos = pComputers->GetHeadPosition();
		while (pos)
		{
			// Ensure not too many threads already running
			nThread = ::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_GET_MAX_THREADS, 0);
			while (threadHandles.GetSize() >= nThread)
			{
				// Too many threads lanuched, wait a while
				Sleep( 500);
				// Cleaning dead threads if needed
				nThread = 0;
				while (nThread < threadHandles.GetSize())
				{
					// Get thread status
					pThread = (CWinThread*)threadHandles.GetAt( nThread);
					GetExitCodeThread( pThread->m_hThread, &dwErr );
					if( dwErr != STILL_ACTIVE )
					{
						// Clean thread
						threadHandles.RemoveAt( nThread);
						delete pThread;
						nThread --;
					}
					nThread ++;
				}
				nThread = ::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_GET_MAX_THREADS, 0);
			}
			// Launch new thread
			csComputer = pComputers->GetNext( pos);
			myParam.SetParam( hWnd, pSettings, pFailed, csComputer);
			pThread = AfxBeginThread((AFX_THREADPROC)InstallComputer, &myParam, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			threadHandles.Add( pThread);
			pThread->m_bAutoDelete = FALSE;
			pThread->ResumeThread();
			// Set status text on dialogbox
			nIndex ++;
			csSuccess.Format( _T( "%d"), nIndex);
			csFailure.Format( _T( "%d"), pComputers->GetCount());
			csComputer.FormatMessage( IDS_STATUS_RUNNING_DEPLOYMENT, csSuccess, csFailure);
			::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_STATUS, (LPARAM) LPCTSTR( csComputer));
			// Wait a while before launching next script
			Sleep( 1000);
		}
		// Waiting for threads to terminate
		csComputer.LoadString( IDS_STATUS_STOP_DEPLOYMENT);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_STATUS, (LPARAM) LPCTSTR( csComputer));
		while (threadHandles.GetSize() > 0)
		{
			Sleep( 500);
			// Cleaning dead threads if needed
			nThread = 0;
			while (nThread < threadHandles.GetSize())
			{
				// Get thread status
				pThread = (CWinThread*)threadHandles.GetAt( nThread);
				GetExitCodeThread( pThread->m_hThread, &dwErr );
				if( dwErr != STILL_ACTIVE )
				{
					// Clean thread
					threadHandles.RemoveAt( nThread);
					delete pThread;
					nThread --;
				}
				nThread ++;
			}
		}
		// Show number of success and failure
		csComputer = _T( " ");
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csComputer));
		csSuccess.Format( _T( "%u"), pComputers->GetCount() - pFailed->GetCount());
		csFailure.Format( _T( "%u"), pFailed->GetCount());
		csComputer.FormatMessage( IDS_STATUS_DEPLOYMENT_FINISHED, csSuccess, csFailure);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csComputer));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_STATUS, (LPARAM) LPCTSTR( csComputer));
		AfxMessageBox( csComputer, MB_ICONINFORMATION|MB_OK);
		if (pFailed->GetCount() == 0)
			// No error
			return 0;
		// Ask to export list of failed computers into CSV
		if (AfxMessageBox( IDS_STATUS_EXPORT_FAILED_HOSTS, MB_ICONQUESTION|MB_YESNO) == IDNO)
		{
			// Don't save
			return 0;
		}
		// Get User Desktop path
		if (SHGetMalloc( &pIMalloc) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			return 0;
		}
		if (SHGetSpecialFolderLocation( hWnd, CSIDL_DESKTOP, &pMyIdList) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			return 0;
		}
		if (!SHGetPathFromIDList( pMyIdList, szInitialFolder))
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			pIMalloc->Free( pMyIdList);
			return 0;
		}
		pIMalloc->Free( pMyIdList);
		pMyIdList = NULL;
		// Fill in the OPENFILENAME structure to support a template and hook.
		dlgOpenFile.m_ofn.lpstrInitialDir   = szInitialFolder;
		dlgOpenFile.m_ofn.lpstrDefExt		= _T( "csv");
		if (!csComputer.LoadString( IDS_SELECT_CSV_FILE))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle  = csComputer.GetBuffer( csComputer.GetLength());
		if (dlgOpenFile.DoModal() != IDOK)
		{
			// Cancel
			return 0;
		}
		if (!myFile.Open( dlgOpenFile.GetPathName(), CFile::modeWrite|CFile::modeCreate|CFile::typeText))
		{
			csComputer.FormatMessage( IDS_ERROR_OPENING_FILE, dlgOpenFile.GetPathName());
			AfxMessageBox( csComputer, MB_ICONSTOP|MB_OK);
			return 0;
		}
		// Write first header line
		myFile.WriteString( _T( "Computer address\n"));
		// Write next line 
		pos = pFailed->GetHeadPosition();
		while (pos)
		{
			myFile.WriteString( pFailed->GetNext( pos));
			myFile.WriteString( _T( "\n"));
		}
		myFile.Close();
		return 0;
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		threadHandles.RemoveAll();
		if (pMyIdList != NULL)
			pIMalloc->Free( pMyIdList);
		myFile.Abort();
		return 1;
	}
}

UINT InstallComputer( LPVOID pParam)
{
	try
	{
		CWorkerThreadParam *pThreadParam = (CWorkerThreadParam *)pParam;

		switch (pThreadParam->GetSettings()->GetTargetOS())
		{
		case AGENT_OS_WINDOWS:
			// Settings for Windows
			return WindowsRemoteInstall( pThreadParam);
		case AGENT_OS_UNIX:
			// Settings for Unix
			return UnixRemoteInstall( pThreadParam);
		default:
			// Unknown target OS
			return FALSE;
		}

	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return FALSE;
	}
}

BOOL WindowsRemoteInstall( CWorkerThreadParam *pParam)
{
	CString			csComputer = pParam->GetComputer(), // Computer to setup
					csTemp,			// Temporary buffer
					csDestDir,		// Destination directory in UNC (\\computer\drive$\folder)
					csSourceFile,	// Source file to copy
					csTargetFile;	// Target destination file
	HWND			hWnd = pParam->GetHwnd();
	CAgentSettings  *pSettings = pParam->GetSettings();
	CStringList		*pFailedList = pParam->GetFailedList();
	DWORD			dwErr,
					dwLength = 0;	// Length of retreived registry buffer
	NETRESOURCE		nr;				// Network ressource handle
	CStringList		*pList;			// Computer list to setup
	POSITION		pos;			// Position into string list
	CStdioFile		myFile;			// To read/write text files
	BOOL			bSetupSuccess = FALSE; // Is agent setup successful ?
	HKEY			hKeyHKLM,		// Remote HKLM registry key
					hKey;			// Registry key to read Program Files directory
	TCHAR			szProgramFiles[4*_MAX_PATH+1]; // String storing Program Files directory

	///////////////////////////////////////////////////////////
	// First, test connection to remote host on RPC port
	csTemp.FormatMessage( IDS_STATUS_TESTING_REMOTE_HOST, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	if (!TestConnection( csComputer, AfxGetApp()->GetProfileInt( SETTING_SECTION, OPTION_TEST_PORT_WINDOWS, DEFAULT_TEST_PORT_WINDOWS)))
	{
		csTemp.FormatMessage( IDS_ERROR_TESTING_REMOTE_HOST, csComputer);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		pFailedList->AddTail( csComputer);
		return FALSE;
	}
	///////////////////////////////////////////////////////////
	// Establish connection to remote host (using username/pwd)
	csTemp.FormatMessage( IDS_STATUS_CONNECTING_REMOTE_HOST, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	csTemp.Format( _T( "\\\\%s\\ADMIN$"), csComputer);
	nr.dwType = RESOURCETYPE_ANY;
	nr.lpLocalName = NULL;
	nr.lpRemoteName = (LPTSTR)LPCTSTR( csTemp);
	nr.lpProvider = NULL;
	if (!pSettings->IsLoginNameAvailable())
		// Using current logged in user credentials
		dwErr = WNetAddConnection2( &nr, NULL, NULL, FALSE);
	else
		// Using provided credentials
		dwErr = WNetAddConnection2( &nr, pSettings->GetPassword(), pSettings->GetLoginName(), FALSE);
	if (dwErr != NO_ERROR)
	{
		csTemp.FormatMessage( IDS_ERROR_CONNECTING_REMOTE_HOST, csComputer, LookupError( dwErr));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		pFailedList->AddTail( csComputer);
		return FALSE;
	}
	// Connect to registry to get system Program Files directory
	csTemp.FormatMessage( IDS_STATUS_RETREIVING_PROGRAM_FILES_REGISTRY, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	csTemp.Format( _T( "\\\\%s"), csComputer);
	szProgramFiles[0] = 0;
	if ((dwErr = RegConnectRegistry( csTemp, HKEY_LOCAL_MACHINE, &hKeyHKLM)) == ERROR_SUCCESS)
	{
		if ((dwErr = RegOpenKeyEx( hKeyHKLM, WIN_DEFAULT_PROGRAM_FILES_KEY, 0, KEY_READ, &hKey)) == ERROR_SUCCESS)
		{
			dwErr = REG_SZ;
			dwLength = 4*_MAX_PATH;
			if ((dwErr = RegQueryValueEx( hKey, WIN_DEFAULT_PROGRAM_FILES_VALUE, NULL, &dwErr, (LPBYTE)szProgramFiles, &dwLength)) != ERROR_SUCCESS)
			{
				csTemp.FormatMessage( IDS_ERROR_REMOTE_REGISTRY_ACCESS, csComputer, LookupError( dwErr));
				::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
				RegCloseKey( hKey);
				RegCloseKey( hKeyHKLM);
				dwLength = 0;
			}
			szProgramFiles[dwLength] = 0;
			RegCloseKey( hKey);
		}
		else
		{
			csTemp.FormatMessage( IDS_ERROR_REMOTE_REGISTRY_ACCESS, csComputer, LookupError( dwErr));
			::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
			RegCloseKey( hKeyHKLM);
		}
		RegCloseKey( hKeyHKLM);
	}
	else
	{
		csTemp.FormatMessage( IDS_ERROR_REMOTE_REGISTRY_ACCESS, csComputer, LookupError( dwErr));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	}
	if (dwLength == 0)
	{
		// Unable to get Program Files path from remote registry, try using RemCom
		csTemp.FormatMessage( IDS_STATUS_RETREIVING_PROGRAM_FILES_REMCOM, csComputer);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		// First create command
		csTemp.Format( _T( "echo %%PROGRAMFILES%%> %%WINDIR%%\\%s_pf.log"), csComputer);
		// Launch RemCom.exe to start agent setup on remote computer
		if ((dwErr = WinRemoteExec( csComputer, csTemp)) != ERROR_SUCCESS)
		{
			csTemp.FormatMessage( IDS_ERROR_RETREIVING_REMOTE_PROGRAM_FILES, csComputer, LookupError( dwErr));
			::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		}
		else
		{
			///////////////////////////////////////////////////////////
			// Try to get result
			csTargetFile.Format( _T( "\\\\%s\\ADMIN$\\%s_pf.log"), csComputer, csComputer);
			if (!myFile.Open( csTargetFile, CFile::modeRead))
			{
				// Unable to open result file
				csTemp.FormatMessage( IDS_ERROR_RETREIVING_REMOTE_PROGRAM_FILES, csComputer, LookupError( GetLastError()));
				::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
			}
			else
			{
				// Read result file
				myFile.ReadString( csTemp);
				myFile.Close();
				CFile::Remove( csTargetFile);
				csTemp.TrimRight( _T( " "));
				_tcsncpy( szProgramFiles, csTemp, 4*_MAX_PATH);
				dwLength = csTemp.GetLength();
			}
		}
	}

	///////////////////////////////////////////////////////////
	// Create directories
	csTemp.FormatMessage( IDS_STATUS_CREATING_DIRECTORY, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	// Replace %ProgramFiles% by remote system program files if needed
	csTemp = pSettings->GetAgentSetupDirectory();
	csTemp.Replace( _T( "PROGRAM_FILES"), szProgramFiles);
	pSettings->SetAgentSetupDirectory( csTemp);
	// Use administrative shares
	csDestDir.Format( _T( "\\\\%s\\%s"), csComputer, csTemp);
	csDestDir.Replace( _T(":\\"), _T( "$\\"));
	if (!CreateDirectory( csDestDir, NULL) && GetLastError()!=ERROR_ALREADY_EXISTS)
	{
		// Unable to create directory
		csTemp.FormatMessage( IDS_ERROR_CREATING_DIRECTORY, csComputer, LookupError( GetLastError()));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		goto WIN_COMPUTER_DISCONNECT;
	}
	if (csDestDir.Right( 1) != '\\')
		csDestDir += '\\';
	///////////////////////////////////////////////////////////
	// Copy files
	csTemp.FormatMessage( IDS_STATUS_COPYING_FILES, csComputer, csDestDir);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	// First, copy agent setup file
	csTargetFile.Format( _T( "%s\\%s"), csDestDir, GetFileName( pSettings->GetAgentSetupFile()));
	if (!CopyFile( pSettings->GetAgentSetupFile(), csTargetFile, FALSE))
	{
		// Unable to copy file
		csTemp.FormatMessage( IDS_ERROR_COPYING_FILES, csComputer, LookupError( GetLastError()));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		goto WIN_COMPUTER_DISCONNECT;
	}
	// Next, copy additional files
	pList = pSettings->GetAgentOtherFiles();
	pos = pList->GetHeadPosition();
	while (pos)
	{
		csSourceFile = pList->GetNext( pos);
		csTargetFile.Format( _T( "%s\\%s"), csDestDir, GetFileName( csSourceFile));
		if (!CopyFile( csSourceFile, csTargetFile, FALSE))
		{
			// Unable to copy file
			csTemp.FormatMessage( IDS_ERROR_COPYING_FILES, csComputer, LookupError( GetLastError()));
			::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
			goto WIN_COMPUTER_DISCONNECT;
		}
	}
	///////////////////////////////////////////////////////////
	// Launch agent setup
	csTemp.FormatMessage( IDS_STATUS_LAUNCHING_SETUP, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	// First create command
	csTemp.Format( _T( "\"%s\\%s\" /S %s /SERVER:%s /PNUM:%u %s %s %s /D=%s\n"),
					pSettings->GetAgentSetupDirectory(), 
					GetFileName( pSettings->GetAgentSetupFile()), // remote agent setup file
					pSettings->IsLaunchNowRequired() ? _T( "/NOW") : _T( ""), // enable immediate inventory
					pSettings->GetServerAddress(), // OCS Server address
					pSettings->GetServerPort(), // OCS Server port
					pSettings->IsDebugEnabled() ? _T( "/DEBUG") : _T( ""), // enable debug
					pSettings->IsProxyDisabled() ? _T( "/NP") : _T( ""), // disabke IE proxy
					pSettings->GetAgentOthersOptions(), // Others OCS agent options
					pSettings->GetAgentSetupDirectory()); // OCS agent setup directory
	// Launch RemCom.exe to start agent setup on remote computer
	if ((dwErr = WinRemoteExec( csComputer, csTemp)) != ERROR_SUCCESS)
	{
		csTemp.FormatMessage( IDS_ERROR_LAUNCHING_SETUP, csComputer, LookupError( dwErr));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		goto WIN_COMPUTER_DISCONNECT;
	}
	///////////////////////////////////////////////////////////
	// Try to get setup result
	csTemp.FormatMessage( IDS_STATUS_DISPLAYING_LOG, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	csTargetFile.Format( _T( "%s%s"), csDestDir, WIN_AGENT_SETUP_LOG);
	if (!myFile.Open( csTargetFile, CFile::modeRead))
	{
		csTemp.FormatMessage( IDS_ERROR_LOG_NOT_FOUND, csComputer, LookupError( GetLastError()));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		goto WIN_COMPUTER_DISCONNECT;
	}
	while (myFile.ReadString( csTemp))
	{
		// Check if SUCCESS found in string
		if (csTemp.Find( WIN_AGENT_SETUP_SUCCESS) >= 0)
			bSetupSuccess = TRUE;
		// Add log to listbox
		csTemp.FormatMessage( IDS_STATUS_LOG_FILE, csComputer, csTemp);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	}
	myFile.Close();
	if (bSetupSuccess)
		csTemp.FormatMessage( IDS_STATUS_AGENT_SETUP_SUCCESS, csComputer);
	else
		csTemp.FormatMessage( IDS_ERROR_AGENT_SETUP_FAILURE, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	///////////////////////////////////////////////////////////
	// Launch inventory if needed
	if (pSettings->IsLaunchNowRequired())
	{
		csTemp.FormatMessage( IDS_STATUS_LAUNCHING_INVENTORY, csComputer);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	}
	///////////////////////////////////////////////////////////
	// Disconnect
WIN_COMPUTER_DISCONNECT:
	csTemp.FormatMessage( IDS_STATUS_DISCONNECTING_REMOTE_HOST, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	csTemp.Format( _T( "\\\\%s\\ADMIN$"), csComputer);
	dwErr = WNetCancelConnection2( (LPTSTR) LPCTSTR( csTemp), 0, NULL );
	if (!bSetupSuccess)
		pFailedList->AddTail( csComputer);
	return bSetupSuccess;
}

LPCTSTR GetFileName(CString csFile)
{
	int i;
	static CString csTemp;

	if ((i = csFile.ReverseFind( '\\')) == -1)
		csTemp = csFile;
	else
		csTemp = csFile.Right( csFile.GetLength() - i - 1);
	return csTemp;
}

LPCTSTR GetFolderName(CString csFile)
{
	int i;
	static CString csTemp;

	if ((i = csFile.ReverseFind( '\\')) == -1)
		csTemp = csFile;
	else
		csTemp = csFile.Left( i);
	return csTemp;
}

CString LookupError( DWORD Err)
{
	LPVOID lpMsgBuf;
	DWORD dwErr;
	HMODULE hModule = NULL; // default to system source
 
    //
    // if dwLastError is in the network range, load the message source
    //
	if (Err >= NERR_BASE && Err <= MAX_NERR) 
		hModule = LoadLibraryEx(TEXT("netmsg.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE);

	dwErr = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM | // always consider system table
        ((hModule != NULL) ? FORMAT_MESSAGE_FROM_HMODULE : 0),
        hModule, // module to get message from (NULL == system)
        Err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL);

	if(hModule != NULL)
      FreeLibrary(hModule);

	if (dwErr == 0)
		return "";

	CString Msg((LPCTSTR) lpMsgBuf);
	Msg = " " + Msg;
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return Msg;
}

DWORD WinRemoteExec( LPCTSTR lpstrComputer, LPCTSTR lpstrCommand)
{
	CString			csComputer = lpstrComputer, // Computer
					csCommand,		// Command to execute
					csTemp,			// Temporary buffer
					csRemComBin,	// RemCom path
					csBatchFile;	// Batch file to execute using RemCom
	STARTUPINFO		si;				// To create RemCom process
	PROCESS_INFORMATION pi;
	DWORD			dwErr;
	CStdioFile		myFile;			// To read/write text files

	// First create command file
	csCommand.Format( _T( "@echo off\n%s"), lpstrCommand);
	if (GetTempPath( 4*_MAX_PATH, csBatchFile.GetBuffer( 4*_MAX_PATH+1)) == 0)
		return ERROR_PATH_NOT_FOUND;
	csBatchFile.ReleaseBuffer();
	csBatchFile += csComputer;
	csBatchFile += _T( "_");
	csBatchFile += WIN_AGENT_BATCH_FILE;
	if (!myFile.Open( csBatchFile, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		return ERROR_FILE_NOT_FOUND;
	myFile.WriteString( csCommand);
	myFile.Close();
	// Next, get path to RemCom.exe
	if (GetModuleFileName(AfxGetApp()->m_hInstance, csTemp.GetBuffer( 4*_MAX_PATH+1), 4*_MAX_PATH) == 0)
		return GetLastError();
	csTemp.ReleaseBuffer();
	csRemComBin.Format( _T( "%s\\%s"), GetFolderName( csTemp), REMOTE_EXECUTOR);
	// Next, construct command to execute with RemCom, using command file created before
	csTemp.Format( _T( "\"%s\" \\\\%s /c \"%s\""),
					csRemComBin, // RemCom path
					csComputer,	// Remote computer
					csBatchFile); // Command to launch
	// Launch RemCom.exe to start agent setup on remote computer
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow=SW_HIDE;
	if (CreateProcess( NULL, csTemp.GetBuffer(0),NULL, NULL, FALSE, 0, NULL, NULL, &si,&pi))
	{
		// Process created
		if (WaitForSingleObject( pi.hProcess, 240000 ) == WAIT_TIMEOUT)
		{
			// Process did not ended correctly, kill it
			TerminateProcess( pi.hProcess, 0);
		}
		if (GetExitCodeProcess( pi.hProcess, &dwErr) && (dwErr != ERROR_SUCCESS))
			//  The specified server cannot perform the requested operation.
			return ERROR_BAD_NET_RESP;
	}
	else
	{
		return GetLastError();
	}
	// Remove script file
	CFile::Remove( csBatchFile);
	return ERROR_SUCCESS;
}


DWORD CopyToUnix( LPCTSTR lpstrComputer, CAgentSettings *pSettings, LPCTSTR lpstrLocalFile, LPCTSTR lpstrRemoteFile, BOOL bHide)
{
	CString			csTemp,			// Temporary buffer
					csPscpBin,		// PSCP path
					csSshKey,		// PSCP option for SSH Key
					csPassword,		// PSCP option for password
					csBatchMode;	// PSCP option for batch mode
	STARTUPINFO		si;				// To create RemCom process
	PROCESS_INFORMATION pi;
	DWORD			dwErr;

	// First, get path to pscp.exe
	if (GetModuleFileName(AfxGetApp()->m_hInstance, csTemp.GetBuffer( 4*_MAX_PATH+1), 4*_MAX_PATH) == 0)
		return GetLastError();
	csTemp.ReleaseBuffer();
	csPscpBin.Format( _T( "%s\\%s"), GetFolderName( csTemp), SSH_FILE_TRANSFER);
	// Next, construct command to execute with PSCP
	// PSCP uses credentials or SSH key
	if (pSettings->IsSshKeyAvailable())
		// Using provided SSH key
		csSshKey.Format( _T( "-i \"%s\""), pSettings->GetSshKey());
	else
		// Using provided credentials, not SSH Key
		csSshKey.Empty();
	// PSCP use password or not if using PuTTY Agent
	if (pSettings->IsPasswordAvailable())
		// Password provided
		csPassword.Format( _T( "-pw %s"), pSettings->GetPassword());
	else
		// No password !!
		csPassword.Empty();
	// PSCP use batch mode or not
	if (bHide)
		// Hidding DOS prompt, so enable batch mode
		csBatchMode = _T( "-batch");
	else
		// User can view DOS prompt, no batch mode
		csBatchMode.Empty();
	csTemp.Format( _T( "\"%s\" %s %s %s -P %d \"%s\" %s@%s:%s"),
					csPscpBin, // PSCP path
					csBatchMode,
					csSshKey,
					csPassword,
					AfxGetApp()->GetProfileInt( SETTING_SECTION, OPTION_TEST_PORT_UNIX, DEFAULT_TEST_PORT_UNIX),
					lpstrLocalFile, 
					pSettings->GetLoginName(),
					lpstrComputer,	// Remote computer
					lpstrRemoteFile);
	// Launch pscp.exe to start copy on remote computer
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow=(bHide ? SW_HIDE : SW_SHOW);
	if (CreateProcess( NULL, csTemp.GetBuffer(0),NULL, NULL, FALSE, 0, NULL, NULL, &si,&pi))
	{
		// Process created
		if (WaitForSingleObject( pi.hProcess, 240000 ) == WAIT_TIMEOUT)
		{
			// Process did not ended correctly, kill it
			TerminateProcess( pi.hProcess, 0);
		}
		if (GetExitCodeProcess( pi.hProcess, &dwErr) && (dwErr != ERROR_SUCCESS))
			//  The specified server cannot perform the requested operation.
			return ERROR_BAD_NET_RESP;
	}
	else
	{
		return GetLastError();
	}
	return ERROR_SUCCESS;
}


DWORD CopyFromUnix( LPCTSTR lpstrComputer, CAgentSettings *pSettings, LPCTSTR lpstrLocalFile, LPCTSTR lpstrRemoteFile, BOOL bHide)
{
	CString			csTemp,			// Temporary buffer
					csPscpBin,		// PSCP path
					csSshKey,		// PSCP option for SSH Key
					csPassword,		// PSCP option for password
					csBatchMode;	// PSCP option for batch mode
	STARTUPINFO		si;				// To create RemCom process
	PROCESS_INFORMATION pi;
	DWORD			dwErr;

	// First, get path to pscp.exe
	if (GetModuleFileName(AfxGetApp()->m_hInstance, csTemp.GetBuffer( 4*_MAX_PATH+1), 4*_MAX_PATH) == 0)
		return GetLastError();
	csTemp.ReleaseBuffer();
	csPscpBin.Format( _T( "%s\\%s"), GetFolderName( csTemp), SSH_FILE_TRANSFER);
	// Next, construct command to execute with PSCP
	// PSCP uses credentials or SSH key
	if (pSettings->IsSshKeyAvailable())
		// Using provided SSH key
		csSshKey.Format( _T( "-i \"%s\""), pSettings->GetSshKey());
	else
		// Using provided credentials, not SSH Key
		csSshKey.Empty();
	// PSCP use password or not if using PuTTY Agent
	if (pSettings->IsPasswordAvailable())
		// Password provided
		csPassword.Format( _T( "-pw %s"), pSettings->GetPassword());
	else
		// No password !!
		csPassword.Empty();
	// PSCP use batch mode or not
	if (bHide)
		// Hidding DOS prompt, so enable batch mode
		csBatchMode = _T( "-batch");
	else
		// User can view DOS prompt, no batch mode
		csBatchMode.Empty();
	csTemp.Format( _T( "\"%s\" %s %s %s -P %d %s@%s:%s %s"),
					csPscpBin, // PSCP path
					csBatchMode,
					csSshKey,
					csPassword,
					AfxGetApp()->GetProfileInt( SETTING_SECTION, OPTION_TEST_PORT_UNIX, DEFAULT_TEST_PORT_UNIX),
					pSettings->GetLoginName(),
					lpstrComputer,	// Remote computer
					lpstrRemoteFile,
					lpstrLocalFile);
	// Launch pscp.exe to start copy from remote computer
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow=(bHide ? SW_HIDE : SW_SHOW);
	if (CreateProcess( NULL, csTemp.GetBuffer(0),NULL, NULL, FALSE, 0, NULL, NULL, &si,&pi))
	{
		// Process created
		if (WaitForSingleObject( pi.hProcess, 240000 ) == WAIT_TIMEOUT)
		{
			// Process did not ended correctly, kill it
			TerminateProcess( pi.hProcess, 0);
		}
		if (GetExitCodeProcess( pi.hProcess, &dwErr) && (dwErr != ERROR_SUCCESS))
			//  The specified server cannot perform the requested operation.
			return ERROR_BAD_NET_RESP;
	}
	else
	{
		return GetLastError();
	}
	return ERROR_SUCCESS;
}


DWORD UnixRemoteExec( LPCTSTR lpstrComputer, CAgentSettings *pSettings, LPCTSTR lpstrCommand, BOOL bHide)
{
	CString			csTemp,			// Temporary buffer
					csPuttyBin,		// PuTTY path
					csSshKey,		// PSCP option for SSH Key
					csPassword,		// PSCP option for password
					csCommandFile;
	STARTUPINFO		si;				// To create RemCom process
	PROCESS_INFORMATION pi;
	DWORD			dwErr;
	CStdioFile		myFile;

	// First, create command file
	if (GetTempPath( 4*_MAX_PATH, csCommandFile.GetBuffer( 4*_MAX_PATH+1)) == 0)
		return ERROR_PATH_NOT_FOUND;
	csCommandFile.ReleaseBuffer();
	csCommandFile += lpstrComputer;
	csCommandFile += _T( "_putty_command.sh");
	if (!myFile.Open( csCommandFile, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		return ERROR_FILE_NOT_FOUND;
	myFile.WriteString( lpstrCommand);
	myFile.Close();
	// Next, get path to pscp.exe
	if (GetModuleFileName(AfxGetApp()->m_hInstance, csTemp.GetBuffer( 4*_MAX_PATH+1), 4*_MAX_PATH) == 0)
		return GetLastError();
	csTemp.ReleaseBuffer();
	csPuttyBin.Format( _T( "%s\\%s"), GetFolderName( csTemp), SSH_EXECUTOR);
	// Construct command to execute with PuTTY
	// PSCP uses credentials or SSH key
	if (pSettings->IsSshKeyAvailable())
		// Using provided SSH key
		csSshKey.Format( _T( "-i \"%s\""), pSettings->GetSshKey());
	else
		// Using provided credentials, not SSH Key
		csSshKey.Empty();
	// PSCP use password or not if using PuTTY Agent
	if (pSettings->IsPasswordAvailable())
		// Password provided
		csPassword.Format( _T( "-pw %s"), pSettings->GetPassword());
	else
		// No password !!
		csPassword.Empty();
	csTemp.Format( _T( "\"%s\" -ssh %s %s -P %d -m \"%s\" %s@%s"),
					csPuttyBin, // PuTTY path
					csSshKey,
					csPassword,
					AfxGetApp()->GetProfileInt( SETTING_SECTION, OPTION_TEST_PORT_UNIX, DEFAULT_TEST_PORT_UNIX),
					csCommandFile,
					pSettings->GetLoginName(),
					lpstrComputer);
	// Launch pscp.exe to start copy on remote computer
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	si.dwFlags=STARTF_USESHOWWINDOW;
	si.wShowWindow=(bHide ? SW_HIDE : SW_SHOW);
	if (CreateProcess( NULL, csTemp.GetBuffer(0),NULL, NULL, FALSE, 0, NULL, NULL, &si,&pi))
	{
		// Process created
		if (WaitForSingleObject( pi.hProcess, 240000 ) == WAIT_TIMEOUT)
		{
			// Process did not ended correctly, kill it
			TerminateProcess( pi.hProcess, 0);
		}
		if (GetExitCodeProcess( pi.hProcess, &dwErr) && (dwErr != ERROR_SUCCESS))
			//  The specified server cannot perform the requested operation.
			return ERROR_BAD_NET_RESP;
	}
	else
	{
		return GetLastError();
	}
	CFile::Remove( csCommandFile);
	return ERROR_SUCCESS;
}


BOOL UnixRemoteInstall( CWorkerThreadParam *pParam)
{
	CString			csComputer = pParam->GetComputer(), // Computer to setup
					csTemp,			// Temporary buffer
					csDestDir,		// Destination directory in UNC (\\computer\drive$\folder)
					csSourceFile,	// Source file to copy
					csTargetFile;	// Target destination file
	HWND			hWnd = pParam->GetHwnd();
	CAgentSettings  *pSettings = pParam->GetSettings();
	CStringList		*pFailedList = pParam->GetFailedList();
	DWORD			dwErr;
	CStringList		*pList;			// Computer list to setup
	POSITION		pos;			// Position into string list
	CStdioFile		myFile;			// To read/write text files
	BOOL			bSetupSuccess = FALSE; // Is agent setup successful ?

	///////////////////////////////////////////////////////////
	// First, test connection to remote host on SSH port
	csTemp.FormatMessage( IDS_STATUS_TESTING_REMOTE_HOST, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	if (!TestConnection( csComputer, AfxGetApp()->GetProfileInt( SETTING_SECTION, OPTION_TEST_PORT_UNIX, DEFAULT_TEST_PORT_UNIX)))
	{
		csTemp.FormatMessage( IDS_ERROR_TESTING_REMOTE_HOST, csComputer);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		pFailedList->AddTail( csComputer);
		return FALSE;
	}
	///////////////////////////////////////////////////////////
	// Copy setup file to remote host using PSCP
	csTemp.FormatMessage( IDS_STATUS_COPYING_FILES, csComputer, pSettings->GetAgentSetupDirectory());
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	csSourceFile = pSettings->GetAgentSetupFile();
	csTargetFile.Format( _T( "%s/%s"), pSettings->GetAgentSetupDirectory(), GetFileName( pSettings->GetAgentSetupFile()));
	dwErr = CopyToUnix( csComputer, pSettings, csSourceFile, csTargetFile, FALSE);
	if (dwErr != NO_ERROR)
	{
		csTemp.FormatMessage( IDS_ERROR_COPYING_FILES, csComputer, LookupError( dwErr));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		pFailedList->AddTail( csComputer);
		return FALSE;
	}
	///////////////////////////////////////////////////////////
	// Uncompress, untar and launch setup
	csTemp.FormatMessage( IDS_STATUS_LAUNCHING_SETUP, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	// Get uncompressed file name
	csSourceFile = GetFileName( pSettings->GetAgentSetupFile());
	csSourceFile.Replace( _T( ".gz"), _T( ""));
	// Get untar folder name
	csTargetFile = csSourceFile;
	csTargetFile.Replace( _T( ".tar"), _T( ""));
	// Create command
	csTemp.Format( _T( "cd %s && gzip -d %s && tar -xf %s && cd %s && sh setup.sh %s %u \"%s\" %u"),
					pSettings->GetAgentSetupDirectory(), // Directory where agent setup file copied
					GetFileName( pSettings->GetAgentSetupFile()), // remote agent setup file to uncompress
					csSourceFile, // remote agent setup file to untar
					csTargetFile, // remote directory extracted
					pSettings->GetServerAddress(), // OCS Server address
					pSettings->GetServerPort(), // OCS Server port
					pSettings->GetTagValue(), // TAG Value
					pSettings->IsDaemonEnabled() ? _T( "1") : _T( "0")); // Use daemon mode
	// Launch PuTTY.exe to start agent setup on remote computer
	if ((dwErr = UnixRemoteExec( csComputer, pSettings, csTemp)) != ERROR_SUCCESS)
	{
		csTemp.FormatMessage( IDS_ERROR_LAUNCHING_SETUP, csComputer, LookupError( dwErr));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		goto UNIX_COMPUTER_DISCONNECT;
	}
	///////////////////////////////////////////////////////////
	// Try to get setup result using PSCP
	csTemp.FormatMessage( IDS_STATUS_DISPLAYING_LOG, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	csSourceFile.Format( _T( "%s/%s/%s"), pSettings->GetAgentSetupDirectory(), csTargetFile, UNIX_AGENT_SETUP_LOG);
	if (GetTempPath( 4*_MAX_PATH, csTargetFile.GetBuffer( 4*_MAX_PATH+1)) == 0)
		return ERROR_PATH_NOT_FOUND;
	csTargetFile.ReleaseBuffer();
	csTargetFile += csComputer;
	csTargetFile += _T( "_");
	csTargetFile += UNIX_AGENT_SETUP_LOG;
	dwErr = CopyFromUnix( csComputer, pSettings, csTargetFile, csSourceFile);
	if (dwErr != NO_ERROR)
	{
		csTemp.FormatMessage( IDS_ERROR_COPYING_FILES, csComputer, LookupError( dwErr));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		goto UNIX_COMPUTER_DISCONNECT;
	}
	if (!myFile.Open( csTargetFile, CFile::modeRead))
	{
		csTemp.FormatMessage( IDS_ERROR_LOG_NOT_FOUND, csComputer, LookupError( GetLastError()));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		goto UNIX_COMPUTER_DISCONNECT;
	}
	while (myFile.ReadString( csTemp))
	{
		// Check if SUCCESS found in string
		if (csTemp.Find( UNIX_AGENT_SETUP_SUCCESS) >= 0)
			bSetupSuccess = TRUE;
		// Add log to listbox
		csTemp.FormatMessage( IDS_STATUS_LOG_FILE, csComputer, csTemp);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	}
	myFile.Close();
	CFile::Remove( csTargetFile);
	///////////////////////////////////////////////////////////
	// Next, copy additional files to agent state directory
	csTemp.FormatMessage( IDS_STATUS_COPYING_FILES, csComputer, DEFAULT_UNIX_AGENT_STATE_DIRECTORY);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	pList = pSettings->GetAgentOtherFiles();
	pos = pList->GetHeadPosition();
	while (pos)
	{
		csSourceFile = pList->GetNext( pos);
		csTargetFile.Format( _T( "%s/%s:%u/%s"), DEFAULT_UNIX_AGENT_STATE_DIRECTORY,
							pSettings->GetServerAddress(),
							pSettings->GetServerPort(),
							GetFileName( csSourceFile));
		dwErr = CopyToUnix( csComputer, pSettings, csSourceFile, csTargetFile, FALSE);
		if (dwErr != NO_ERROR)
		{
			// Unable to copy file
			csTemp.FormatMessage( IDS_ERROR_COPYING_FILES, csComputer, LookupError( GetLastError()));
			::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
			goto UNIX_COMPUTER_DISCONNECT;
		}
	}
	///////////////////////////////////////////////////////////
	// Launch inventory if needed
	if (pSettings->IsLaunchNowRequired())
	{
		csTemp.FormatMessage( IDS_STATUS_LAUNCHING_INVENTORY, csComputer);
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		// Create command
		csTemp.Format( _T( "/bin/ocsinv -s=%s:%u -f"),
						pSettings->GetServerAddress(), // OCS Server address
						pSettings->GetServerPort()); // OCS Server port
		// Launch PuTTY.exe to start agent setup on remote computer
		if ((dwErr = UnixRemoteExec( csComputer, pSettings, csTemp)) != ERROR_SUCCESS)
		{
			csTemp.FormatMessage( IDS_ERROR_LAUNCHING_SETUP, csComputer, LookupError( dwErr));
			::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
		}
	}
	if (bSetupSuccess)
		csTemp.FormatMessage( IDS_STATUS_AGENT_SETUP_SUCCESS, csComputer);
	else
		csTemp.FormatMessage( IDS_ERROR_AGENT_SETUP_FAILURE, csComputer);
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	///////////////////////////////////////////////////////////
	// Disconnect
UNIX_COMPUTER_DISCONNECT:
	///////////////////////////////////////////////////////////
	// Clean temporary setup directory using PuTTY
	csTemp.FormatMessage( IDS_STATUS_CLEANING_TEMP_DIR, csComputer, pSettings->GetAgentSetupDirectory());
	::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	// Get uncompressed folder name
	csSourceFile = GetFileName( pSettings->GetAgentSetupFile());
	csSourceFile.Replace( _T( ".tar.gz"), _T( ""));
	// Create command
	csTemp.Format( _T( "cd %s && /bin/rm -Rf %s*"),
					pSettings->GetAgentSetupDirectory(), // Directory where agent setup file copied
					csSourceFile); // OCS Server port
	// Launch PuTTY.exe to start agent setup on remote computer
	if ((dwErr = UnixRemoteExec( csComputer, pSettings, csTemp)) != ERROR_SUCCESS)
	{
		csTemp.FormatMessage( IDS_ERROR_CLEANING_TEMP_DIR, csComputer, LookupError( dwErr));
		::SendMessage( hWnd, WM_SETTEXT, IDC_MESSAGE_HANDLER_LISTBOX, (LPARAM) LPCTSTR( csTemp));
	}
	if (!bSetupSuccess)
		pFailedList->AddTail( csComputer);
	return bSetupSuccess;
}
