//====================================================================================
// Open Computer and Software Inventory Next Generation
// Copyright (C) 2010 OCS Inventory NG Team. All rights reserved.
// Web: http://www.ocsinventory-ng.org

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence V2 http://www.gnu.org/ or Licence.txt
//====================================================================================

// AgentSettings.h: interface for the CAgentSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AGENTSETTINGS_H__4EE4E937_ADC9_4346_B54D_68C79E5C7C69__INCLUDED_)
#define AFX_AGENTSETTINGS_H__4EE4E937_ADC9_4346_B54D_68C79E5C7C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_SERVER_ADDRESS	_T( "http://ocsinventory-ng/ocsinventory")
#define AGENT_SECTION			_T( "Agents")
#define OPTION_SERVER_ADDRESS	_T( "OCS Inventory NG Communication Server Address")
#define AGENT_OS_UNKNOWN		0
#define AGENT_OS_WINDOWS		1
#define AGENT_OS_UNIX			2

class CAgentSettings  
{
public:
	CAgentSettings();
	virtual ~CAgentSettings();

	// Setter
	virtual void SetPassword( CString csPassword);
	virtual void SetLoginName( CString csLogin);
	virtual void SetSshKey( CString csSshKeyFile);
	virtual void SetAgentSetupFile( CString csFile);
	virtual void SetInstallerOptions( CString csOptions);
	virtual void SetServerAddress( CString csServer = DEFAULT_SERVER_ADDRESS);
	virtual void SetAgentSetupDirectory( CString csDirectory);
	virtual void SetAgentEtcDirectory( CString csDirectory);

	// Getter
	virtual UINT GetTargetOS();
	virtual LPCTSTR GetLoginName();
	virtual BOOL IsLoginNameAvailable();
	virtual LPCTSTR GetPassword();
	virtual BOOL IsPasswordAvailable();
	virtual LPCTSTR GetSshKey();
	virtual BOOL IsSshKeyAvailable();
	virtual CString GetAgentSetupDirectory();
	virtual CString GetServerAddress();
	virtual CString GetInstallerOptions();
	virtual CStringList * GetAgentOtherFiles();
	virtual CString GetAgentSetupFile();
	virtual CString GetAgentEtcDirectory();

// Implementation
protected:
	UINT m_uOS;						// Target OS, windows or Unix
	CString m_csInstallerOptions;   // Installer command line options
	CString m_csServer;				// Communication Server Address
	CStringList m_AgentOtherFiles;	// Other files to copy in agent's directory
	CString m_csAgentSetupFile;		// Agent setup file
	CString m_csAgentDirectory;		// Agent setup directory
	CString m_csLoginName;			// Remote host login
	CString m_csPassword;			// Remote host password
	CString m_csSshKey;				// SSH key for remote host authentication
	CString m_csAgentEtcDirectory;	// Agent etc directory
};

#endif // !defined(AFX_AGENTSETTINGS_H__4EE4E937_ADC9_4346_B54D_68C79E5C7C69__INCLUDED_)
