
// shellDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "shell.h"
#include "shellDlg.h"
#include "afxdialogex.h"
#include "IniCfg.h"
#include <string>
#include "Tlhelp32.h"





using std::string;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()


// CshellDlg 对话框




CshellDlg::CshellDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CshellDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	visible = false;   //在此初始化
	lockstate = 0;
	unlockstate = 0;
}

void CshellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CshellDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_BN_CLICKED(IDC_BUTTON1, &CshellDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CshellDlg 消息处理程序

BOOL CshellDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
		
	//1.隐藏窗口
	SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_HIDEWINDOW); 

	//2.注册快捷键
	RegHotKey();
	
	
	//3.读取配置文件
	CIniCfg ini;
	string m_strIniPath;
	m_strIniPath = "C:\\shell\\config.ini";
	ini.SetPath(m_strIniPath.c_str());

	//if configfile is not exist
	if (!ini.ReadFile())
	{
		MessageBox("shell file_missing！");
	}
	
	//ewf state
	state = atoi(ini.GetValue("ewf","state","2").c_str());
	//app path
	apppath = ini.GetValue("path","apppath","");
	//taskmanager path
	taskmanager = ini.GetValue("path","taskmanager","");

	//4.配置ewf
	char szCmd[200];
	if(state == 0)
	{	
		sprintf(szCmd,"ewfcfg /install-configuration");
		WinExec(szCmd,SW_HIDE);
		ini.SetValue("ewf","state","1");
		//system("cfg.bat");
		Sleep(300);
		
		sprintf(szCmd,"shutdown -r -t 0");
		WinExec(szCmd,SW_HIDE);
	};

	if(state == 1)
	{	
		sprintf(szCmd,"ewfmgr c: -enable");
		ini.SetValue("ewf", "state", "2");
		WinExec(szCmd,SW_HIDE);
		

		Sleep(300);
				
		sprintf(szCmd,"shutdown -r -t 0");
		WinExec(szCmd,SW_HIDE);
	};

	if (state == 2)
	{
		Sleep(60000);
		sprintf(szCmd, "ewfmgr c: -commit");
		ini.SetValue("ewf", "state", "3");
		WinExec(szCmd, SW_HIDE);
		

		Sleep(30);

		sprintf(szCmd, "shutdown -r -t 0");
		WinExec(szCmd, SW_HIDE);
	};

	
	
	//5.启动主程序
	WinExec(apppath.c_str(),SW_SHOW);
	
	

	
	

	
}


void CshellDlg::RegHotKey()
{
	//regist some hot key,do task in "PreTranslateMessage"
	::RegisterHotKey(m_hWnd, 001, MOD_CONTROL | MOD_SHIFT, 0X31);     //1   explorer
	::RegisterHotKey(m_hWnd, 002, MOD_CONTROL | MOD_SHIFT, 0X32);     //2   cmd
	::RegisterHotKey(m_hWnd, 003, MOD_CONTROL | MOD_SHIFT, 0X33);     //3   任务管理器
	::RegisterHotKey(m_hWnd, 004, MOD_CONTROL | MOD_SHIFT, 0X34);     //4   显示cpu，内存  
	::RegisterHotKey(m_hWnd, 005, MOD_CONTROL | MOD_SHIFT, 0X35);     //5   ewf，commit  
	::RegisterHotKey(m_hWnd, 006, MOD_CONTROL | MOD_SHIFT, 0X36);     //6   重置版本号     
	::RegisterHotKey(m_hWnd, 111, MOD_ALT, VK_F4);                    //alt+f4
	::RegisterHotKey(m_hWnd, 017, MOD_CONTROL | MOD_SHIFT, 0X38);     //8    从u盘还原     
	::RegisterHotKey(m_hWnd, 020, MOD_CONTROL | MOD_SHIFT, 0X39);     //9    从c盘还原
	::RegisterHotKey(m_hWnd, 007, MOD_CONTROL, 0x4F);         //unlock1,control+o
	::RegisterHotKey(m_hWnd, 011, MOD_ALT, 0x50);             //unlock2,alt+p
	::RegisterHotKey(m_hWnd, 012, MOD_CONTROL | MOD_ALT, 0x4D); //lock,control+alt+m
	
}

void CshellDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CshellDlg::OnPaint()
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
HCURSOR CshellDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL KillProcessFromName(CString strProcessName)
{
	//创建进程快照(TH32CS_SNAPPROCESS表示创建所有进程的快照)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32进程快照的结构体  
	PROCESSENTRY32 pe;

	//实例化后使用Process32First获取第一个快照的进程前必做的初始化操作  
	pe.dwSize = sizeof(PROCESSENTRY32);


	//下面的IF效果同:  
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   无效的句柄  
	if (!Process32First(hSnapShot, &pe))
	{
		return FALSE;
	}

	//将字符串转换为小写  
	strProcessName.MakeLower();

	//如果句柄有效  则一直获取下一个句柄循环下去  
	while (Process32Next(hSnapShot, &pe))
	{

		//pe.szExeFile获取当前进程的可执行文件名称  
		CString scTmp = pe.szExeFile;


		//将可执行文件名称所有英文字母修改为小写  
		scTmp.MakeLower();

		//比较当前进程的可执行文件名称和传递进来的文件名称是否相同  
		//相同的话Compare返回0  
		if (!scTmp.Compare(strProcessName))
		{

			//从快照进程中获取该进程的PID(即任务管理器中的PID)  
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
			::TerminateProcess(hProcess, 0);
			CloseHandle(hProcess);
			//return TRUE;
		}
		scTmp.ReleaseBuffer();
	}
	strProcessName.ReleaseBuffer();
	return TRUE;
}

#define BUF_SIZE 256
int copyFile(const char * pSrc, const char *pDes)
{
	FILE *in_file, *out_file;
	char data[BUF_SIZE];
	size_t bytes_in, bytes_out;
	long len = 0;
	if ((in_file = fopen(pSrc, "rb")) == NULL)
	{
		perror(pSrc);
		return -2;
	}
	if ((out_file = fopen(pDes, "wb")) == NULL)
	{
		perror(pDes);
		return -3;
	}
	while ((bytes_in = fread(data, 1, BUF_SIZE, in_file)) > 0)
	{
		bytes_out = fwrite(data, 1, bytes_in, out_file);
		if (bytes_in != bytes_out)
		{
			perror("Fatal write error.\n");
			return -4;
		}
		len += bytes_out;
		//printf("copying file .... %d bytes copy\n", len);
	}
	fclose(in_file);
	fclose(out_file);
	return 1;
}


BOOL CshellDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
	case WM_HOTKEY:
		if (001 == pMsg->wParam)
			if(lockstate == 2)
			ShellExecute(this->m_hWnd,"open","explorer","","",SW_SHOW );
		if (002 == pMsg->wParam)
			if(lockstate == 2)
			ShellExecute(this->m_hWnd,"open","cmd","","",SW_SHOW );
		if (003 == pMsg->wParam)
			if(lockstate == 2)
			ShellExecute(this->m_hWnd,"open",taskmanager.c_str(),"","",SW_SHOW );
		if (004 == pMsg->wParam)
		{
			if (lockstate == 2)
			ShellExecute(this->m_hWnd, "open", "c://shell//monitor", "", "", SW_SHOW);
		}
		if (005 == pMsg->wParam)
			if (lockstate == 2)			
			{
				char szCmd[200];
				sprintf(szCmd, "ewfmgr c: -commit");
				WinExec(szCmd, SW_HIDE);

				Sleep(30);
				MessageBox("系统修改已经提交，点击确定重启设备！", "医诊达调试系统", MB_SYSTEMMODAL);

				sprintf(szCmd, "shutdown -r -t 0");
				WinExec(szCmd, SW_HIDE);
			};
			
		if (006 == pMsg->wParam)
		{
			if (lockstate == 2)
			{
				ShellExecute(this->m_hWnd, "open", "c://shell//versionchanger", "", "", SW_SHOW);
				MessageBox("版本信息还原成功，按ok键重启后会自动升级！", "医诊达调试系统", MB_SYSTEMMODAL);
				char szCmd[200];
				sprintf(szCmd, "shutdown -r -t 0");
				WinExec(szCmd, SW_HIDE);
			}
			
			/*
			int res = copyFile("c:\\Yzd_bak\\updateconfiguration_bak.config" , "d:\\Yzd\\updateconfiguration.config");
			if (res == 1)
			{
				MessageBox("版本信息还原成功，按ok键重启！", NULL, MB_SYSTEMMODAL);

				char szCmd[200];
				sprintf(szCmd, "shutdown -r -t 0");
				WinExec(szCmd, SW_HIDE);
			}
			else
			{
				MessageBox("版本信息还原失败！", NULL, MB_SYSTEMMODAL);
			}
			*/
		};
		if (111 == pMsg->wParam)
		{
			break;
		};
		if (007 == pMsg->wParam)
		{
			lockstate = 1;
		};
		if (011 == pMsg->wParam)
		{
			if(lockstate == 1)
			{
				lockstate++;
				string  hehe;
				hehe = "system unlock!" + 
				MessageBox("system unlocked! 调试方法: \nctrl+shift+1 文件系统\nctrl+shift+2 命令行\nctrl+shift+3 任务管理器\nctrl+shift+4 监控\nctrl+shift+5 提交\nctrl+shift+6 重置版本\nctrl+shift+8 从u盘恢复出厂设置\nctrl+shift+9 从本地恢复出厂设置","医诊达调试系统",MB_SYSTEMMODAL);
				ShellExecute(this->m_hWnd,"open","cmd","","",SW_SHOW );
			}

		};

		if (012 == pMsg->wParam)
		{
			if(lockstate != 0)
			{
				lockstate = 0;
				MessageBox("system locked!", "医诊达调试系统",MB_SYSTEMMODAL);

			}
			
		};
		if (017 == pMsg->wParam)
		{
			if (lockstate == 2)
			{

				if (MessageBox("您确定将要进行系统修复(USB)？", "医诊达自动修复系统", MB_YESNO | MB_SETFOREGROUND) == IDYES)
				{
					if (GetDriveType("E:") != DRIVE_NO_ROOT_DIR)
					{
						KillProcessFromName("cmd.exe");
						KillProcessFromName("SmartChecker.exe");
						KillProcessFromName("YzdSmartBox.exe");

						char szCmd[200];
						sprintf(szCmd, "format D: /q /y");
						//WinExec(szCmd, SW_SHOW);
						system(szCmd);

						WinExec("C:\\shell\\copydir.exe e:\\yzd\\Yzd_bak d:\\Yzd", SW_SHOWNORMAL);

						MessageBox("修复完成，请按ok键重启！", "医诊达调试系统", MB_SYSTEMMODAL);
						sprintf(szCmd, "shutdown -r -t 0");
						WinExec(szCmd, SW_HIDE);

						
					}
					else
					{
						MessageBox("未检测到还原u盘,请插入还原u盘后重试！", NULL, MB_SYSTEMMODAL);
					}
					
					
				}

			}

		};
		if (020 == pMsg->wParam)
		{
			if (lockstate == 2)
			{
				
				if (MessageBox("您确定将要进行系统修复(本地)？", "医诊达自动修复系统", MB_YESNO | MB_SETFOREGROUND) == IDYES)
				{
					KillProcessFromName("cmd.exe");
					KillProcessFromName("SmartChecker.exe");
					KillProcessFromName("YzdSmartBox.exe");

					
					char szCmd[200];
					sprintf(szCmd, "format D: /q /y");
					//WinExec(szCmd, SW_SHOW);
					system(szCmd);
					

					WinExec("C:\\shell\\copydir.exe c:\\Yzd_bak d:\\Yzd", SW_SHOWNORMAL);

					MessageBox("修复完成，请按ok键重启！", "医诊达调试系统", MB_SYSTEMMODAL);
					sprintf(szCmd, "shutdown -r -t 0");
					WinExec(szCmd, SW_HIDE);
				}

			}

		};

	default:
		break;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAboutDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO: 在此处添加消息处理程序代码
}


void CshellDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	
	if(!visible)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO: 在此处添加消息处理程序代码
}


void CshellDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}

