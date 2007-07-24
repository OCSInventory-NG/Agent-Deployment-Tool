//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// AgentSettings.h: interface for the CAgentSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AGENTSETTINGS_H__4EE4E937_ADC9_4346_B54D_68C79E5C7C69__INCLUDED_)
#define AFX_AGENTSETTINGS_H__4EE4E937_ADC9_4346_B54D_68C79E5C7C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_SERVER_ADDRESS	_T( "ocsinventory-ng")
#define DEFAULT_SERVER_PORT		80
#define SETTING_SECTION			_T( "Settings")
#define OPTION_SERVER_ADDRESS	_T( "OCS Inventory NG Communication Server Address")
#define OPTION_SERVER_PORT		_T( "OCS Inventory NG Communication Server Port")
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
	virtual void SetAgentOthersOptions( CString csOptions);
	virtual void SetDebugEnabled( BOOL bDebug = TRUE);
	virtual void SetProxyDisabled( BOOL bDisabled = TRUE);
	virtual void SetLaunchNowRequired( BOOL bLaunch = TRUE);
	virtual void SetServerAddress( CString csServer = DEFAULT_SERVER_ADDRESS);
	virtual void SetServerPort( UINT uPort = DEFAULT_SERVER_PORT);
	virtual void SetAgentSetupDirectory( CString csDirectory);
	virtual void SetTagValue( CString csTag);
	virtual void SetDaemonEnabled( BOOL bDaemon = TRUE);

	// Getter
	virtual UINT GetTargetOS();
	virtual LPCTSTR GetLoginName();
	virtual BOOL IsLoginNameAvailable();
	virtual LPCTSTR GetPassword();
	virtual BOOL IsPasswordAvailable();
	virtual LPCTSTR GetSshKey();
	virtual BOOL IsSshKeyAvailable();
	virtual CString GetAgentSetupDirectory();
	virtual UINT GetServerPort();
	virtual CString GetServerAddress();
	virtual BOOL IsLaunchNowRequired();
	virtual BOOL IsProxyDisabled();
	virtual BOOL IsDebugEnabled();
	virtual CString GetAgentOthersOptions();
	virtual CStringList * GetAgentOtherFiles();
	virtual CString GetAgentSetupFile();
	virtual BOOL IsDaemonEnabled();
	virtual CString GetTagValue();

// Implementation
protected:
	UINT m_uOS;						// Target OS, windows or Unix
	BOOL m_bLaunchNow;				// Launch inventory whe setup successfull
	CString m_csAgentOtherOptions;	// Windows Agent command line options
	BOOL m_bDisableProxy;			// Disable use of IE proxy settings
	BOOL m_bDebug;					// Enable log files creation 
	UINT m_uPort;					// Communication Server Port
	CString m_csServer;				// Communication Server Address
	CStringList m_AgentOtherFiles;	// Other files to copy in agent's directory
	CString m_csAgentSetupFile;		// Agent setup file
	CString m_csAgentDirectory;		// Agent setup directory
	CString m_csLoginName;			// Remote host login
	CString m_csPassword;			// Remote host password
	CString m_csSshKey;				// SSH key for remote host authentication
	CString m_csTag;				// Agent's tag value
	BOOL	m_bDaemon;				// Setup agent as a daemon
};

#endif // !defined(AFX_AGENTSETTINGS_H__4EE4E937_ADC9_4346_B54D_68C79E5C7C69__INCLUDED_)
