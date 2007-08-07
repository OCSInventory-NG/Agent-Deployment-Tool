//====================================================================================
// Open Computer and Software Inventory
// Copyleft Didier LIROULET 2007
// Web: http://ocsinventory.sourceforge.net

// This code is open source and may be copied and modified as long as the source
// code is always made freely available.
// Please refer to the General Public Licence http://www.gnu.org/ or Licence.txt
//====================================================================================

// SelectHostsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OCS_DEPLOY_TOOL.h"
#include "AgentSettings.h"
#include "SelectHostsDlg.h"
#include "UnixHostAddressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectHostsDlg dialog


CSelectHostsDlg::CSelectHostsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectHostsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectHostsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectHostsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectHostsDlg)
	DDX_Control(pDX, IDC_LIST_COMPUTERS, m_List);
	DDX_Control(pDX, IDC_IPADDRESS_TO, m_IpTo);
	DDX_Control(pDX, IDC_IPADDRESS_FROM, m_IpFrom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectHostsDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectHostsDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	ON_BN_CLICKED(IDC_RADIO_IP, OnRadioIp)
	ON_BN_CLICKED(IDC_RADIO_LIST, OnRadioList)
	ON_BN_CLICKED(ID_WIZNEXT, OnWiznext)
	ON_LBN_DBLCLK(IDC_LIST_COMPUTERS, OnDblclkListComputers)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_EXE, OnButtonSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectHostsDlg message handlers

BOOL CSelectHostsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	try
	{
		CString	csMessage;
		POSITION pos;
		
		csMessage.LoadString( IDS_OCS_DEPLOY_TOOL);
		SetDlgItemText( IDC_STATUS, csMessage);
		if (m_pComputerList->IsEmpty())
		{
			// By default, enable IP range
			CheckDlgButton( IDC_RADIO_IP, TRUE);
			GetDlgItem( IDC_IPADDRESS_FROM)->EnableWindow( TRUE);
			GetDlgItem( IDC_IPADDRESS_TO)->EnableWindow( TRUE);
			// Disable List of computers
			CheckDlgButton( IDC_RADIO_LIST, FALSE);
			GetDlgItem( IDC_LIST_COMPUTERS)->EnableWindow( FALSE);
			GetDlgItem( IDC_BUTTON_ADD)->EnableWindow( FALSE);
			GetDlgItem( IDC_BUTTON_REMOVE)->EnableWindow( FALSE);
			GetDlgItem( IDC_BUTTON_IMPORT)->EnableWindow( FALSE);
			GetDlgItem( IDC_BUTTON_EXE)->EnableWindow( FALSE);
		}
		else
		{
			// Disable IP range
			CheckDlgButton( IDC_RADIO_IP, FALSE);
			GetDlgItem( IDC_IPADDRESS_FROM)->EnableWindow( FALSE);
			GetDlgItem( IDC_IPADDRESS_TO)->EnableWindow( FALSE);
			// Enable List of computers
			CheckDlgButton( IDC_RADIO_LIST, TRUE);
			GetDlgItem( IDC_LIST_COMPUTERS)->EnableWindow( TRUE);
			GetDlgItem( IDC_BUTTON_ADD)->EnableWindow( TRUE);
			GetDlgItem( IDC_BUTTON_REMOVE)->EnableWindow( TRUE);
			GetDlgItem( IDC_BUTTON_IMPORT)->EnableWindow( TRUE);
			GetDlgItem( IDC_BUTTON_EXE)->EnableWindow( TRUE);
			// Populate computer list with previously selected computers
			pos = m_pComputerList->GetHeadPosition();
			while (pos)
				m_List.AddString( m_pComputerList->GetNext( pos));
		}
		// Limit IP Address Control field range
		m_IpFrom.SetFieldRange( 0, 0, MAX_IP_RANGE);
		m_IpFrom.SetFieldRange( 1, 0, MAX_IP_RANGE);
		m_IpFrom.SetFieldRange( 2, 0, MAX_IP_RANGE);
		m_IpFrom.SetFieldRange( 3, 0, MAX_IP_RANGE);
		m_IpTo.SetFieldRange( 0, 0, MAX_IP_RANGE);
		m_IpTo.SetFieldRange( 1, 0, MAX_IP_RANGE);
		m_IpTo.SetFieldRange( 2, 0, MAX_IP_RANGE);
		m_IpTo.SetFieldRange( 3, 0, MAX_IP_RANGE);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return 1;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectHostsDlg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	switch( m_uOS)
	{
	case AGENT_OS_WINDOWS:
		AddWindowsComputer();
		break;
	case AGENT_OS_UNIX:
	default:
		AddUnixComputer();
		break;
	}
}

void CSelectHostsDlg::OnButtonRemove() 
{
	// TODO: Add your control notification handler code here
	int		nSelCount;

	try
	{
		if (((nSelCount = m_List.GetSelCount()) == LB_ERR) || (nSelCount == 0))
			// No item selected
			return;
		for (int i=0; i<m_List.GetCount(); i++)
			if (m_List.GetSel( i) > 0)
			{
				m_List.DeleteString( i);
				// Stay at same index to avoid skipping item which take index of deleted
				i--;
			}
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void CSelectHostsDlg::OnRadioIp() 
{
	// TODO: Add your control notification handler code here
	try
	{
		// Enable IP range
		CheckDlgButton( IDC_RADIO_IP, TRUE);
		GetDlgItem( IDC_IPADDRESS_FROM)->EnableWindow( TRUE);
		GetDlgItem( IDC_IPADDRESS_TO)->EnableWindow( TRUE);
		// Disable List of computers
		CheckDlgButton( IDC_RADIO_LIST, FALSE);
		GetDlgItem( IDC_LIST_COMPUTERS)->EnableWindow( FALSE);
		GetDlgItem( IDC_BUTTON_ADD)->EnableWindow( FALSE);
		GetDlgItem( IDC_BUTTON_REMOVE)->EnableWindow( FALSE);
		GetDlgItem( IDC_BUTTON_IMPORT)->EnableWindow( FALSE);
		GetDlgItem( IDC_BUTTON_EXE)->EnableWindow( FALSE);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void CSelectHostsDlg::OnRadioList() 
{
	// TODO: Add your control notification handler code here
	try
	{
		// Disable IP range
		CheckDlgButton( IDC_RADIO_IP, FALSE);
		GetDlgItem( IDC_IPADDRESS_FROM)->EnableWindow( FALSE);
		GetDlgItem( IDC_IPADDRESS_TO)->EnableWindow( FALSE);
		// Enable List of computers
		CheckDlgButton( IDC_RADIO_LIST, TRUE);
		GetDlgItem( IDC_LIST_COMPUTERS)->EnableWindow( TRUE);
		GetDlgItem( IDC_BUTTON_ADD)->EnableWindow( TRUE);
		GetDlgItem( IDC_BUTTON_REMOVE)->EnableWindow( TRUE);
		GetDlgItem( IDC_BUTTON_IMPORT)->EnableWindow( TRUE);
		GetDlgItem( IDC_BUTTON_EXE)->EnableWindow( TRUE);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void CSelectHostsDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void CSelectHostsDlg::SetComputerList(CStringList *pList, UINT uOS)
{
	m_pComputerList = pList;
	m_uOS = uOS;
}

void CSelectHostsDlg::OnWiznext() 
{
	// TODO: Add your control notification handler code here
	try
	{
		CString csMessage;

		if (IsDlgButtonChecked( IDC_RADIO_IP))
		{
			// IP address range
			BYTE nIpFromA, nIpFromB, nIpFromC, nIpFromD, nIpToA, nIpToB, nIpToC, nIpToD;

			if (m_IpFrom.GetAddress( nIpFromA, nIpFromB, nIpFromC, nIpFromD) != 4)
			{
				// No data filled in
				AfxMessageBox( IDS_ERROR_INVALID_VALUE, MB_ICONINFORMATION);
				GetDlgItem( IDC_IPADDRESS_FROM)->SetFocus();
				return;
			}
			if (m_IpTo.GetAddress( nIpToA, nIpToB, nIpToC, nIpToD) != 4)
			{
				// No data filled in
				AfxMessageBox( IDS_ERROR_INVALID_VALUE, MB_ICONINFORMATION);
				GetDlgItem( IDC_IPADDRESS_TO)->SetFocus();
				return;
			}
			// Empty current computer list
			m_pComputerList->RemoveAll();
			// Fill in computer list with each IP addresses in range
			if (!AddNetwork( nIpFromA, nIpFromB, nIpFromC, nIpFromD, nIpToA, nIpToB, nIpToC, nIpToD))
				return;
		}
		else
		{
			// Computer list
			int nIndex, nCount = 0;
			CString csComputer;

			if (((nCount = m_List.GetCount()) == LB_ERR) || (nCount == 0))
			{
				// No items in List
				AfxMessageBox( IDS_ERROR_INVALID_VALUE, MB_ICONINFORMATION);
				return;
			}
			// Empty current computer list
			m_pComputerList->RemoveAll();
			// Fill in computer list with each computer from list
			for (nIndex = 0; nIndex < nCount; nIndex++)
			{
				m_List.GetText( nIndex, csComputer);
				m_pComputerList->AddTail( csComputer);
			}
		}
		EndDialog( ID_WIZNEXT);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void CSelectHostsDlg::OnDblclkListComputers() 
{
	// TODO: Add your control notification handler code here
	OnButtonRemove();
}

void CSelectHostsDlg::OnButtonSelectAll() 
{
	// TODO: Add your control notification handler code here
	try
	{
		for (int i=0; i<m_List.GetCount(); i++)
			m_List.SetSel( i);
		m_List.UpdateWindow();
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

void CSelectHostsDlg::OnButtonImport() 
{
	// TODO: Add your control notification handler code here
	LPITEMIDLIST	pMyIdList = NULL;
	LPMALLOC		pIMalloc;
	CStdioFile		myFile;
	
	try
	{
		CFileDialog		dlgOpenFile( TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "CSV Files|*.csv|All files|*.*||"));
		TCHAR			szInitialFolder[4*_MAX_PATH+1];
		CString			csMessage,
						csComputer;
		int				nIndex;

		// Get User Desktop path
		if (SHGetMalloc( &pIMalloc) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			return;
		}
		if (SHGetSpecialFolderLocation( m_hWnd, CSIDL_DESKTOP, &pMyIdList) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			return;
		}
		if (!SHGetPathFromIDList( pMyIdList, szInitialFolder))
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			pIMalloc->Free( pMyIdList);
			return;
		}
		pIMalloc->Free( pMyIdList);
		pMyIdList = NULL;
		// Fill in the OPENFILENAME structure to support a template and hook.
		dlgOpenFile.m_ofn.lpstrInitialDir   = szInitialFolder;
		if (!csMessage.LoadString( IDS_SELECT_CSV_FILE))
			AfxThrowMemoryException();
		dlgOpenFile.m_ofn.lpstrTitle        = csMessage.GetBuffer( csMessage.GetLength());
		if (dlgOpenFile.DoModal() != IDOK)
		{
			// Cancel
			return;
		}
		if (!myFile.Open( dlgOpenFile.GetPathName(), CFile::modeRead|CFile::typeText))
		{
			csMessage.FormatMessage( IDS_ERROR_OPENING_FILE, dlgOpenFile.GetPathName());
			AfxMessageBox( csMessage, MB_ICONSTOP|MB_OK);
			return;
		}
		// Read first header line and discard
		myFile.ReadString( csMessage);
		// Read next line and get first field as computer name or IP address
		while (myFile.ReadString( csMessage))
		{
			if ((nIndex = csMessage.Find( _T( ";"))) == -1)
			{
				// No ; as separator
				if ((nIndex = csMessage.Find( _T( ","))) == -1)
				{
					// No , as separator, so only one field
					csComputer = csMessage;
				}
				else
					// Get IP on first field
					csComputer = csMessage.Left( nIndex);
			}
			else
				// Get IP on first field
				csComputer = csMessage.Left( nIndex);
			// Add computer to list if not address not empty
			if (!csComputer.IsEmpty())
				m_List.AddString( csComputer);
		}
		myFile.Close();
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		if (pMyIdList != NULL)
			pIMalloc->Free( pMyIdList);
		myFile.Abort();
		return;
	}
}

void CSelectHostsDlg::AddWindowsComputer()
{
	LPITEMIDLIST	lpiFolder = NULL;				
	LPMALLOC		pIMalloc;

	try
	{
		TCHAR			szFolder[4*_MAX_PATH+1];	
		CString			csMessage;
		BROWSEINFO		biFolder;				
		LPSHELLFOLDER	lpsfDestopFolder;

		if (SHGetMalloc( &pIMalloc) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			return;
		}
		if (SHGetDesktopFolder( &lpsfDestopFolder) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			return;
		}
		if (SHGetSpecialFolderLocation( m_hWnd, CSIDL_NETWORK, &lpiFolder) != NOERROR)
		{
			AfxMessageBox( IDS_ERROR_DISPLAY_FOLDER, MB_ICONSTOP);
			lpsfDestopFolder->Release();
			return;
		}
		biFolder.hwndOwner = m_hWnd;
		// Start in network folder
		biFolder.pidlRoot = lpiFolder;
		// Name of the selected folder
		biFolder.pszDisplayName = szFolder;
		// Title of the action
		csMessage.LoadString( IDS_SELECT_REMOTE_HOST);
		biFolder.lpszTitle = (LPTSTR) LPCTSTR( csMessage);;
		// Browse only for real computers
		biFolder.ulFlags = BIF_BROWSEFORCOMPUTER|BIF_EDITBOX;
		biFolder.lpfn = NULL;
		biFolder.lParam = 0L;
		// Show select folder Dialog Box
		if ((lpiFolder = SHBrowseForFolder( &biFolder)) == NULL)
		{
			// Cancel or host not found
			AfxMessageBox( IDS_ERROR_HOST_NOT_FOUND, MB_OK|MB_ICONEXCLAMATION);
			pIMalloc->Free( lpiFolder);
			lpsfDestopFolder->Release();
			return;
		}
		pIMalloc->Free( lpiFolder);
		lpiFolder = NULL;
		lpsfDestopFolder->Release();
		csMessage = szFolder;
		csMessage.Remove( '\\');
		m_List.AddString( csMessage);
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		if (lpiFolder != NULL)
			pIMalloc->Free( lpiFolder);
		return;
	}
}

void CSelectHostsDlg::AddUnixComputer()
{
	try
	{
		CUnixHostAddressDlg cDlg;

		if (cDlg.DoModal() == IDOK)
			m_List.AddString( cDlg.GetHost());
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return;
	}
}

BOOL CSelectHostsDlg::AddNetwork(BYTE nIpFromA, BYTE nIpFromB, BYTE nIpFromC, BYTE nIpFromD, BYTE nIpToA, BYTE nIpToB, BYTE nIpToC, BYTE nIpToD)
{
	try
	{
		BYTE	n;
		CString csAddress;

		if (nIpFromA == nIpToA)
		{
			// Address range is inside a single class A network
			if (nIpFromB == nIpToB)
			{
				// Address range is inside a single class B network
				if (nIpFromC == nIpToC)
				{
					// Address range is inside a single class C network
					for (n=nIpFromD; n<=nIpToD; n++)
					{
						csAddress.Format( _T( "%u.%u.%u.%u"), nIpFromA, nIpFromB, nIpFromC, n);
						m_pComputerList->AddTail( csAddress);
					}
					return TRUE;
				}
				// Address range is over mulitple class C networks
				for (n=nIpFromC; n<=nIpToC; n++)
				{
					if (n==nIpFromC)
					{
						// First class C, add only from first specified address
						if (!AddNetwork( nIpFromA, nIpFromB, n, nIpFromD, nIpFromA, nIpFromB, n, MAX_IP_RANGE))
							return FALSE;
					}
					else
					{
						if (n==nIpToC)
						{
							// Last class C, add only to last specified address
							if (!AddNetwork( nIpFromA, nIpFromB, n, 0, nIpFromA, nIpFromB, n, nIpToD))
								return FALSE;
						}
						else
						{
							// Full class C
							if (!AddNetwork( nIpFromA, nIpFromB, n, 0, nIpFromA, nIpFromB, n, MAX_IP_RANGE))
								return FALSE;
						}
					}
				}
				return TRUE;
			}
			// Address range is over mulitple class B networks
			for (n=nIpFromB; n<=nIpToB; n++)
			{
				if (n==nIpFromB)
				{
					// First class B, add only from first specified address
					if (!AddNetwork( nIpFromA, n, nIpFromC, nIpFromD, nIpFromA, n, MAX_IP_RANGE, MAX_IP_RANGE))
						return FALSE;
				}
				else
				{
					if (n==nIpToB)
					{
						// Last class B, add only to last specified address
						if (!AddNetwork( nIpFromA, n, 0, 0, nIpFromA, n, nIpToC, nIpToD))
							return FALSE;
					}
					else
					{
						// Full class B
						if (!AddNetwork( nIpFromA, n, 0, 0, nIpFromA, n, MAX_IP_RANGE, MAX_IP_RANGE))
							return FALSE;
					}
				}
			}
			return TRUE;
		}
		// Address range is over mulitple class A networks
		for (n=nIpFromA; n<=nIpToA; n++)
		{
			if (n==nIpFromA)
			{
				// First class A, add only from first specified address
				if (!AddNetwork( n, nIpFromB, nIpFromC, nIpFromD, n, MAX_IP_RANGE, MAX_IP_RANGE, MAX_IP_RANGE))
					return FALSE;
			}
			else
			{
				if (n==nIpToA)
				{
					// Last class A, add only to last specified address
					if (!AddNetwork( n, 0, 0, 0, n, nIpToB, nIpToC, nIpToD))
						return FALSE;
				}
				else
				{
					// Full class A
					if (!AddNetwork( n, 0, 0, 0, n, MAX_IP_RANGE, MAX_IP_RANGE, MAX_IP_RANGE))
						return FALSE;
				}
			}
		}
		return TRUE;
	}
	catch( CException *pEx)
	{
		pEx->ReportError( MB_OK|MB_ICONSTOP);
		pEx->Delete();
		return FALSE;
	}
}
