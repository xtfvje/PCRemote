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

#endif // !_PC_REMOTE_SYSPUB_H_
