// Appdev.cpp : 实现文件
//

#include "stdafx.h"
#include "UpperMonitor.h"
#include "Appdev.h"
#include "afxdialogex.h"

#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"

#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 0, 255)
#define BLACK RGB(0, 0, 0)

// CAppdev 对话框

IMPLEMENT_DYNAMIC(CAppdev, CDialogEx)

CAppdev::CAppdev(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAppdev::IDD, pParent)
{
	m_font2.CreatePointFont(110, _T("华文新魏"));
}

CAppdev::~CAppdev()
{
	DeleteObject(m_font2);
}

void CAppdev::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	GetDlgItem(IDC_STELTPURSE)->SetFont(&m_font2);
	GetDlgItem(IDC_STWEBMASTER)->SetFont(&m_font2);
	GetDlgItem(IDC_STHISTORY)->SetFont(&m_font2);
}


BEGIN_MESSAGE_MAP(CAppdev, CDialogEx)
	ON_BN_CLICKED(IDC_BTNPURSEINIT, &CAppdev::OnBnClickedBtnpurseinit)
	ON_BN_CLICKED(IDC_BTNCHECKBALANCE, &CAppdev::OnBnClickedBtncheckbalance)
	ON_BN_CLICKED(IDC_BTNRECHARGE, &CAppdev::OnBnClickedBtnrecharge)
	ON_BN_CLICKED(IDC_BTNCOMSURGE2, &CAppdev::OnBnClickedBtncomsurge2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNSTARTWEB, &CAppdev::OnBnClickedBtnstartweb)
END_MESSAGE_MAP()

// 自定义函数
void CAppdev::CString2CharStar(const CString& s, char* ch, int len) {
	int i;
	for(i = 0; i < len; i++) {
		ch[i] = s[i];
	}
	ch[i] = '\0';
	return;
}

void CAppdev::HexCString2UnsignedCharStar(const CString& hexStr, unsigned char* asc, int* asc_len) {
	*asc_len = 0;
	int len = hexStr.GetLength();

	char temp[200];
	char tmp[3] = { 0 };
	char* Hex;
	unsigned char* p;

	CString2CharStar(hexStr, temp, len);
	Hex = temp;
	p = asc;

	while(*Hex != '\0') {
		tmp[0] = *Hex;
		Hex++;
		tmp[1] = *Hex;
		Hex++;
		tmp[2] = '\0';
		*p = (unsigned char)strtol(tmp, NULL, 16);
		p++;
		(*asc_len)++;
	}
	*p = '\0';
	return;
}


// CAppdev 消息处理程序


void CAppdev::OnBnClickedBtnpurseinit() {
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->GetWindowTextW(balance);
	if(balance.IsEmpty()) {
		// 检查输入
		MessageBox(_T("请输入正确的初始化余额！"));
		return;
	}
	long account = _ttol(balance);
	if(account < 0) {
		// 检查输入
		MessageBox(_T("初始化金额有误！"));
		return;
	}
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥类型转换
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 初始化钱包
	if(write_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("初始化钱包成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("初始化钱包失败"));
	}
}


void CAppdev::OnBnClickedBtncheckbalance() {
	CString balance;
	long account = 0;
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥转换
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 读取钱包
	if(read_account(sectionNum, blockNum, pswtype, chpwd, &account) == IFD_OK) {
		balance.Format(_T("%d"), account);
		((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->SetWindowTextW(balance);
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("查询钱包成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("查询钱包失败"));
	}
}


void CAppdev::OnBnClickedBtnrecharge() {
	CString addAccount;
	((CEdit*)GetDlgItem(IDC_EDITRECHABALAN))->GetWindowTextW(addAccount);
	if(addAccount.IsEmpty()) {
		// 检查充值输入
		MessageBox(_T("请输入正确的充值金额！"));
		return;
	}
	long account = _ttol(addAccount);
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥转换
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 充值函数
	if(add_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckbalance();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("充值成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("充值失败"));
	}
}


void CAppdev::OnBnClickedBtncomsurge2() {
	CString subAccount;
	((CEdit*)GetDlgItem(IDC_EDITCOMSUBALAN2))->GetWindowTextW(subAccount);
	if(subAccount.IsEmpty()) {
		// 检查输入
		MessageBox(_T("请输入正确的消费金额！"));
		return;
	}
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->GetWindowTextW(balance);
	long account = _ttol(subAccount);
	if(account > _ttol(balance)) {
		// 检查余额
		MessageBox(_T("余额不足！"));
		return;
	}
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥类型转换
	CString pwd = _T("FFFFFFFFFFFF");
	HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 消费函数
	if(sub_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckbalance();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("消费成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("消费失败"));
	}
}


HBRUSH CAppdev::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch(pWnd->GetDlgCtrlID()) {
		case IDC_EDITELESTATUS:
			if(this->canIOPurse)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		default:
			pDC->SetTextColor(BLACK);
			break;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CAppdev::OnBnClickedBtnstartweb() {
	CString uid, temp;
	unsigned char buff[1024];
	int buff_len;

	// 成功获取
	if (find_14443(buff, &buff_len) == IFD_OK) {
		uid.Empty();
		for (int i = 0; i < buff_len; i++) {
			// 将获得的UID数据（1 byte）转为16进制
			temp.Format(_T("%02x"), buff[i]);
			uid += temp;
		}
		// 插入一条新记录
		CTime curTime = CTime::GetCurrentTime();
		if (adoMySQLHelper.MySQL_Insert(OnRecord(uid, DEFAULTREMAINTIME, curTime.Format("%Y/%m/%d %H:%M:%S")))
			&& adoMySQLHelper.MySQL_Insert(RemainTime(uid, DEFAULTREMAINTIME))){
			// 更新状态栏成功
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("上机成功"));
		}
		else {
			adoMySQLHelper.MySQL_Delete(uid, _T("OnTable"));
			adoMySQLHelper.MySQL_Delete(uid, _T("RemainTimeTable"));
			// 更新状态栏成功
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("插入新记录失败"));
		}
	}
	else {
		// 更新状态栏，失败
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("获取卡号异常"));
	}
}
