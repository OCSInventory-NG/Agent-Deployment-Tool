//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// WorkerThreadParam.h: interface for the CWorkerThreadParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKERTHREADPARAM_H__A8A9D32B_B605_4636_B635_B7076F615808__INCLUDED_)
#define AFX_WORKERTHREADPARAM_H__A8A9D32B_B605_4636_B635_B7076F615808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWorkerThreadParam  
{
public:
	CWorkerThreadParam();
	virtual ~CWorkerThreadParam();

	void SetParam( HWND hWnd, CAgentSettings *pSettings, CStringList *pFailed, LPCTSTR lpstrComputer);
	HWND GetHwnd();
	CAgentSettings * GetSettings();
	LPCTSTR GetComputer();
	CStringList * GetFailedList();

protected:
	HWND	m_hWnd;
	CAgentSettings *m_pSettings;
	CStringList *m_pFailed;
	CString m_csComputer;
};

#endif // !defined(AFX_WORKERTHREADPARAM_H__A8A9D32B_B605_4636_B635_B7076F615808__INCLUDED_)
