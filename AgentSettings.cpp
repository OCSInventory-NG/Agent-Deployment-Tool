//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
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
	m_csServer = DEFAULT_SERVER_ADDRESS;
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

CStringList * CAgentSettings::GetPluginFiles()
{
	return &m_PluginFiles;
}

CString CAgentSettings::GetInstallerOptions()
{
	return m_csInstallerOptions;
}

CString CAgentSettings::GetServerAddress()
{
	return m_csServer;
}

CString CAgentSettings::GetAgentSetupDirectory()
{
	return m_csAgentDirectory;
}

CString CAgentSettings::GetAgentEtcDirectory()
{
	return m_csAgentEtcDirectory;
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

void CAgentSettings::SetInstallerOptions( CString csOptions)
{
	m_csInstallerOptions = csOptions;
}

void CAgentSettings::SetServerAddress( CString csServer)
{
	m_csServer = csServer;
}

void CAgentSettings::SetAgentSetupDirectory( CString csDirectory)
{
	m_csAgentDirectory = csDirectory;
}

void CAgentSettings::SetAgentEtcDirectory( CString csDirectory)
{
	m_csAgentEtcDirectory = csDirectory;
}
