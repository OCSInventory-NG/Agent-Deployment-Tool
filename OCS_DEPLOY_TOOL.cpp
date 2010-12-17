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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
