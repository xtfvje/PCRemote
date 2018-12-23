
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
	void Test();
private:
	CListCtrl m_CList_Online;  // 服务端连接后显示的列表控件
	CListCtrl m_CList_Message; // 日志列表控件
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickListOnline(NMHDR *pNMHDR, LRESULT *pResult);
};
