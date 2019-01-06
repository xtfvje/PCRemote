#pragma once
#include "afxwin.h"

#include "socket/IOCPServer.h"
// CShellDlg 对话框

class CShellDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShellDlg)

public:
	CShellDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // 标准构造函数
	virtual ~CShellDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHELL_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit m_edit_shell;
	CIOCPServer* m_iocpServer;
	ClientContext* m_pContext;
	HICON m_hIcon;	
	UINT m_nCurSel;
	UINT m_nReceiveLength;
public:
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditShell();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void ResizeEdit(void);
	void OnReceiveComplete(void);

private:
	void AddKeyBoardData(void);
};
