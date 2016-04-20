
// shellDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CshellDlg �Ի���




CshellDlg::CshellDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CshellDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	visible = false;   //�ڴ˳�ʼ��
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


// CshellDlg ��Ϣ�������

BOOL CshellDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
		
	//1.���ش���
	SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_HIDEWINDOW); 

	//2.ע���ݼ�
	RegHotKey();
	
	
	//3.��ȡ�����ļ�
	CIniCfg ini;
	string m_strIniPath;
	m_strIniPath = "C:\\shell\\config.ini";
	ini.SetPath(m_strIniPath.c_str());

	//if configfile is not exist
	if (!ini.ReadFile())
	{
		MessageBox("shell file_missing��");
	}
	
	//ewf state
	state = atoi(ini.GetValue("ewf","state","2").c_str());
	//app path
	apppath = ini.GetValue("path","apppath","");
	//taskmanager path
	taskmanager = ini.GetValue("path","taskmanager","");

	//4.����ewf
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

	
	
	//5.����������
	WinExec(apppath.c_str(),SW_SHOW);
	
	

	
	

	
}


void CshellDlg::RegHotKey()
{
	//regist some hot key,do task in "PreTranslateMessage"
	::RegisterHotKey(m_hWnd, 001, MOD_CONTROL | MOD_SHIFT, 0X31);     //1   explorer
	::RegisterHotKey(m_hWnd, 002, MOD_CONTROL | MOD_SHIFT, 0X32);     //2   cmd
	::RegisterHotKey(m_hWnd, 003, MOD_CONTROL | MOD_SHIFT, 0X33);     //3   ���������
	::RegisterHotKey(m_hWnd, 004, MOD_CONTROL | MOD_SHIFT, 0X34);     //4   ��ʾcpu���ڴ�  
	::RegisterHotKey(m_hWnd, 005, MOD_CONTROL | MOD_SHIFT, 0X35);     //5   ewf��commit  
	::RegisterHotKey(m_hWnd, 006, MOD_CONTROL | MOD_SHIFT, 0X36);     //6   ���ð汾��     
	::RegisterHotKey(m_hWnd, 111, MOD_ALT, VK_F4);                    //alt+f4
	::RegisterHotKey(m_hWnd, 017, MOD_CONTROL | MOD_SHIFT, 0X38);     //8    ��u�̻�ԭ     
	::RegisterHotKey(m_hWnd, 020, MOD_CONTROL | MOD_SHIFT, 0X39);     //9    ��c�̻�ԭ
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CshellDlg::OnPaint()
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
HCURSOR CshellDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL KillProcessFromName(CString strProcessName)
{
	//�������̿���(TH32CS_SNAPPROCESS��ʾ�������н��̵Ŀ���)  
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//PROCESSENTRY32���̿��յĽṹ��  
	PROCESSENTRY32 pe;

	//ʵ������ʹ��Process32First��ȡ��һ�����յĽ���ǰ�����ĳ�ʼ������  
	pe.dwSize = sizeof(PROCESSENTRY32);


	//�����IFЧ��ͬ:  
	//if(hProcessSnap == INVALID_HANDLE_VALUE)   ��Ч�ľ��  
	if (!Process32First(hSnapShot, &pe))
	{
		return FALSE;
	}

	//���ַ���ת��ΪСд  
	strProcessName.MakeLower();

	//��������Ч  ��һֱ��ȡ��һ�����ѭ����ȥ  
	while (Process32Next(hSnapShot, &pe))
	{

		//pe.szExeFile��ȡ��ǰ���̵Ŀ�ִ���ļ�����  
		CString scTmp = pe.szExeFile;


		//����ִ���ļ���������Ӣ����ĸ�޸�ΪСд  
		scTmp.MakeLower();

		//�Ƚϵ�ǰ���̵Ŀ�ִ���ļ����ƺʹ��ݽ������ļ������Ƿ���ͬ  
		//��ͬ�Ļ�Compare����0  
		if (!scTmp.Compare(strProcessName))
		{

			//�ӿ��ս����л�ȡ�ý��̵�PID(������������е�PID)  
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
	// TODO: �ڴ����ר�ô����/����û���
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
				MessageBox("ϵͳ�޸��Ѿ��ύ�����ȷ�������豸��", "ҽ������ϵͳ", MB_SYSTEMMODAL);

				sprintf(szCmd, "shutdown -r -t 0");
				WinExec(szCmd, SW_HIDE);
			};
			
		if (006 == pMsg->wParam)
		{
			if (lockstate == 2)
			{
				ShellExecute(this->m_hWnd, "open", "c://shell//versionchanger", "", "", SW_SHOW);
				MessageBox("�汾��Ϣ��ԭ�ɹ�����ok����������Զ�������", "ҽ������ϵͳ", MB_SYSTEMMODAL);
				char szCmd[200];
				sprintf(szCmd, "shutdown -r -t 0");
				WinExec(szCmd, SW_HIDE);
			}
			
			/*
			int res = copyFile("c:\\Yzd_bak\\updateconfiguration_bak.config" , "d:\\Yzd\\updateconfiguration.config");
			if (res == 1)
			{
				MessageBox("�汾��Ϣ��ԭ�ɹ�����ok��������", NULL, MB_SYSTEMMODAL);

				char szCmd[200];
				sprintf(szCmd, "shutdown -r -t 0");
				WinExec(szCmd, SW_HIDE);
			}
			else
			{
				MessageBox("�汾��Ϣ��ԭʧ�ܣ�", NULL, MB_SYSTEMMODAL);
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
				MessageBox("system unlocked! ���Է���: \nctrl+shift+1 �ļ�ϵͳ\nctrl+shift+2 ������\nctrl+shift+3 ���������\nctrl+shift+4 ���\nctrl+shift+5 �ύ\nctrl+shift+6 ���ð汾\nctrl+shift+8 ��u�ָ̻���������\nctrl+shift+9 �ӱ��ػָ���������","ҽ������ϵͳ",MB_SYSTEMMODAL);
				ShellExecute(this->m_hWnd,"open","cmd","","",SW_SHOW );
			}

		};

		if (012 == pMsg->wParam)
		{
			if(lockstate != 0)
			{
				lockstate = 0;
				MessageBox("system locked!", "ҽ������ϵͳ",MB_SYSTEMMODAL);

			}
			
		};
		if (017 == pMsg->wParam)
		{
			if (lockstate == 2)
			{

				if (MessageBox("��ȷ����Ҫ����ϵͳ�޸�(USB)��", "ҽ����Զ��޸�ϵͳ", MB_YESNO | MB_SETFOREGROUND) == IDYES)
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

						MessageBox("�޸���ɣ��밴ok��������", "ҽ������ϵͳ", MB_SYSTEMMODAL);
						sprintf(szCmd, "shutdown -r -t 0");
						WinExec(szCmd, SW_HIDE);

						
					}
					else
					{
						MessageBox("δ��⵽��ԭu��,����뻹ԭu�̺����ԣ�", NULL, MB_SYSTEMMODAL);
					}
					
					
				}

			}

		};
		if (020 == pMsg->wParam)
		{
			if (lockstate == 2)
			{
				
				if (MessageBox("��ȷ����Ҫ����ϵͳ�޸�(����)��", "ҽ����Զ��޸�ϵͳ", MB_YESNO | MB_SETFOREGROUND) == IDYES)
				{
					KillProcessFromName("cmd.exe");
					KillProcessFromName("SmartChecker.exe");
					KillProcessFromName("YzdSmartBox.exe");

					
					char szCmd[200];
					sprintf(szCmd, "format D: /q /y");
					//WinExec(szCmd, SW_SHOW);
					system(szCmd);
					

					WinExec("C:\\shell\\copydir.exe c:\\Yzd_bak d:\\Yzd", SW_SHOWNORMAL);

					MessageBox("�޸���ɣ��밴ok��������", "ҽ������ϵͳ", MB_SYSTEMMODAL);
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

	// TODO: �ڴ˴������Ϣ����������
}


void CshellDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	
	if(!visible)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	
	CDialogEx::OnWindowPosChanging(lpwndpos);

	// TODO: �ڴ˴������Ϣ����������
}


void CshellDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

