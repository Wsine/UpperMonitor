#pragma once

#include "AdoMySQLHelper.h"

#define DEFAULTREMAINTIME 20
// CAppdev 对话框

class CAppdev : public CDialogEx
{
	DECLARE_DYNAMIC(CAppdev)

public:
	CAppdev(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAppdev();

// 对话框数据
	enum { IDD = IDD_APPDEV };

private:
	bool canIOPurse;
	bool canIOWeb;
	CFont m_font2;
	CAdoMySQLHelper adoMySQLHelper;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnpurseinit();
	void CString2CharStar(const CString& s, char* ch, int len);
	void HexCString2UnsignedCharStar(const CString& hexStr, unsigned char* asc, int* asc_len);
	afx_msg void OnBnClickedBtncheckbalance();
	afx_msg void OnBnClickedBtnrecharge();
	afx_msg void OnBnClickedBtncomsurge2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnstartweb();
};
