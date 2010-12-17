//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// LauncherThreadParam.h: interface for the CLauncherThreadParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LAUNCHERTHREADPARAM_H__82253152_2B51_4700_9907_4578D601A767__INCLUDED_)
#define AFX_LAUNCHERTHREADPARAM_H__82253152_2B51_4700_9907_4578D601A767__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLauncherThreadParam  
{
public:
	CLauncherThreadParam();
	virtual ~CLauncherThreadParam();

	void SetParam( HWND hWnd, CAgentSettings *pSettings, CStringList *pComputers, CStringList *pFailed);
	HWND GetHwnd();
	CAgentSettings * GetSettings();
	CStringList * GetComputersList();
	CStringList * GetFailedList();

protected:
	HWND	m_hWnd;
	CAgentSettings *m_pSettings;
	CStringList *m_pComputers;
	CStringList *m_pFailed;
};

#endif // !defined(AFX_LAUNCHERTHREADPARAM_H__82253152_2B51_4700_9907_4578D601A767__INCLUDED_)
