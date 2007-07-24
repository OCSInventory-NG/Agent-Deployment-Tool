//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// LauncherThreadParam.cpp: implementation of the CLauncherThreadParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ocs_deploy_tool.h"
#include "AgentSettings.h"
#include "LauncherThreadParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLauncherThreadParam::CLauncherThreadParam()
{

}

CLauncherThreadParam::~CLauncherThreadParam()
{

}

void CLauncherThreadParam::SetParam( HWND hWnd, CAgentSettings *pSettings, CStringList *pComputers, CStringList *pFailed)
{
	m_hWnd = hWnd;
	m_pSettings = pSettings;
	m_pComputers = pComputers;
	m_pFailed = pFailed;
}

HWND CLauncherThreadParam::GetHwnd()
{
	return m_hWnd;
}

CAgentSettings * CLauncherThreadParam::GetSettings()
{
	return m_pSettings;
}

CStringList * CLauncherThreadParam::GetComputersList()
{
	return m_pComputers;
}

CStringList * CLauncherThreadParam::GetFailedList()
{
	return m_pFailed;
}
