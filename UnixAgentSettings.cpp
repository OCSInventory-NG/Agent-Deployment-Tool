//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// UnixAgentSettings.cpp: implementation of the CUnixAgentSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ocs_deploy_tool.h"
#include "AgentSettings.h"
#include "UnixAgentSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnixAgentSettings::CUnixAgentSettings()
{
	m_uOS = AGENT_OS_UNIX;
	m_csAgentDirectory = DEFAULT_UNIX_AGENT_VAR_DIRECTORY;
	m_csAgentEtcDirectory = DEFAULT_UNIX_AGENT_ETC_DIRECTORY;
}

CUnixAgentSettings::~CUnixAgentSettings()
{

}
