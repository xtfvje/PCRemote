#pragma once
#include "afxcmn.h"
#include "socket/IOCPServer.h"

// CSystemDlg 对话框

class CSystemDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemDlg)

public:
	CSystemDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // 标准构造函数
	virtual ~CSystemDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SYSTEM_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabSys(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnProcesslistKillProcess();
	afx_msg void OnProcesslistRefreshProcesslist();
	afx_msg void OnNMRClickListSysProcess(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
private:
	CTabCtrl m_sysTab;
	CListCtrl m_list_SysWindows;
	CListCtrl m_list_sysProcess;
	
	CIOCPServer* m_iocpServer;
	ClientContext* m_pContext;
	HICON m_hIcon;
	
private:
	void initDialog();
	void AdjustList(void);
	void ShowSelectWindow(void);
	void GetProcessList(void);
	void ShowProcessList(void);
public:
	void OnReceiveComplete(void);	
};
