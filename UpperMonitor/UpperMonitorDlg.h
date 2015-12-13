
// UpperMonitorDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "Debugger.h"
#include "Appdev.h"


// CUpperMonitorDlg 对话框
class CUpperMonitorDlg : public CDialogEx
{
// 构造
public:
	CUpperMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CUpperMonitorDlg();

// 对话框数据
	enum { IDD = IDD_UPPERMONITOR_DIALOG };

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
public:
	CTabCtrl m_MainMenu;
//private:
	CDebugger m_MenuDebugger;
	CAppdev m_MenuAppdev;
public:
	afx_msg void OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
};
