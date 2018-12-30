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

//自定义消息
enum
{
	UM_ICONNOTIFY = WM_USER + 0x100,  // 系统托盘
};

enum
{
	WM_CLIENT_CONNECT = WM_APP + 0x1001,
	WM_CLIENT_CLOSE,
	WM_CLIENT_NOTIFY,
	WM_DATA_IN_MSG,
	WM_DATA_OUT_MSG,
	WM_ADDTOLIST = WM_USER + 102,	// 添加到列表视图中
	WM_REMOVEFROMLIST,				// 从列表视图中删除
	WM_OPENMANAGERDIALOG,			// 打开一个文件管理窗口
	WM_OPENSCREENSPYDIALOG,			// 打开一个屏幕监视窗口
	WM_OPENWEBCAMDIALOG,			// 打开摄像头监视窗口
	WM_OPENAUDIODIALOG,				// 打开一个语音监听窗口
	WM_OPENKEYBOARDDIALOG,			// 打开键盘记录窗口
	WM_OPENPSLISTDIALOG,			// 打开进程管理窗口
	WM_OPENSHELLDIALOG,				// 打开shell窗口
	WM_RESETPORT,					// 改变端口
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
	OSVERSIONINFOEX	OsVerInfoEx;	// 版本信息
	int				CPUClockMhz;	// CPU主频
	IN_ADDR			IPAddress;		// 存储32位的IPv4的地址数据结构
	char			HostName[50];	// 主机名
	bool			bIsWebCam;		// 是否有摄像头
	DWORD			dwSpeed;		// 网速
}LOGININFO;

#endif // !_PC_REMOTE_SYSPUB_H_
