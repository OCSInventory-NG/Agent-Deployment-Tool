//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// WinAgentSettings.h: interface for the CWinAgentSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINAGENTSETTINGS_H__8F6ED0EE_7DE9_4AFE_B84E_7F3E7423EDB3__INCLUDED_)
#define AFX_WINAGENTSETTINGS_H__8F6ED0EE_7DE9_4AFE_B84E_7F3E7423EDB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Where to find in the registry default folder C:\Program Files ?
#define WIN_DEFAULT_PROGRAM_FILES_KEY _T( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion")
#define WIN_DEFAULT_PROGRAM_FILES_VALUE _T( "ProgramFilesDir")

// Where to find in the registry default folder Common AppData ?
#define WIN_COMMON_APPDATA_KEY _T( "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders")
#define WIN_COMMON_APPDATA_VALUE _T( "Common AppData")

// Where to setup agent by default
#define DEFAULT_WIN_AGENT_DIRECTORY _T( "PROGRAM_FILES\\OCS Inventory Agent")
#define DEFAULT_WIN_AGENT_DATA_DIRECTORY _T( "OCS Inventory NG\\Agent")

#define OPTION_WIN_ADMIN_LOGIN _T( "Windows Administrator login")
#define OPTION_WIN_AGENT_SETUP_FILE _T( "Windows Agent Setup File")
#define OPTION_WIN_AGENT_OTHER_FILES _T( "Windows Agent Other Files")
#define OPTION_WIN_INSTALLER_OPTIONS _T( "Windows Installer Command Line Options")
#define OPTION_WIN_AGENT_SETUP_DIRECTORY _T( "Windows Agent Setup Directory")

#define REMOTE_EXECUTOR _T( "psexec.exe")

#define WIN_AGENT_BATCH_FILE _T( "OcsAgentSetup.bat")
#define WIN_AGENT_SETUP_LOG _T( "OcsAgentSetup.log")
#define WIN_AGENT_SETUP_SUCCESS _T( "SUCESS")

class CWinAgentSettings : public CAgentSettings
{
public:
	CWinAgentSettings();
	virtual ~CWinAgentSettings();
};

#endif // !defined(AFX_WINAGENTSETTINGS_H__8F6ED0EE_7DE9_4AFE_B84E_7F3E7423EDB3__INCLUDED_)
