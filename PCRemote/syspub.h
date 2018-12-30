#ifndef _PC_REMOTE_SYSPUB_H_
#define _PC_REMOTE_SYSPUB_H_

enum  // m_CList_Online �б�ؼ�����
{
	ONLINELIST_IP = 0,          //IP����˳��
	ONLINELIST_ADDR,            //��ַ
	ONLINELIST_COMPUTER_NAME,   //�������/��ע
	ONLINELIST_OS,              //����ϵͳ
	ONLINELIST_CPU,             //CPU
	ONLINELIST_VIDEO,           //����ͷ
	ONLINELIST_PING             //PING
};

typedef struct
{
	char	*title;   //�б������
	int		nWidth;   //�б�Ŀ��
}COLUMNSTRUCT;

const int g_Column_Online_Count = 7; //�б�ĸ���

const int g_Column_Message_Count = 3; //�б�ĸ���

//�Զ�����Ϣ
enum
{
	UM_ICONNOTIFY = WM_USER + 0x100,  // ϵͳ����
};

enum
{
	WM_CLIENT_CONNECT = WM_APP + 0x1001,
	WM_CLIENT_CLOSE,
	WM_CLIENT_NOTIFY,
	WM_DATA_IN_MSG,
	WM_DATA_OUT_MSG,
	WM_ADDTOLIST = WM_USER + 102,	// ��ӵ��б���ͼ��
	WM_REMOVEFROMLIST,				// ���б���ͼ��ɾ��
	WM_OPENMANAGERDIALOG,			// ��һ���ļ�������
	WM_OPENSCREENSPYDIALOG,			// ��һ����Ļ���Ӵ���
	WM_OPENWEBCAMDIALOG,			// ������ͷ���Ӵ���
	WM_OPENAUDIODIALOG,				// ��һ��������������
	WM_OPENKEYBOARDDIALOG,			// �򿪼��̼�¼����
	WM_OPENPSLISTDIALOG,			// �򿪽��̹�����
	WM_OPENSHELLDIALOG,				// ��shell����
	WM_RESETPORT,					// �ı�˿�
	FILEMANAGER_DLG = 1,
	SCREENSPY_DLG,
	WEBCAM_DLG,
	AUDIO_DLG,
	KEYBOARD_DLG,
	SYSTEM_DLG,
	SHELL_DLG
};

typedef struct
{
	BYTE			bToken;			// = 1
	OSVERSIONINFOEX	OsVerInfoEx;	// �汾��Ϣ
	int				CPUClockMhz;	// CPU��Ƶ
	IN_ADDR			IPAddress;		// �洢32λ��IPv4�ĵ�ַ���ݽṹ
	char			HostName[50];	// ������
	bool			bIsWebCam;		// �Ƿ�������ͷ
	DWORD			dwSpeed;		// ����
}LOGININFO;

#endif // !_PC_REMOTE_SYSPUB_H_
