
// PCRemoteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PCRemote.h"
#include "PCRemoteDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COLUMNSTRUCT g_Column_Online_Data[] =
{
	{ "IP",				148 },
	{ "区域",			150 },
	{ "计算机名/备注",	160 },
	{ "操作系统",		128 },
	{ "CPU",			80 },
	{ "摄像头",			81 },
	{ "PING",			81 }
};
int g_Column_Online_Width = 0;  //列总宽度

COLUMNSTRUCT g_Column_Message_Data[] =
{
	{ "信息类型",		68 },
	{ "时间",			100 },
	{ "信息内容",	    660 }
};
int g_Column_Message_Width = 0;  //列总宽度

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPCRemoteDlg 对话框



CPCRemoteDlg::CPCRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PCREMOTE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPCRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ONLINE, m_CList_Online);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_CList_Message);
}

BEGIN_MESSAGE_MAP(CPCRemoteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ONLINE, &CPCRemoteDlg::OnNMRClickListOnline)
END_MESSAGE_MAP()


// CPCRemoteDlg 消息处理程序

BOOL CPCRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->InitListCtrl();
	this->Test();

	CRect rect;
	GetWindowRect(&rect);
	rect.bottom += 20;
	MoveWindow(rect);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPCRemoteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPCRemoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPCRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPCRemoteDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	double dcx = cx;     //对话框的总宽度
	if (m_CList_Online.m_hWnd != NULL)
	{
		CRect rc;
		rc.left = 1;       //列表的左坐标
		rc.top = 80;       //列表的上坐标
		rc.right = cx - 1;  //列表的右坐标
		rc.bottom = cy - 160;  //列表的下坐标
		m_CList_Online.MoveWindow(rc);

		for (int i = 0;i < g_Column_Online_Count;i++) //遍历每一个列
		{                   
			double dd = g_Column_Online_Data[i].nWidth;     //得到当前列的宽度
			dd /= g_Column_Online_Width;                    //看一看当前宽度占总长度的几分之几
			dd *= dcx;                                      //用原来的长度乘以所占的几分之几得到当前的宽度
			int len = (int)dd;                                 //转换为int 类型
			m_CList_Online.SetColumnWidth(i, len);      //设置当前的宽度
		}
	}

	if (m_CList_Message.m_hWnd != NULL)
	{
		CRect rc;
		rc.left = 1;        //列表的左坐标
		rc.top = cy - 156;    //列表的上坐标
		rc.right = cx - 1;    //列表的右坐标
		rc.bottom = cy - 6;  //列表的下坐标
		m_CList_Message.MoveWindow(rc);

		for (int i = 0;i < g_Column_Message_Count;i++) //遍历每一个列
		{                   
			double dd = g_Column_Message_Data[i].nWidth;     //得到当前列的宽度
			dd /= g_Column_Message_Width;                    //看一看当前宽度占总长度的几分之几
			dd *= dcx;                                       //用原来的长度乘以所占的几分之几得到当前的宽度
			int lenth = (int)dd;                                  //转换为int 类型
			m_CList_Message.SetColumnWidth(i, lenth);      //设置当前的宽度
		}
	}
}

void CPCRemoteDlg::InitListCtrl()
{
	m_CList_Online.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	for (int i = 0; i < g_Column_Online_Count; i++)
	{
		m_CList_Online.InsertColumn(i, g_Column_Online_Data[i].title, LVCFMT_CENTER, g_Column_Online_Data[i].nWidth);
		g_Column_Online_Width += g_Column_Online_Data[i].nWidth;       //得到总宽度
	}

	m_CList_Message.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	for (int i = 0; i < g_Column_Message_Count; i++)
	{
		m_CList_Message.InsertColumn(i, g_Column_Message_Data[i].title, LVCFMT_CENTER, g_Column_Message_Data[i].nWidth);
		g_Column_Message_Width += g_Column_Message_Data[i].nWidth;       //得到总宽度
	}
}

void CPCRemoteDlg::AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strPing)
{
	m_CList_Online.InsertItem(0, strIP);           //默认为0行  这样所有插入的新列都在最上面
	m_CList_Online.SetItemText(0, ONLINELIST_ADDR, strAddr);      //设置列的显示字符   这里 ONLINELIST_ADDR等 为第二节课中的枚举类型 用这样的方法
	m_CList_Online.SetItemText(0, ONLINELIST_COMPUTER_NAME, strPCName); //解决问题会避免以后扩展时的冲突
	m_CList_Online.SetItemText(0, ONLINELIST_OS, strOS);
	m_CList_Online.SetItemText(0, ONLINELIST_CPU, strCPU);
	m_CList_Online.SetItemText(0, ONLINELIST_VIDEO, strVideo);
	m_CList_Online.SetItemText(0, ONLINELIST_PING, strPing);
}

void CPCRemoteDlg::ShowMessageLog(bool bIsOK, CString strMsg)
{
	CString strIsOK, strTime;
	CTime t = CTime::GetCurrentTime();
	strTime = t.Format("%H:%M:%S");
	if (bIsOK)
	{
		strIsOK = "执行成功";
	}
	else {
		strIsOK = "执行失败";
	}
	m_CList_Message.InsertItem(0, strIsOK);
	m_CList_Message.SetItemText(0, 1, strTime);
	m_CList_Message.SetItemText(0, 2, strMsg);
}

void CPCRemoteDlg::Test()
{
	this->AddList("192.168.0.1", "本机局域网", "zhangxueming", "Windows 10", "2.2GHZ", "有", "123232");
	this->ShowMessageLog(true, "软件初始化成功...");
}

void CPCRemoteDlg::OnNMRClickListOnline(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu	popup;
	popup.LoadMenu(IDR_MENU_ONLINE);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	if (m_CList_Online.GetSelectedCount() == 0)       //如果没有选中
	{
		for (int i = 0; i < count; ++i)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);  //菜单全部变灰
		}

	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
}
