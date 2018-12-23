
// PCRemoteDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


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
	void Test();
private:
	CListCtrl m_CList_Online;  // ��������Ӻ���ʾ���б�ؼ�
	CListCtrl m_CList_Message; // ��־�б�ؼ�
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMRClickListOnline(NMHDR *pNMHDR, LRESULT *pResult);
};
