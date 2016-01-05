#include "stdafx.h"
#include "Utils.h"

#pragma comment(lib, "./libs/ZM124U.lib")
#include "./libs/ZM124U.h"

CUtils::CUtils(){}

CUtils::~CUtils(){}

void CUtils::CString2CharStar(const CString& s, char* ch, int len) {
	int i;
	for (i = 0; i < len; i++) {
		ch[i] = s[i];
	}
	ch[i] = '\0';
	return;
}

void CUtils::HexCString2UnsignedCharStar(const CString& hexStr, unsigned char* asc, int* asc_len) {
	*asc_len = 0;
	int len = hexStr.GetLength();

	char temp[200];
	char tmp[3] = { 0 };
	char* Hex;
	unsigned char* p;

	CString2CharStar(hexStr, temp, len);
	Hex = temp;
	p = asc;

	while (*Hex != '\0') {
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

BOOL CUtils::buzzerFailed() {
	// 设置蜂鸣器的蜂鸣方式
	if (buzzer_setting('1000', '200', 10, 0) == IFD_OK) {
		return true;
	}
	else {
		return false;
	}
}

BOOL CUtils::LEDSet(long num) {
	// 设置LED
	// 最高支持6位数字
	if (num >=0 && num <= 999999) {
		unsigned char point = 0x00; // 无小数
		CString strNum;
		strNum.Format(_T("%d"), num);
		int len_info = strNum.GetLength();
		char chinfo[7];
		CString2CharStar(strNum, chinfo, len_info);
		if (LED(chinfo, len_info, point) == IFD_OK) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
