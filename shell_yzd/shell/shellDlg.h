
// shellDlg.h : ͷ�ļ�
//

#pragma once
#include <string>
using std::string;


// CshellDlg �Ի���
class CshellDlg : public CDialogEx
{
// ����
public:
	CshellDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SHELL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
