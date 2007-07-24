//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
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
#define DEFAULT_TEST_PORT_WINDOWS	135
#define OPTION_TEST_PORT_UNIX _T( "Test Unix Connection on Port")
#define DEFAULT_TEST_PORT_UNIX		22

// Test connection to remote Host
// Return ERROR_SUCCESS if connection successful
BOOL TestConnection( LPCTSTR lpstrAddress, int nPort);


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
