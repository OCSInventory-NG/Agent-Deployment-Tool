//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// WinAgentSettings.cpp: implementation of the CWinAgentSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ocs_deploy_tool.h"
#include "AgentSettings.h"
#include "WinAgentSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWinAgentSettings::CWinAgentSettings()
{
	m_uOS = AGENT_OS_WINDOWS;
	m_csAgentDirectory = DEFAULT_WIN_AGENT_DIRECTORY;
}

CWinAgentSettings::~CWinAgentSettings()
{

}
