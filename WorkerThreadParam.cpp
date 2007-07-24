//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
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

void CWorkerThreadParam::SetParam( HWND hWnd, CAgentSettings *pSettings, CStringList *pFailed, LPCTSTR lpstrComputer)
{
	m_hWnd = hWnd;
	m_pSettings = pSettings;
	m_pFailed = pFailed;
	m_csComputer = lpstrComputer;
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
