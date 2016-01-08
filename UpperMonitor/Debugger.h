#pragma once


// CDebugger 对话框

class CDebugger : public CDialogEx
{
	DECLARE_DYNAMIC(CDebugger)

public:
	CDebugger(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDebugger();

// 对话框数据
	enum { IDD = IDD_DEBUGGER };
	enum CurRatioStatus { noType, typeA = 0x0A, typeB = 0x0B };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtncardget();
	afx_msg void OnBnClickedBtnopendevice();
	afx_msg void OnBnClickedBtncardcheck();
	afx_msg void OnBnClickedBtnledset();
	afx_msg void OnBnClickedBtndefpwd();
	afx_msg void OnBnClickedRaotypea();
private:
	CurRatioStatus m_RatioStatus;
	CFont m_font;
	CFont m_font2;
	bool isDeviceOpen;
	bool canGetCardInfo;
	bool canSetLED;
	bool canIO;
public:
	afx_msg void OnBnClickedRaotypeb();
	afx_msg void OnBnClickedBtnreadblock();
	afx_msg void OnBnClickedBtnwriteblock();
	afx_msg void OnBnClickedBtnreadsection();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
