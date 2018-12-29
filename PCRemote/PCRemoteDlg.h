
// PCRemoteDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "tools/TrueColorToolBar.h"
#include "socket/IOCPServer.h"

// CPCRemoteDlg �Ի���
class CPCRemoteDlg : public CDialogEx
{
// ����
public:
	CPCRemoteDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PCREMOTE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	CListCtrl m_CList_Online;  // ��������Ӻ���ʾ���б�ؼ�
	CListCtrl m_CList_Message; // ��־�б�ؼ�
	CStatusBar  m_wndStatusBar;  //״̬��
	int m_iCount;
	CTrueColorToolBar m_ToolBar;
	NOTIFYICONDATA m_notifyID; // ����
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

	//�Զ�����Ϣ
	afx_msg void OnIconNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNotifyShow();
	afx_msg void OnNotifyClose();
	afx_msg void OnClose();

protected:
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	void Activate(UINT nPort, UINT nMaxConnections);
};
