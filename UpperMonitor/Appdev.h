#pragma once

#include "AdoMySQLHelper.h"

#define DEFAULTREMAINTIME 20
// CAppdev �Ի���

class CAppdev : public CDialogEx
{
	DECLARE_DYNAMIC(CAppdev)

public:
	CAppdev(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAppdev();

// �Ի�������
	enum { IDD = IDD_APPDEV };

private:
	bool canIOPurse;
	bool canIOWeb;
	CFont m_font2;
	CAdoMySQLHelper adoMySQLHelper;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
