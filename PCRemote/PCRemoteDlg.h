
// PCRemoteDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


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
private:
	CListCtrl m_CList_Online;  // 服务端连接后显示的列表控件
	CListCtrl m_CList_Message; // 日志列表控件
	CStatusBar  m_wndStatusBar;  //状态条
	int m_iCount;
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
};
