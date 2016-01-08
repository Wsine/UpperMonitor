// Appdev.cpp : 实现文件
//

#include "stdafx.h"
#include "UpperMonitor.h"
#include "Appdev.h"
#include "afxdialogex.h"
#include "Utils.h"

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
	this->isWritingRemainTimeTable = false;
}

CAppdev::~CAppdev()
{
	// 销毁自定义字体
	DeleteObject(m_font2);
}

void CAppdev::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	GetDlgItem(IDC_STELTPURSE)->SetFont(&m_font2);
	GetDlgItem(IDC_STWEBMASTER)->SetFont(&m_font2);
	GetDlgItem(IDC_STHISTORY)->SetFont(&m_font2);
	// 启动定时器
	m_ActiveTimer = SetTimer(SCANTIMER_ID, SCANTIMER * 1000, NULL);
}


BEGIN_MESSAGE_MAP(CAppdev, CDialogEx)
	ON_BN_CLICKED(IDC_BTNPURSEINIT, &CAppdev::OnBnClickedBtnpurseinit)
	ON_BN_CLICKED(IDC_BTNCHECKBALANCE, &CAppdev::OnBnClickedBtncheckbalance)
	ON_BN_CLICKED(IDC_BTNRECHARGE, &CAppdev::OnBnClickedBtnrecharge)
	ON_BN_CLICKED(IDC_BTNCOMSURGE2, &CAppdev::OnBnClickedBtncomsurge2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTNSTARTWEB, &CAppdev::OnBnClickedBtnstartweb)
	ON_BN_CLICKED(IDC_BTNRETIMEDEFINIT, &CAppdev::OnBnClickedBtnretimedefinit)
	ON_BN_CLICKED(IDC_BTNEXITWEB, &CAppdev::OnBnClickedBtnexitweb)
	ON_BN_CLICKED(IDC_BTNCHECKRETIME, &CAppdev::OnBnClickedBtncheckretime)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTNLOADHIS, &CAppdev::OnBnClickedBtnloadhis)
	ON_BN_CLICKED(IDC_BTNCLEARHIS, &CAppdev::OnBnClickedBtnclearhis)
END_MESSAGE_MAP()

CString CAppdev::GetCardUID() {
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
		return uid;
	}
	else {
		return NOCARD;
		//return TESTCARD;
	}
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
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 初始化钱包
	if(write_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("初始化钱包成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("初始化钱包失败"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
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
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 读取钱包
	if(read_account(sectionNum, blockNum, pswtype, chpwd, &account) == IFD_OK) {
		balance.Format(_T("%d"), account);
		((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->SetWindowTextW(balance);
		canIOPurse = true;
		CUtils::LEDSet(account); // MayBe Wrong
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("查询钱包成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("查询钱包失败"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
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
	CString balance;
	((CEdit*)GetDlgItem(IDC_EDITELEBALAN))->GetWindowTextW(balance);
	long account = _ttol(addAccount);
	int sectionNum = 15;
	int blockNum = 0;
	unsigned char pswtype = 0x0A;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	// 密钥转换
	CString pwd = _T("FFFFFFFFFFFF");
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 获取卡号
	CString uid = GetCardUID();
	// 充值函数
	if(add_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckbalance();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("充值成功"));
		fileRecordHelper.SaveRecharges(uid, addAccount, account + _ttol(balance), _T("成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("充值失败"));
		fileRecordHelper.SaveRecharges(uid, addAccount, _ttol(balance), _T("失败"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
	// 更新历史记录显示
	OnBnClickedBtnloadhis();
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
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 获取卡号
	CString uid = GetCardUID();
	// 消费函数
	if(sub_account(sectionNum, blockNum, pswtype, chpwd, account) == IFD_OK) {
		OnBnClickedBtncheckbalance();
		canIOPurse = true;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("消费成功"));
		fileRecordHelper.SaveConsumptions(uid, subAccount, _ttol(balance) - account, _T("成功"));
	}
	else {
		canIOPurse = false;
		((CEdit*)GetDlgItem(IDC_EDITELESTATUS))->SetWindowTextW(_T("消费失败"));
		fileRecordHelper.SaveConsumptions(uid, subAccount, _ttol(balance), _T("失败"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
	// 更新历史记录显示
	OnBnClickedBtnloadhis();
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
		case IDC_EDITWEBSTATUS:
			if (this->canIOWeb)
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
	CString uid = GetCardUID();

	// 成功获取
	if (uid != NOCARD) {
		// 当用户不在OnTable中，提示初始化
		if (!adoMySQLHelper.MySQL_QueryByUID(uid, ONTABLE)) {
			MessageBox(_T("请先初始化余时"));
		}
		// 当用户存在OnTable中
		else {
			// 当用户存在RemainTimeTable中，什么也不做
			if (adoMySQLHelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)){
				this->canIOWeb = false;
				// 更新状态栏，失败
				((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("正在上机中"));
			}
			// 当用户不存在RemainTimeTable中，从OnTable中读取并写入到RemainTimeTable
			else{
				CString cond = _T("UID=\'") + uid + _T("\'");
				OnRecord* pRecord = (OnRecord*)adoMySQLHelper.MySQL_Query(cond, ONTABLE);
				// 当用户已经超时
				if (pRecord->isOvertime) {
					this->canIOWeb = false;
					// 更新状态栏，失败
					((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("已超时，请先充值"));
					fileRecordHelper.StartNets(uid, pRecord->RemainSeconds, _T("失败"));
				}
				// 用户没有超时
				else {
					adoMySQLHelper.MySQL_Insert(RemainTime(pRecord->UID, pRecord->RemainSeconds));
					this->canIOWeb = true;
					// 更新状态栏，成功
					((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("开始上机成功"));
					fileRecordHelper.StartNets(uid, pRecord->RemainSeconds, _T("成功"));
				}
				// 更新历史记录显示
				OnBnClickedBtnloadhis();
				delete(pRecord); // Important!
			}
		}
	}
	else {
		this->canIOWeb = false;
		// 更新状态栏，失败
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("获取卡号异常"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}


void CAppdev::OnBnClickedBtnretimedefinit() {
	CString uid = GetCardUID();

	// 成功获取
	if (uid != NOCARD) {
		// 当用户不在OnTable中，插入新记录
		if (!adoMySQLHelper.MySQL_QueryByUID(uid, ONTABLE)) {
			CTime curTime = CTime::GetCurrentTime();
			adoMySQLHelper.MySQL_Insert(OnRecord(uid, DEFAULTREMAINTIME, curTime.Format(TIMEFORMAT)));
			this->canIOWeb = true;
			// 更新状态栏，成功
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("初始化余时成功"));
		}
		// 当用户存在OnTable中，更新OnTable
		else {
			adoMySQLHelper.MySQL_UpdateRemainTime(uid, DEFAULTREMAINTIME, ONTABLE);
			// 如果用户正在上机，一并更新RemainTimeTable
			if (adoMySQLHelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
				while (this->isWritingRemainTimeTable) { Sleep(100); } // 休眠0.1s等待定时器操作完成
				adoMySQLHelper.MySQL_UpdateRemainTime(uid, DEFAULTREMAINTIME, REMAINTIMETABLE);
			}
			this->canIOWeb = true;
			// 更新状态栏，成功
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("重设余时成功"));
		}
	}
	else {
		this->canIOWeb = false;
		// 更新状态栏，失败
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("获取卡号异常"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}


void CAppdev::OnBnClickedBtnexitweb() {
	CString uid = GetCardUID();

	// 成功获取
	if (uid != NOCARD) {
		// 用户没有在上机
		if (!adoMySQLHelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
			this->canIOWeb = false;
			// 更新状态栏，失败
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("没有在上机，无法退出"));
		}
		// 用户正在上机
		else {
			CString cond = _T("UID=\'") + uid + _T("\'");
			while (this->isWritingRemainTimeTable) { Sleep(100); } // 休眠0.1s等待定时器操作完成
			RemainTime* pRemainTime = (RemainTime*)adoMySQLHelper.MySQL_Query(cond, REMAINTIMETABLE);
			adoMySQLHelper.MySQL_Delete(uid, REMAINTIMETABLE); // 删除RemainTimeTable记录，退出上机
			adoMySQLHelper.MySQL_UpdateRemainTime(uid, pRemainTime->RemainSeconds, ONTABLE); // 更新OnTable
			this->canIOWeb = true;
			// 更新状态栏，成功
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("成功退出上机"));
			fileRecordHelper.ExitNets(uid, pRemainTime->RemainSeconds, DEFAULTREMAINTIME, _T("成功")); // TODO: Fix OverTime
			// 更新历史记录显示
			OnBnClickedBtnloadhis();
			delete(pRemainTime); // important!
		}
	}
	else {
		this->canIOWeb = false;
		// 更新状态栏，失败
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("获取卡号异常"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}


void CAppdev::OnBnClickedBtncheckretime() {
	CString uid = GetCardUID();

	// 成功获取
	if (uid != NOCARD) {
		while (this->isWritingRemainTimeTable) { Sleep(100); } // 休眠0.1s等待定时器操作完成
		// 用户没有在上机
		if (!adoMySQLHelper.MySQL_QueryByUID(uid, REMAINTIMETABLE)) {
			this->canIOWeb = false;
			// 更新状态栏，失败
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("没有在上机"));
		}
		// 用户正在上机
		else {
			CString cond = _T("UID=\'") + uid + _T("\'");
			RemainTime* pRemainTime = (RemainTime*)adoMySQLHelper.MySQL_Query(cond, REMAINTIMETABLE);
			CString remainHours;
			remainHours.Format(_T("%d"), pRemainTime->RemainSeconds / 3600);
			CString remainMinutes;
			remainMinutes.Format(_T("%d"), (pRemainTime->RemainSeconds % 3600) / 60);
			CString remainSeconds;
			remainSeconds.Format(_T("%d"), pRemainTime->RemainSeconds  % 60);
			// 更新UI
			((CEdit*)GetDlgItem(IDC_EDITREHOUR))->SetWindowTextW(remainHours);
			((CEdit*)GetDlgItem(IDC_EDITREMINUTE))->SetWindowTextW(remainMinutes);
			((CEdit*)GetDlgItem(IDC_EDITRESECOND))->SetWindowTextW(remainSeconds);
			this->canIOWeb = true;
			// 更新状态栏，成功
			((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("查询余时成功"));
			delete(pRemainTime); // important!
		}
	}
	else {
		this->canIOWeb = false;
		// 更新状态栏，失败
		((CEdit*)GetDlgItem(IDC_EDITWEBSTATUS))->SetWindowTextW(_T("获取卡号异常"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}

void CAppdev::OnBnClickedBtnloadhis(){
	((CEdit*)GetDlgItem(IDC_EDITHISTORY))->SetWindowTextW(fileRecordHelper.LoadRecords());
}


void CAppdev::OnBnClickedBtnclearhis(){
	if (fileRecordHelper.EmptyRecords()) {
		((CEdit*)GetDlgItem(IDC_EDITHISTORY))->SetWindowTextW(_T(""));
	}
}

void CAppdev::OnTimer(UINT_PTR nIDEvent){
	// 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent){
		case SCANTIMER_ID:
			this->isWritingRemainTimeTable = true;
			adoMySQLHelper.MySQL_ScanOnTable(SCANTIMER);
			this->isWritingRemainTimeTable = false;
			break;
		default:
			break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


BOOL CAppdev::DestroyWindow(){
	// 销毁定时器
	KillTimer(m_ActiveTimer);
	return CDialogEx::DestroyWindow();
}
