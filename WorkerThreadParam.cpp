//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// WorkerThreadParam.cpp: implementation of the CWorkerThreadParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ocs_deploy_tool.h"
#include "AgentSettings.h"
#include "WorkerThreadParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkerThreadParam::CWorkerThreadParam()
{

}

CWorkerThreadParam::~CWorkerThreadParam()
{

}

void CWorkerThreadParam::SetParam( HWND hWnd, CAgentSettings *pSettings, CStringList *pFailed, LPCTSTR lpstrComputer, LPCTSTR lpstrLocalDir)
{
	m_hWnd = hWnd;
	m_pSettings = pSettings;
	m_pFailed = pFailed;
	m_csComputer = lpstrComputer;
	m_csLocalDir = lpstrLocalDir;
}

HWND CWorkerThreadParam::GetHwnd()
{
	return m_hWnd;
}

CAgentSettings * CWorkerThreadParam::GetSettings()
{
	return m_pSettings;
}

CStringList * CWorkerThreadParam::GetFailedList()
{
	return m_pFailed;
}

LPCTSTR CWorkerThreadParam::GetComputer()
{
 	return m_csComputer;
}

LPCTSTR CWorkerThreadParam::GetLocaLDir()
{
 	return m_csLocalDir;
}
