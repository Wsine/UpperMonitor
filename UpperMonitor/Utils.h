#pragma once

class CUtils{
public:
	CUtils();
	~CUtils();
	static void CString2CharStar(const CString& s, char* ch, int len);
	static void HexCString2UnsignedCharStar(const CString& hexStr, unsigned char* asc, int* asc_len);
	static BOOL buzzerFailed();
	static BOOL LEDSet(long num);
};

