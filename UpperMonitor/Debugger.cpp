// Debugger.cpp : 实现文件
//

#include "stdafx.h"
#include "UpperMonitor.h"
#include "Debugger.h"
#include "afxdialogex.h"
#include "atlrx.h"
#include "Utils.h"

#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"

#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 0, 255)
#define BLACK RGB(0, 0, 0)

// CDebugger 对话框

IMPLEMENT_DYNAMIC(CDebugger, CDialogEx)

CDebugger::CDebugger(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugger::IDD, pParent)
{
	// 
	m_RatioStatus = noType;
	// 创建字体
	m_font.CreatePointFont(93, _T("楷体"));
	m_font2.CreatePointFont(110, _T("华文新魏"));
}

CDebugger::~CDebugger()
{
	DeleteObject(m_font);
}

void CDebugger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	GetDlgItem(IDC_OpenDevice)->SetFont(&m_font2);
	GetDlgItem(IDC_GETCARDINFO)->SetFont(&m_font2);
	GetDlgItem(IDC_LEDSET)->SetFont(&m_font2);
	GetDlgItem(IDC_IOOPERATION)->SetFont(&m_font2);
	GetDlgItem(IDC_PWDSET)->SetFont(&m_font2);
}


BEGIN_MESSAGE_MAP(CDebugger, CDialogEx)
	ON_BN_CLICKED(IDC_BTNCARDGET, &CDebugger::OnBnClickedBtncardget)
	ON_BN_CLICKED(IDC_BTNOPENDEVICE, &CDebugger::OnBnClickedBtnopendevice)
	ON_BN_CLICKED(IDC_BTNCARDCHECK, &CDebugger::OnBnClickedBtncardcheck)
	ON_BN_CLICKED(IDC_BTNLEDSET, &CDebugger::OnBnClickedBtnledset)
	ON_BN_CLICKED(IDC_BTNDEFPWD, &CDebugger::OnBnClickedBtndefpwd)
	ON_BN_CLICKED(IDC_RAOTYPEA, &CDebugger::OnBnClickedRaotypea)
	ON_BN_CLICKED(IDC_RAOTYPEB, &CDebugger::OnBnClickedRaotypeb)
	ON_BN_CLICKED(IDC_BTNREADBLOCK, &CDebugger::OnBnClickedBtnreadblock)
	ON_BN_CLICKED(IDC_BTNWRITEBLOCK, &CDebugger::OnBnClickedBtnwriteblock)
	ON_BN_CLICKED(IDC_BTNREADSECTION, &CDebugger::OnBnClickedBtnreadsection)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CDebugger 消息处理程序

void CDebugger::OnBnClickedBtncardget() {
	CString uid, temp;
	unsigned char buff[1024];
	int buff_len;

	// 成功获取
	if(find_14443(buff, &buff_len) == IFD_OK) {
		uid.Empty();
		for(int i = 0; i < buff_len; i++) {
			// 将获得的UID数据（1 byte）转为16进制
			temp.Format(_T("%02x"), buff[i]);
			uid += temp;
		}
		// 显示UID信息
		((CEdit*)GetDlgItem(IDC_EDITCARDUID))->SetWindowTextW(uid);
		// 更新状态栏成功
		canGetCardInfo = true;
		((CEdit*)GetDlgItem(IDC_EDITCARDSTATUS))->SetWindowTextW(_T("成功"));
	}
	else {
		// 失败时清空UID信息，防止上次遗漏数据
		((CEdit*)GetDlgItem(IDC_EDITCARDUID))->SetWindowTextW(_T(""));
		// 更新状态栏，失败
		canGetCardInfo = false;
		((CEdit*)GetDlgItem(IDC_EDITCARDSTATUS))->SetWindowTextW(_T("获取卡号异常"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtnopendevice() {
	if(IDD_PowerOn() == IFD_OK) {
		// 更新状态栏，成功
		isDeviceOpen = true;
		((CEdit*)GetDlgItem(IDC_EDITSTATUS))->SetWindowTextW(_T("开启设备成功"));
	}
	else {
		// 更新状态栏，失败
		isDeviceOpen = false;
		((CEdit*)GetDlgItem(IDC_EDITSTATUS))->SetWindowTextW(_T("开启设备失败"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtncardcheck() {
	unsigned char card_version[1024];
	int card_version_len;
	// 查看卡片版本信息
	if(Reader_Version(card_version, &card_version_len) == IFD_OK) {
		// 更新状态栏，成功
		canGetCardInfo = true;
		((CEdit*)GetDlgItem(IDC_EDITVERSIONINFO))->SetWindowTextW((CString)card_version);
	}
	else {
		// 更新状态栏，失败
		canGetCardInfo = false;
		((CEdit*)GetDlgItem(IDC_EDITVERSIONINFO))->SetWindowTextW(_T("查看失败"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtnledset() {
	CString info, mecNum;
	// 读取数字
	((CEdit*)GetDlgItem(IDC_EDITLEDINFO))->GetWindowTextW(info);
	// 读取小数位数
	((CEdit*)GetDlgItem(IDC_EDITLEDMECNUM))->GetWindowTextW(mecNum);
	// 判断是否空值非法
	if(info.IsEmpty() || mecNum.IsEmpty()) {
		MessageBox(_T("请输入LED设置内容"));
		return;
	}
	else if(info.SpanIncluding(_T("0123456789")) != info) {
		MessageBox(_T("LED只能输入纯数字"));
		return;
	}
	
	int len_info = info.GetLength();
	int intMecNum = _ttoi(mecNum);
	// 小数位数最大6位
	if(len_info > 6) {
		MessageBox(_T("数字过长"));
		return;
	}
	// 小数点比数字位数大
	else if (intMecNum >= len_info) {
		for (int i = 0; i <= intMecNum - len_info; i++){
			info = _T("0") + info;
		}
		len_info = info.GetLength();
	}
	// 根据实际+1
	unsigned char point = (unsigned char)_ttoi(mecNum) + 1;
	if(point > 0x06 || point < 0x00) {
		// 位数不能大于6
		MessageBox(_T("小数位数过大"));
		return;
	}
	else if (mecNum == _T("0")){
		// 无小数修复
		point = 0x00;
	}
	char chinfo[7];
	CUtils::CString2CharStar(info, chinfo, len_info);
	if(LED(chinfo, len_info, point) == IFD_OK) {
		// 更新状态栏，成功
		canSetLED = true;
		((CEdit*)GetDlgItem(IDC_EDITLEDSTATUS))->SetWindowTextW(_T("设置成功"));
	}
	else {
		// 更新状态栏，失败
		canSetLED = false;
		((CEdit*)GetDlgItem(IDC_EDITLEDSTATUS))->SetWindowTextW(_T("设置失败"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtndefpwd() {
	// 获取默认密钥
	((CEdit*)GetDlgItem(IDC_EDITPWD))->SetWindowTextW(_T("FFFFFFFFFFFF"));
}


void CDebugger::OnBnClickedRaotypea() {
	// 设置卡片类型为A
	m_RatioStatus = typeA;
}


void CDebugger::OnBnClickedRaotypeb() {
	// 设置卡片类型为Ｂ
	m_RatioStatus = typeB;
}


void CDebugger::OnBnClickedBtnreadblock() {
	// 检查是否选择密钥类型
	if(m_RatioStatus == noType) {
		MessageBox(_T("请选择密钥类型！"));
		return;
	}
	// 获取密钥
	CString pwd;
	((CEdit*)GetDlgItem(IDC_EDITPWD))->GetWindowTextW(pwd);
	if(pwd.IsEmpty()) {
		MessageBox(_T("请输入密钥！"));
		return;
	}
	else if(pwd.GetLength() != 12) {
		MessageBox(_T("密钥长度非法！"));
		return;
	}
	// 密钥类型类型转换
	//unsigned char* chpwd = (unsigned char*)(LPCTSTR)pwd;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	pwd.MakeUpper();
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 获取扇区号
	int sectionNum = ((CComboBox*)GetDlgItem(IDC_COMBOSECTION))->GetCurSel();
	if(sectionNum == CB_ERR) {
		MessageBox(_T("请选择扇区！"));
		return;
	}
	// 获取块区号
	int blockNum = ((CComboBox*)GetDlgItem(IDC_COMBOBLOCK))->GetCurSel();
	if(blockNum == CB_ERR) {
		MessageBox(_T("请选择块区"));
		return;
	}
	// 读取块
	unsigned char des_data[1024];
	int des_len;
	if(read_block(sectionNum, blockNum, m_RatioStatus, chpwd, des_data, &des_len) == IFD_OK) {
		// 格式化
		CString block_data, temp;
		block_data.Empty();
		for(int i = 0; i < des_len; i++) {
			temp.Format(_T("%02x"), des_data[i]);
			block_data += temp;
		}
		// 根据块号设置不同的块
		switch(blockNum) {
			case 0:
				((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->SetWindowTextW(block_data);
				break;
			case 1:
				((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->SetWindowTextW(block_data);
				break;
			case 2:
				((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->SetWindowTextW(block_data);
				break;
			default:
				// Maybe Wrong
				((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->SetWindowTextW(block_data.Left(12));
				((CEdit*)GetDlgItem(IDC_EDITBL3DATA1))->SetWindowTextW(block_data.Mid(12, 8));
				((CEdit*)GetDlgItem(IDC_EDITBL3DATA2))->SetWindowTextW(block_data.Right(12));
				break;

		}
		// 更新状态栏，成功
		canIO = true;
		((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("读取块成功"));
	}
	else {
		// 置空全部，更新状态栏，失败
		canIO = false;
		switch (blockNum){
		case 0:
			((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->SetWindowTextW(_T(""));
			break;
		case 1:
			((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->SetWindowTextW(_T(""));
			break;
		case 2:
			((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->SetWindowTextW(_T(""));
			break;
		default:
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA1))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA2))->SetWindowTextW(_T(""));
			break;
		}
		((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("读取块异常"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
}


void CDebugger::OnBnClickedBtnwriteblock() {
	// 获取密钥类型
	if(m_RatioStatus == noType) {
		MessageBox(_T("请选择密钥类型！"));
		return;
	}
	// 获取密钥
	CString pwd;
	((CEdit*)GetDlgItem(IDC_EDITPWD))->GetWindowTextW(pwd);
	if(pwd.IsEmpty()) {
		MessageBox(_T("请输入密钥！"));
		return;
	}
	else if(pwd.GetLength() != 12) {
		MessageBox(_T("密钥长度非法！"));
		return;
	}
	// 密钥类型转换
	//unsigned char* chpwd = (unsigned char*)(LPCTSTR)pwd;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 获取扇区
	int sectionNum = ((CComboBox*)GetDlgItem(IDC_COMBOSECTION))->GetCurSel();
	if(sectionNum == CB_ERR) {
		MessageBox(_T("请选择扇区！"));
		return;
	}
	// 获取块号
	int blockNum = ((CComboBox*)GetDlgItem(IDC_COMBOBLOCK))->GetCurSel();
	if(blockNum == CB_ERR) {
		MessageBox(_T("请选择块区"));
		return;
	}
	// 根据块号获取内容
	CString block_data;
	if(blockNum == 0) {
		((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->GetWindowTextW(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(blockNum == 1) {
		((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->GetWindowTextW(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(blockNum == 2) {
		((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->GetWindowTextW(block_data);
		if (block_data.GetLength() % 2 == 1)	block_data += _T("0");
	}
	else if(blockNum == 3) {
		((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->GetWindowTextW(block_data);
		if(block_data.GetLength() < 12) {
			for(int i = block_data.GetLength(); i<12; i++) {
				block_data += _T("0");
			}
		}
		block_data = block_data + _T("FF078069") + _T("FFFFFFFFFFFF");
	}
	
	if(block_data.GetLength() > 32) {
		MessageBox(_T("写入信息数据过长"));
		return;
	}
	/******************正则表达式验证************************/
	block_data.MakeLower();
	CAtlRegExp<> reNumEng;
	REParseError status = reNumEng.Parse(_T("^[a-z0-9]+$"));
	if(REPARSE_ERROR_OK != status) {
		MessageBox(_T("是我自己傻逼写错正则表达式了"));
		return;
	}
	CAtlREMatchContext<> mcNumEng;
	if(!reNumEng.Match(block_data, &mcNumEng)) {
		MessageBox(_T("输入有误，请检查输入!"));
		return;
	}
	//MessageBox(_T("调试强制退出"));
	//return;
	/*********************************************************/

	// 类型转换
	unsigned char src_data[200];
	int len_data = 0;
	CUtils::HexCString2UnsignedCharStar(block_data, src_data, &len_data);
	// 写入块
	if(write_block(blockNum, sectionNum, m_RatioStatus, chpwd, src_data, len_data) == IFD_OK) {
		canIO = true;
		((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("写入块成功"));
	}
	else {
		canIO = false;
		((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("写入块失败"));
		// 蜂鸣器提示失败
		CUtils::buzzerFailed();
	}
	
}


void CDebugger::OnBnClickedBtnreadsection() {
	// 检查是否选择密钥类型
	if(m_RatioStatus == noType) {
		MessageBox(_T("请选择密钥类型！"));
		return;
	}
	// 获取密钥
	CString pwd;
	((CEdit*)GetDlgItem(IDC_EDITPWD))->GetWindowTextW(pwd);
	if(pwd.IsEmpty()) {
		MessageBox(_T("请输入密钥！"));
		return;
	}
	else if(pwd.GetLength() != 12) {
		MessageBox(_T("密钥长度非法！"));
		return;
	}
	// 密钥类型类型转换
	//unsigned char* chpwd = (unsigned char*)(LPCTSTR)pwd;
	unsigned char chpwd[8];
	int len_chpwd = 0;
	pwd.MakeUpper();
	CUtils::HexCString2UnsignedCharStar(pwd, chpwd, &len_chpwd);
	// 获取扇区号
	int sectionNum = ((CComboBox*)GetDlgItem(IDC_COMBOSECTION))->GetCurSel();
	if(sectionNum == CB_ERR) {
		MessageBox(_T("请选择扇区！"));
		return;
	}
	// 获取块区号
	int blockNum = 0;
	// 读取块
	unsigned char des_data[1024];
	int des_len;
	for(blockNum = 0; blockNum < 4; blockNum++) {
		if(read_block(sectionNum, blockNum, m_RatioStatus, chpwd, des_data, &des_len) == IFD_OK) {
			// 格式化
			CString block_data, temp;
			block_data.Empty();
			for(int i = 0; i < des_len; i++) {
				temp.Format(_T("%02x"), des_data[i]);
				block_data += temp;
			}
			// 根据块号设置不同的块
			switch(blockNum) {
				case 0:
					((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->SetWindowTextW(block_data);
					break;
				case 1:
					((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->SetWindowTextW(block_data);
					break;
				case 2:
					((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->SetWindowTextW(block_data);
					break;
				default:
					// Maybe Wrong
					((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->SetWindowTextW(block_data.Left(12));
					((CEdit*)GetDlgItem(IDC_EDITBL3DATA1))->SetWindowTextW(block_data.Mid(12, 8));
					((CEdit*)GetDlgItem(IDC_EDITBL3DATA2))->SetWindowTextW(block_data.Right(12));
					break;

			}
			// 更新状态栏，成功
			canIO = true;
			((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("读取扇区成功"));
		}
		else {
			// 置空全部，更新状态栏，失败
			canIO = false;
			((CEdit*)GetDlgItem(IDC_EDITBL0DATA))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL1DATA))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL2DATA))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA0))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA1))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITBL3DATA2))->SetWindowTextW(_T(""));
			((CEdit*)GetDlgItem(IDC_EDITIOSTATUS))->SetWindowTextW(_T("读取扇区异常"));
			// 蜂鸣器提示失败
			CUtils::buzzerFailed();
		}
	}
	return;
}


HBRUSH CDebugger::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch(pWnd->GetDlgCtrlID()) {
		case IDC_EDITSTATUS:
			if(this->isDeviceOpen)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDITCARDSTATUS:
			if(this->canGetCardInfo)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDITLEDSTATUS:
			if(this->canSetLED)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDITIOSTATUS:			if(this->canIO)
				pDC->SetTextColor(BLUE);
			else
				pDC->SetTextColor(RED);
			break;
		case IDC_EDITVERSIONINFO:
		case IDC_EDITCARDUID:
		case IDC_EDITLEDINFO:
		case IDC_EDITLEDMECNUM:
		case IDC_EDITPWD:
		case IDC_EDITBL0DATA:
		case IDC_EDITBL1DATA:
		case IDC_EDITBL2DATA:
		case IDC_EDITBL3DATA0:
		case IDC_EDITBL3DATA1:
		case IDC_EDITBL3DATA2:
			pDC->SelectObject(&m_font);
			break;
		default:
			pDC->SetTextColor(BLACK);
			break;
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
