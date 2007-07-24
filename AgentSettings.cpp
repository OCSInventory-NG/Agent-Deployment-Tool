//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// AgentSettings.cpp: implementation of the CAgentSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ocs_deploy_tool.h"
#include "AgentSettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAgentSettings::CAgentSettings()
{
	m_uOS = AGENT_OS_UNKNOWN;
	m_bLaunchNow = FALSE;
	m_bDisableProxy = FALSE;
	m_bDebug = FALSE;
	m_uPort = DEFAULT_SERVER_PORT;
	m_csServer = DEFAULT_SERVER_ADDRESS;
	m_bDaemon = FALSE;
}

CAgentSettings::~CAgentSettings()
{

}


LPCTSTR CAgentSettings::GetLoginName()
{
	return m_csLoginName;
}

BOOL CAgentSettings::IsLoginNameAvailable()
{
	return !m_csLoginName.IsEmpty();
}

LPCTSTR CAgentSettings::GetPassword()
{
	return m_csPassword;
}

BOOL CAgentSettings::IsPasswordAvailable()
{
	return !m_csPassword.IsEmpty();
}

LPCTSTR CAgentSettings::GetSshKey()
{
	return m_csSshKey;
}

BOOL CAgentSettings::IsSshKeyAvailable()
{
	return !m_csSshKey.IsEmpty();
}

UINT CAgentSettings::GetTargetOS()
{
	return m_uOS;
}

CString CAgentSettings::GetAgentSetupFile()
{
	return m_csAgentSetupFile;
}

CStringList * CAgentSettings::GetAgentOtherFiles()
{
	return &m_AgentOtherFiles;
}

CString CAgentSettings::GetAgentOthersOptions()
{
	return m_csAgentOtherOptions;
}

BOOL CAgentSettings::IsDebugEnabled()
{
	return m_bDebug;
}

BOOL CAgentSettings::IsDaemonEnabled()
{
	return m_bDaemon;
}

BOOL CAgentSettings::IsProxyDisabled()
{
	return m_bDisableProxy;
}

BOOL CAgentSettings::IsLaunchNowRequired()
{
	return m_bLaunchNow;
}

CString CAgentSettings::GetServerAddress()
{
	return m_csServer;
}

UINT CAgentSettings::GetServerPort()
{
	return m_uPort;
}

CString CAgentSettings::GetAgentSetupDirectory()
{
	return m_csAgentDirectory;
}

CString CAgentSettings::GetTagValue()
{
	return m_csTag;
}

void CAgentSettings::SetLoginName(CString csLogin)
{
	m_csLoginName = csLogin;
}

void CAgentSettings::SetPassword(CString csPassword)
{
	m_csPassword = csPassword;
}

void CAgentSettings::SetSshKey( CString csSshKeyFile)
{
	m_csSshKey = csSshKeyFile;
}

void CAgentSettings::SetAgentSetupFile( CString csFile)
{
	m_csAgentSetupFile = csFile;
}

void CAgentSettings::SetAgentOthersOptions( CString csOptions)
{
	m_csAgentOtherOptions = csOptions;
}

void CAgentSettings::SetDebugEnabled( BOOL bDebug)
{
	m_bDebug = bDebug;
}

void CAgentSettings::SetProxyDisabled( BOOL bDisabled)
{
	m_bDisableProxy = bDisabled;
}

void CAgentSettings::SetLaunchNowRequired( BOOL bLaunch)
{
	m_bLaunchNow = bLaunch;
}

void CAgentSettings::SetDaemonEnabled( BOOL bDaemon)
{
	m_bDaemon = bDaemon;
}

void CAgentSettings::SetServerAddress( CString csServer)
{
	m_csServer = csServer;
}

void CAgentSettings::SetServerPort( UINT uPort)
{
	m_uPort = uPort;
}

void CAgentSettings::SetAgentSetupDirectory( CString csDirectory)
{
	m_csAgentDirectory = csDirectory;
}

void CAgentSettings::SetTagValue( CString csTag)
{
	m_csTag = csTag;
}

