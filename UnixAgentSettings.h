//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// UnixAgentSettings.h: interface for the CUnixAgentSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIXAGENTSETTINGS_H__A25A405B_BADE_4E85_8EDA_45B4D0774D23__INCLUDED_)
#define AFX_UNIXAGENTSETTINGS_H__A25A405B_BADE_4E85_8EDA_45B4D0774D23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_UNIX_AGENT_TEMP_DIRECTORY	_T( "/tmp")
#define DEFAULT_UNIX_AGENT_VAR_DIRECTORY	_T( "/var/lib/ocsinventory-agent")
#define DEFAULT_UNIX_AGENT_ETC_DIRECTORY	_T( "/etc/ocsinventory")

#define OPTION_UNIX_ROOT_LOGIN				_T( "Unix Root login")
#define OPTION_UNIX_SSH_KEY					_T( "Unix SSH key")
#define OPTION_UNIX_AGENT_SETUP_FILE		_T( "Unix Agent Setup File")
#define OPTION_UNIX_AGENT_OTHER_FILES		_T( "Unix Agent Other Files")
#define OPTION_UNIX_AGENT_ETC_DIRECTORY		_T( "Unix Agent etc Directory")
#define OPTION_UNIX_AGENT_VAR_DIRECTORY		_T( "Unix Agent var Directory")

#define SSH_FILE_TRANSFER					_T( "pscp.exe")
#define SSH_EXECUTOR						_T( "putty.exe")

#define UNIX_AGENT_BATCH_FILE				_T( "setup.sh")
#define UNIX_AGENT_CONFIG_FILE				_T( "ocsinventory-agent.cfg")
#define UNIX_AGENT_MODULES_FILE				_T( "modules.conf")
#define UNIX_AGENT_ADM_FILE					_T( "ocsinv.adm")
#define UNIX_AGENT_SETUP_LOG				_T( "ocs_agent_setup.log")
#define UNIX_AGENT_SETUP_SUCCESS			_T( "SUCCESS")

class CUnixAgentSettings : public CAgentSettings  
{
public:
	CUnixAgentSettings();
	virtual ~CUnixAgentSettings();

};

#endif // !defined(AFX_UNIXAGENTSETTINGS_H__A25A405B_BADE_4E85_8EDA_45B4D0774D23__INCLUDED_)
