//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// OCS_DEPLOY_TOOL.h : main header file for the OCS_DEPLOY_TOOL application
//

#if !defined(AFX_OCS_DEPLOY_TOOL_H__B7D94451_1DAB_4B40_BE54_72371F625D1D__INCLUDED_)
#define AFX_OCS_DEPLOY_TOOL_H__B7D94451_1DAB_4B40_BE54_72371F625D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define OPTION_TEST_PORT_WINDOWS _T( "Test Windows Connection on Port")
#define DEFAULT_TEST_PORT_WINDOWS	445
#define OPTION_TEST_PORT_UNIX _T( "Test Unix Connection on Port")
#define DEFAULT_TEST_PORT_UNIX		22

#define SETTINGS_SECTION	_T( "Settings")
#define SETTING_PSEXEC_PATH _T( "PsExec")
#define SETTING_PUTTY_PATH	_T( "PuTTY")
#define SETTING_PSCP_PATH	_T( "Pscp")

// Test connection to remote Host
// Return ERROR_SUCCESS if connection successful
BOOL TestConnection( LPCTSTR lpstrAddress, int nPort);

// Allow retreiving from system error message associated to an error code
CString LookupError( DWORD Err);

// Transform UNICODE string to ANSI string
CStringA GetAnsiFromUnicode(LPCTSTR a_wstrString);

// Transform ANSI string to UNICODE string
CStringW GetUnicodeFromAnsi(LPCSTR a_strString);

// Create directory (and every missing parent directories if needed)
BOOL directoryCreate( LPCTSTR lpstrDir);

// Delete directory (and every file or sub directory)
BOOL directoryDelete( LPCTSTR lpstrDir);

// Check if file or directory exists
BOOL fileExists( LPCTSTR lpstrFile);

/////////////////////////////////////////////////////////////////////////////
// COCS_DEPLOY_TOOLApp:
// See OCS_DEPLOY_TOOL.cpp for the implementation of this class
//

class COCS_DEPLOY_TOOLApp : public CWinApp
{
public:
	COCS_DEPLOY_TOOLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COCS_DEPLOY_TOOLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COCS_DEPLOY_TOOLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OCS_DEPLOY_TOOL_H__B7D94451_1DAB_4B40_BE54_72371F625D1D__INCLUDED_)
