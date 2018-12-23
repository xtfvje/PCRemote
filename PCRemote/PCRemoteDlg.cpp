
// PCRemoteDlg.cpp : ʵ���ļ�
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
	{ "����",			150 },
	{ "�������/��ע",	160 },
	{ "����ϵͳ",		128 },
	{ "CPU",			80 },
	{ "����ͷ",			81 },
	{ "PING",			81 }
};
int g_Column_Online_Width = 0;  //���ܿ��

COLUMNSTRUCT g_Column_Message_Data[] =
{
	{ "��Ϣ����",		68 },
	{ "ʱ��",			100 },
	{ "��Ϣ����",	    660 }
};
int g_Column_Message_Width = 0;  //���ܿ��

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPCRemoteDlg �Ի���



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


// CPCRemoteDlg ��Ϣ�������

BOOL CPCRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	this->InitListCtrl();
	this->Test();

	CRect rect;
	GetWindowRect(&rect);
	rect.bottom += 20;
	MoveWindow(rect);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPCRemoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPCRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPCRemoteDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	double dcx = cx;     //�Ի�����ܿ��
	if (m_CList_Online.m_hWnd != NULL)
	{
		CRect rc;
		rc.left = 1;       //�б��������
		rc.top = 80;       //�б��������
		rc.right = cx - 1;  //�б��������
		rc.bottom = cy - 160;  //�б��������
		m_CList_Online.MoveWindow(rc);

		for (int i = 0;i < g_Column_Online_Count;i++) //����ÿһ����
		{                   
			double dd = g_Column_Online_Data[i].nWidth;     //�õ���ǰ�еĿ��
			dd /= g_Column_Online_Width;                    //��һ����ǰ���ռ�ܳ��ȵļ���֮��
			dd *= dcx;                                      //��ԭ���ĳ��ȳ�����ռ�ļ���֮���õ���ǰ�Ŀ��
			int len = (int)dd;                                 //ת��Ϊint ����
			m_CList_Online.SetColumnWidth(i, len);      //���õ�ǰ�Ŀ��
		}
	}

	if (m_CList_Message.m_hWnd != NULL)
	{
		CRect rc;
		rc.left = 1;        //�б��������
		rc.top = cy - 156;    //�б��������
		rc.right = cx - 1;    //�б��������
		rc.bottom = cy - 6;  //�б��������
		m_CList_Message.MoveWindow(rc);

		for (int i = 0;i < g_Column_Message_Count;i++) //����ÿһ����
		{                   
			double dd = g_Column_Message_Data[i].nWidth;     //�õ���ǰ�еĿ��
			dd /= g_Column_Message_Width;                    //��һ����ǰ���ռ�ܳ��ȵļ���֮��
			dd *= dcx;                                       //��ԭ���ĳ��ȳ�����ռ�ļ���֮���õ���ǰ�Ŀ��
			int lenth = (int)dd;                                  //ת��Ϊint ����
			m_CList_Message.SetColumnWidth(i, lenth);      //���õ�ǰ�Ŀ��
		}
	}
}

void CPCRemoteDlg::InitListCtrl()
{
	m_CList_Online.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	for (int i = 0; i < g_Column_Online_Count; i++)
	{
		m_CList_Online.InsertColumn(i, g_Column_Online_Data[i].title, LVCFMT_CENTER, g_Column_Online_Data[i].nWidth);
		g_Column_Online_Width += g_Column_Online_Data[i].nWidth;       //�õ��ܿ��
	}

	m_CList_Message.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	for (int i = 0; i < g_Column_Message_Count; i++)
	{
		m_CList_Message.InsertColumn(i, g_Column_Message_Data[i].title, LVCFMT_CENTER, g_Column_Message_Data[i].nWidth);
		g_Column_Message_Width += g_Column_Message_Data[i].nWidth;       //�õ��ܿ��
	}
}

void CPCRemoteDlg::AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strPing)
{
	m_CList_Online.InsertItem(0, strIP);           //Ĭ��Ϊ0��  �������в�������ж���������
	m_CList_Online.SetItemText(0, ONLINELIST_ADDR, strAddr);      //�����е���ʾ�ַ�   ���� ONLINELIST_ADDR�� Ϊ�ڶ��ڿ��е�ö������ �������ķ���
	m_CList_Online.SetItemText(0, ONLINELIST_COMPUTER_NAME, strPCName); //������������Ժ���չʱ�ĳ�ͻ
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
		strIsOK = "ִ�гɹ�";
	}
	else {
		strIsOK = "ִ��ʧ��";
	}
	m_CList_Message.InsertItem(0, strIsOK);
	m_CList_Message.SetItemText(0, 1, strTime);
	m_CList_Message.SetItemText(0, 2, strMsg);
}

void CPCRemoteDlg::Test()
{
	this->AddList("192.168.0.1", "����������", "zhangxueming", "Windows 10", "2.2GHZ", "��", "123232");
	this->ShowMessageLog(true, "�����ʼ���ɹ�...");
}

void CPCRemoteDlg::OnNMRClickListOnline(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CMenu	popup;
	popup.LoadMenu(IDR_MENU_ONLINE);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	if (m_CList_Online.GetSelectedCount() == 0)       //���û��ѡ��
	{
		for (int i = 0; i < count; ++i)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);  //�˵�ȫ�����
		}

	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
}
