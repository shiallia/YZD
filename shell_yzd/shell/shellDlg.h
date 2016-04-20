
// shellDlg.h : 头文件
//

#pragma once
#include <string>
using std::string;


// CshellDlg 对话框
class CshellDlg : public CDialogEx
{
// 构造
public:
	CshellDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SHELL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnHotKey(WPARAM wParam,LPARAM lParam);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void RegHotKey();
	bool visible;   //for hide appwindow
	int state;    //ewf state
	string apppath;   
	string taskmanager;
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos); //for hide appwindow
	afx_msg void OnBnClickedButton1();
    int lockstate;
	int unlockstate;
};
