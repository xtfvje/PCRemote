#ifndef _PC_REMOTE_SYSPUB_H_
#define _PC_REMOTE_SYSPUB_H_

enum  // m_CList_Online 列表控件列名
{
	ONLINELIST_IP = 0,          //IP的列顺序
	ONLINELIST_ADDR,            //地址
	ONLINELIST_COMPUTER_NAME,   //计算机名/备注
	ONLINELIST_OS,              //操作系统
	ONLINELIST_CPU,             //CPU
	ONLINELIST_VIDEO,           //摄像头
	ONLINELIST_PING             //PING
};

typedef struct
{
	char	*title;   //列表的名称
	int		nWidth;   //列表的宽度
}COLUMNSTRUCT;

const int g_Column_Online_Count = 7; //列表的个数

const int g_Column_Message_Count = 3; //列表的个数

#endif // !_PC_REMOTE_SYSPUB_H_
