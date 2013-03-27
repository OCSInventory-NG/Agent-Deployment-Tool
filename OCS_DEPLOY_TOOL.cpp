//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// OCS_DEPLOY_TOOL.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OCS_DEPLOY_TOOL.h"
#include "OCS_DEPLOY_TOOLDlg.h"
#include <shlwapi.h>
#include <lmerr.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#include "vld.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// COCS_DEPLOY_TOOLApp

BEGIN_MESSAGE_MAP(COCS_DEPLOY_TOOLApp, CWinApp)
	//{{AFX_MSG_MAP(COCS_DEPLOY_TOOLApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COCS_DEPLOY_TOOLApp construction

COCS_DEPLOY_TOOLApp::COCS_DEPLOY_TOOLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COCS_DEPLOY_TOOLApp object

COCS_DEPLOY_TOOLApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COCS_DEPLOY_TOOLApp initialization

BOOL COCS_DEPLOY_TOOLApp::InitInstance()
{
	try
	{
		AfxEnableControlContainer();

		// Standard initialization
		// If you are not using these features and wish to reduce the size
		//  of your final executable, you should remove from the following
		//  the specific initialization routines you do not need.

		// Initialize socket support
		if (!AfxSocketInit())
		{
			AfxMessageBox( _T( "Unable to initialize Socket support !"), MB_OK|MB_ICONEXCLAMATION);
			return FALSE;
		}

		// Change the registry key under which our settings are stored.
		// TODO: You should modify this string to be something appropriate
		// such as the name of your company or organization.
		SetRegistryKey( _T("OCS Inventory NG"));

		COCS_DEPLOY_TOOLDlg dlg;
		m_pMainWnd = &dlg;
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}

	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


BOOL TestConnection( LPCTSTR lpstrAddress, int nPort)
{
	CSocket* pSocket;

	pSocket = new CSocket;
	ASSERT(pSocket);

	if (!pSocket->Create())
	{
		delete pSocket;
		pSocket = NULL;
		return FALSE;
	}

	while (!pSocket->Connect( lpstrAddress, nPort))
	{
		delete pSocket;
		pSocket = NULL;
		return FALSE;
	}

	pSocket->Close();
	delete pSocket;
	return TRUE;
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
		return _T( "");

	CString Msg((LPCTSTR) lpMsgBuf);
	Msg.Insert( 0, _T( " "));
	// Free the buffer.
	LocalFree( lpMsgBuf );
	return Msg;
}

CStringA GetAnsiFromUnicode(LPCTSTR a_wstrString)
{
	USES_CONVERSION;
/*	static char	szBuffer[1024*1024+1]; // 1MB buffer to handle string 

	if (_tcslen( a_wstrString) > 1024*1024)
		AfxThrowMemoryException();
	strcpy_s( szBuffer, 1024*1024, CT2CA(a_wstrString));
	return szBuffer;
*/
	static CStringA csAnsi;

	csAnsi = CT2CA(a_wstrString);
	return csAnsi;
}

CStringW GetUnicodeFromAnsi(LPCSTR a_strString)
{
	USES_CONVERSION;
/*	static TCHAR szBuffer[1024*1024]; // 1MB buffer to handle string 

	if (strlen( a_strString) > 1024*1024)
		AfxThrowMemoryException();
	_tcscpy_s( szBuffer, 1024*1024, CA2CT( a_strString));
	return szBuffer;
*/
	static CStringW csWide;

	csWide = A2CW(a_strString);
	return csWide;

}

BOOL directoryCreate( LPCTSTR lpstrDir)
{
	// Create it if not exists
	if (fileExists( lpstrDir))
		return TRUE;
	switch (SHCreateDirectoryEx( NULL, lpstrDir, NULL))
	{
	case ERROR_SUCCESS: // Directory create successful
	case ERROR_FILE_EXISTS: // Directory already exists
	case ERROR_ALREADY_EXISTS:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL directoryDelete( LPCTSTR lpstrDir)
{
	CFileFind cFinder;
	CString csPath;
	BOOL	bWorking;

	try
	{
		csPath.Format( _T( "%s\\*.*"), lpstrDir);
		bWorking = cFinder.FindFile( csPath);
		while (bWorking)
		{
			bWorking = cFinder.FindNextFile();
			if (cFinder.IsDots())
				continue;
			if (cFinder.IsDirectory())
				directoryDelete( cFinder.GetFilePath());
			else 
				DeleteFile( cFinder.GetFilePath());
		}
		cFinder.Close();
		return RemoveDirectory( lpstrDir);
	}
	catch (CException *pEx)
	{
		cFinder.Close();
		pEx->Delete();
		return FALSE;
	}
}

BOOL fileExists( LPCTSTR lpstrFile)
{
	return PathFileExists( lpstrFile); 
}
