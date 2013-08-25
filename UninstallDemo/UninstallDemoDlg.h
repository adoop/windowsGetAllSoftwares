
// UninstallDemoDlg.h : header file
//

#include"vector"

#include "ListCtrlEx.h"
using namespace std;
// CUninstallDemoDlg dialog
class CUninstallDemoDlg : public CDialogEx
{
// Construction
public:
	CUninstallDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UNINSTALLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();


LRESULT OnUpdateListCheckCell(WPARAM wParam, LPARAM lp);
// refresh app listview
void FindApp(void);
};
