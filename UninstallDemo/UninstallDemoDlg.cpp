
// UninstallDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UninstallDemo.h"
#include "UninstallDemoDlg.h"
#include"GetAllOSSoft.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

	// show get list soft  view
ListCtrlEx::CListCtrlEx  g_listUninstall;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUninstallDemoDlg dialog




CUninstallDemoDlg::CUninstallDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUninstallDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUninstallDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	 DDX_Control(pDX, IDC_List1, g_listUninstall);
}

BEGIN_MESSAGE_MAP(CUninstallDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CUninstallDemoDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_ListCtrlEx_CHECKCELL,OnUpdateListCheckCell)
	

END_MESSAGE_MAP()


// CUninstallDemoDlg message handlers

BOOL CUninstallDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	   SetWindowTextW(L"Uninstall demo");
    

    g_listUninstall.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    g_listUninstall.SetExtendedStyle(g_listUninstall.GetExtendedStyle() | LVS_EX_FULLROWSELECT );
    g_listUninstall.InsertColumn(0, _T("Select"), 100, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    g_listUninstall.InsertColumn(1, _T("App and Driver Name"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    g_listUninstall.InsertColumn(2, _T("Unstall"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    g_listUninstall.SetSupportSort(FALSE);
    FindApp();
   
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUninstallDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUninstallDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUninstallDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CUninstallDemoDlg::OnUpdateListCheckCell(WPARAM wParam, LPARAM lp)
{

    OutputDebugString(L"WM_ListCtrlEx_CHECKCELL  \r\n");
    ListCtrlEx::CheckCellMsg *msgCell =(ListCtrlEx::CheckCellMsg *)lp;
    //g_listUninstall.GetCellChecked(,);
    if(msgCell->m_bChecked==1)
    {
        for (int i = 0; i < g_listUninstall.GetItemCount(); i++)
        {
            if (g_listUninstall.GetCellChecked(i, 0))
            {
                if(i!=msgCell->m_nRow)
                {
                    OutputDebugString(L"set  \r\n");
                    g_listUninstall.SetCellChecked(i,0,0);
                }

            }
        }
    }

    return 1;
}


// selected one unistall
void CUninstallDemoDlg::OnBnClickedButton1()
{
	CString m_strUnInstallName = L"";
    CString  m_strUninstallString = L"";
	bool bChoose = false;
    if(g_listUninstall.GetItemCount())
    {
        for (int i = g_listUninstall.GetItemCount() - 1; i >= 0; i --)
        {
            if (g_listUninstall.GetCellChecked(i, 0))
            {   
               bChoose = true;
                m_strUnInstallName = g_listUninstall.GetItemText(i, 1);
                m_strUninstallString = g_listUninstall.GetItemText(i, 2);

                TCHAR lpCmdLine[MAX_PATH] = _T("");
                //add %program file% check if have change path and replace.

                if(m_strUninstallString.Find(L"%LOCALAPPDATA%")!=-1)
                {
                    TCHAR cSMPath[200];
                    CString strTemp;
                    if(SHGetSpecialFolderPath(NULL, cSMPath, CSIDL_LOCAL_APPDATA, FALSE))
                    {
                        CString csTemp(cSMPath);
                        strTemp = csTemp;
                    }
                    m_strUninstallString.Replace(L"%LOCALAPPDATA%",strTemp);
                }
                if(m_strUninstallString.Find(L"%ProgramFiles%")!=-1)
                {
                    m_strUninstallString.Replace(L"%ProgramFiles%",L"C:\\Program Files");
                }
                if(m_strUnInstallName.Find(L"AMD Catalyst Install Manager")!=-1)
                {
                    int nFst=m_strUninstallString.ReverseFind('}');
                    CString csTemp = m_strUninstallString.Left(nFst);
                    int nScnd=m_strUninstallString.ReverseFind('{');
                    CString csTemp2 = m_strUninstallString.Left(nScnd);
                    csTemp.Replace(csTemp2+L"{",L"");

                    m_strUninstallString=L"msiexec  /x{"+csTemp+L"}";
                }
                wcscpy(lpCmdLine, m_strUninstallString);
                STARTUPINFO si = {sizeof(si)};
                si.dwFlags |= STARTF_USESHOWWINDOW;
                si.wShowWindow = SW_SHOW;
                PROCESS_INFORMATION pi;
                //need study function

                CreateProcessW(NULL, lpCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
                //Shellex
             
              
                FindApp();
                break;
			}
        }

    }
    else
    {
        MessageBox(_T("No App or Driver is uninstalling!"));
    }
	if(!bChoose){
	MessageBox(_T("No App or Driver is choosing!"));
	}
	
}





// refresh app listview
void CUninstallDemoDlg::FindApp(void)
{
	 g_listUninstall.DeleteAllItems();
    CGetAllOSSoft cGetSoft;

    vector<INSTALLEDINFO> v_all_soft = cGetSoft.GetAllSofts();
    for(int nSoftIndex = 0; nSoftIndex<v_all_soft.size(); nSoftIndex++)
    {

        g_listUninstall.InsertItem(nSoftIndex, _T(""));
        g_listUninstall.SetItemText(nSoftIndex, 1, v_all_soft[nSoftIndex].csDisPlayName);
        g_listUninstall.SetItemText(nSoftIndex, 2, v_all_soft[nSoftIndex].csUnInstallString);

    }
    g_listUninstall.SetColumnWidth(0, 50);
    g_listUninstall.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
    g_listUninstall.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
   

}
