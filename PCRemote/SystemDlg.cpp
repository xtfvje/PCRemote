// SystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PCRemote.h"
#include "SystemDlg.h"
#include "afxdialogex.h"
#include "../thirdpart/macros.h"

// CSystemDlg �Ի���

IMPLEMENT_DYNAMIC(CSystemDlg, CDialogEx)

CSystemDlg::CSystemDlg(CWnd* pParent /*= NULL*/, CIOCPServer* pIOCPServer /*= NULL*/, ClientContext *pContext /*= NULL*/)
	: CDialogEx(IDD_SYSTEM_DIALOG, pParent), m_iocpServer(pIOCPServer), m_pContext(pContext)
{
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SYSTEM));
}

CSystemDlg::~CSystemDlg()
{
}

void CSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SYS, m_sysTab);
	DDX_Control(pDX, IDC_LIST_SYS_WINDOWS, m_list_SysWindows);
	DDX_Control(pDX, IDC_LIST_SYS_PROCESS, m_list_sysProcess);
}


BEGIN_MESSAGE_MAP(CSystemDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SYS, &CSystemDlg::OnTcnSelchangeTabSys)
	ON_COMMAND(ID_PROCESSLIST_KILL_PROCESS, &CSystemDlg::OnProcesslistKillProcess)
	ON_COMMAND(ID_PROCESSLIST_REFRESH_PROCESSLIST, &CSystemDlg::OnProcesslistRefreshProcesslist)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SYS_PROCESS, &CSystemDlg::OnNMRClickListSysProcess)
END_MESSAGE_MAP()


// CSystemDlg ��Ϣ�������
BOOL CSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	initDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CSystemDlg::initDialog()
{
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen); //�õ����ӵ�ip 
	str.Format("\\\\%s - ϵͳ����", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);//���öԻ������

	m_sysTab.InsertItem(0, "���̹���");    //Ϊtab���ñ���
	m_sysTab.InsertItem(1, "���ڹ���");
//	m_sysTab.InsertItem(2, "��������");

	m_list_sysProcess.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);  //��ʼ�����̵��б�
	m_list_sysProcess.InsertColumn(0, "ӳ������", LVCFMT_LEFT, 100);
	m_list_sysProcess.InsertColumn(1, "PID", LVCFMT_LEFT, 50);
	m_list_sysProcess.InsertColumn(2, "����·��", LVCFMT_LEFT, 400);

	m_list_SysWindows.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);  //��ʼ�� ���ڹ�����б�
	m_list_SysWindows.InsertColumn(0, "PID", LVCFMT_LEFT, 50);
	m_list_SysWindows.InsertColumn(1, "��������", LVCFMT_LEFT, 300);


	AdjustList();       //�����б�Ĵ�С
	ShowProcessList();   //���ڵ�һ������������Ϣ��������Ž��̵��������԰�������ʾ���б���
	ShowSelectWindow();   //��ʾ�б�
}

void CSystemDlg::ShowProcessList(void)
{
	char	*lpBuffer = (char *)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	char	*strExeFile;
	char	*strProcessName;
	DWORD	dwOffset = 0;
	CString str;
	m_list_sysProcess.DeleteAllItems();
	//������������ÿһ���ַ��������������ݽṹ�� Id+������+0+������+0
	int i = 0;
	UINT bufLen = m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1;
	for (i = 0; dwOffset < bufLen; ++i)
	{
		LPDWORD	lpPID = LPDWORD(lpBuffer + dwOffset);        //����õ�����ID
		strExeFile = lpBuffer + dwOffset + sizeof(DWORD);      //����������ID֮�����
		int strExeFileLen = lstrlen(strExeFile);
		strProcessName = strExeFile + strExeFileLen + 1;  //���������ǽ�����֮�����
		//�������ݽṹ�Ĺ���������
		m_list_sysProcess.InsertItem(i, strExeFile);       //���õ������ݼ��뵽�б���
		str.Format("%5u", *lpPID);
		m_list_sysProcess.SetItemText(i, 1, str);
		m_list_sysProcess.SetItemText(i, 2, strProcessName);
		// ItemData Ϊ����ID
		m_list_sysProcess.SetItemData(i, *lpPID);

		dwOffset += sizeof(DWORD) + strExeFileLen + lstrlen(strProcessName) + 2;   //����������ݽṹ ������һ��ѭ��
	}

	str.Format("����·�� / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_list_sysProcess.SetColumn(2, &lvc); //���б�����ʾ�ж��ٸ�����
}

void CSystemDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);

	CDialogEx::OnClose();
}

void CSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	AdjustList();
}

void CSystemDlg::AdjustList(void)
{
	if (NULL == m_list_SysWindows.m_hWnd || NULL == m_list_sysProcess.m_hWnd)
	{
		return;
	}

	RECT	rectClient;
	RECT	rectList;
	GetClientRect(&rectClient);
	rectList.left = 0;
	rectList.top = 29;
	rectList.right = rectClient.right;
	rectList.bottom = rectClient.bottom;

	m_list_sysProcess.MoveWindow(&rectList);
	m_list_SysWindows.MoveWindow(&rectList);
}


void CSystemDlg::OnTcnSelchangeTabSys(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowSelectWindow();

	*pResult = 0;
}

void CSystemDlg::ShowSelectWindow(void)
{
	switch (m_sysTab.GetCurSel())
	{
	case 0:
	{
		m_list_SysWindows.ShowWindow(SW_HIDE);
		m_list_sysProcess.ShowWindow(SW_SHOW);
		if (m_list_sysProcess.GetItemCount() == 0)
		{
			GetProcessList();
		}
	}
		break;
	case 1:
	{
		m_list_sysProcess.ShowWindow(SW_HIDE);
		m_list_SysWindows.ShowWindow(SW_SHOW);		
		if (m_list_SysWindows.GetItemCount() == 0)
		{
			//GetWindowsList();
		}		
	}	
			break;
	default:
		break;
	}
}

void CSystemDlg::GetProcessList(void)
{
	BYTE bToken = COMMAND_PSLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}




void CSystemDlg::OnProcesslistKillProcess()
{
	// TODO: �ڴ���������������
	CListCtrl	*pListCtrl = NULL;
	if (m_list_sysProcess.IsWindowVisible())
	{
		pListCtrl = &m_list_sysProcess;
	}		
	else if (m_list_SysWindows.IsWindowVisible())
	{
		pListCtrl = &m_list_SysWindows;
	}		
	else
	{
		return;
	}		

	// TODO: Add your command handler code here
	//���仺����
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + (pListCtrl->GetSelectedCount() * 4));
	//����������̵�����ͷ
	lpBuffer[0] = COMMAND_KILLPROCESS;
	//��ʾ������Ϣ
	char *lpTips = "����: ��ֹ���̻ᵼ�²�ϣ�������Ľ����\n"
		"�������ݶ�ʧ��ϵͳ���ȶ����ڱ���ֹǰ��\n"
		"���̽�û�л��ᱣ����״̬�����ݡ�";
	CString str;
	if (pListCtrl->GetSelectedCount() > 1)
	{
		str.Format("%sȷʵ\n����ֹ��%d�������?", lpTips, pListCtrl->GetSelectedCount());
	}
	else
	{
		str.Format("%sȷʵ\n����ֹ���������?", lpTips);
	}
	if (::MessageBox(m_hWnd, str, "���̽�������", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}		

	DWORD	dwOffset = 1;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	//�õ�Ҫ�����ĸ�����
	while (pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = pListCtrl->GetNextSelectedItem(pos);
		DWORD dwProcessID = pListCtrl->GetItemData(nItem);
		memcpy(lpBuffer + dwOffset, &dwProcessID, sizeof(DWORD));
		dwOffset += sizeof(DWORD);
	}
	//�������ݵ�����˵�����˲���COMMAND_KILLPROCESS�������ͷ
	m_iocpServer->Send(m_pContext, lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}


void CSystemDlg::OnProcesslistRefreshProcesslist()
{
	// TODO: �ڴ���������������
	if (m_list_sysProcess.IsWindowVisible())
	{
		GetProcessList();
	}
		
	if (m_list_SysWindows.IsWindowVisible())
	{
//		GetWindowsList();
	}	
}


void CSystemDlg::OnNMRClickListSysProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu	popup;
	popup.LoadMenu(IDR_MENU_PROCESS);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

	*pResult = 0;
}

void CSystemDlg::OnReceiveComplete(void)
{
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_PSLIST:
		ShowProcessList();
		break;
		//case TOKEN_WSLIST:
		//ShowWindowsList();
		//break;
		//case TOKEN_DIALUPASS:
		//ShowDialupassList();
		//break;
	default:
		// ���䷢���쳣����
		break;
	}
}