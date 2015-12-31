#include "stdafx.h"
#include "RecordHelper.h"


// 自定义帮助函数
int Char2Unicode(char *pchIn, CString *pstrOut){
	int nLen;
	WCHAR *ptch;
	if (pchIn == NULL) {
		return 0;
	}
	nLen = MultiByteToWideChar(CP_ACP, 0, pchIn, -1, NULL, 0);
	ptch = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, pchIn, -1, ptch, nLen);
	pstrOut->Format(_T("%s"), ptch);
	delete[] ptch;
	return nLen;
}

int Unicode2Char(CString &strIn, char *pchOut, int nCharLen){
	if (pchOut == NULL){
		return 0;
	}
	int nLen;
	// Problem: Compile Problem
	/*nLen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strIn.GetBuffer(BUFFER_SIZE_KILO), -1, NULL, 0, NULL, NULL);
	nLen = min(nLen, nCharLen);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)strIn.GetBuffer(BUFFER_SIZE_KILO), -1, pchOut, nLen, NULL, NULL);
	if (nLen < nCharLen){
		pchOut[nLen] = 0;
	}*/
	return nLen;
}

BOOL FileUnicodeEncode(CFile &mFile) {
	// 该函数只能在mFile.Close()前调用
	WORD unicode = 0xFEFF;
	mFile.SeekToBegin();
	mFile.Write(&unicode, 2); // Unicode
	return true;
}

CRecordHelper::CRecordHelper(CString _mSaveFile){
	// 绑定文件路径
	this->mSaveFile = _mSaveFile;
}


CRecordHelper::~CRecordHelper(){

}

void CRecordHelper::SaveRecharges(CString uid, CString accounts, long remainings, CString result){
	// 打开文件
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// 获取当前时间
	CTime curTime = CTime::GetCurrentTime();
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户充值\r\n金额：%s\r\n余额：%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);
	// 指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// 关闭文件
	mFile.Close();
}

void CRecordHelper::SaveConsumptions(CString uid, CString accounts, long remainings, CString result){
	// 打开文件
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// 获取当前时间
	CTime curTime = CTime::GetCurrentTime();
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户消费\r\n金额：%s\r\n余额：%d\r\n\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, accounts, remainings);
	// 指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// 关闭文件
	mFile.Close();
}

void CRecordHelper::StartNets(CString uid, int remainTime, CString result){
	// 打开文件
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// 获取当前时间
	CTime curTime = CTime::GetCurrentTime();
	// 格式化余时
	CString sRemainTime;
	sRemainTime.Format(_T("%d时%d分%d秒"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户上机\r\n余时：%s\r\n\r\n"), 
					uid, curTime.Format(TIMEFORMAT), result, sRemainTime);
	// 指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// 关闭文件
	mFile.Close();
}

void CRecordHelper::ExitNets(CString uid, int remainTime, int overSeconds, CString result){
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// 获取当前时间
	CTime curTime = CTime::GetCurrentTime();
	// 格式化上机时间
	CString sOverTime;
	sOverTime.Format(_T("%d时%d分%d秒"), overSeconds / 3600, (overSeconds % 3600) / 60, overSeconds % 60);
	// 格式化余时
	CString sRemainTime;
	sRemainTime.Format(_T("%d时%d分%d秒"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户退出\r\n上机时间:%s\r\n余时：%s\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, sOverTime, sRemainTime);
	// 指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// 关闭文件
	mFile.Close();
}

CString CRecordHelper::LoadRecords(){
	// 打开文件
	CStdioFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead);
	// 指向开头并循环读入
	mFile.SeekToBegin();
	CString contents, line;
	contents.Empty();
	while (mFile.ReadString(line)) {
		line += "\r\n";
		char *buffer = new char[line.GetLength()];
		for (int i = 0; i < line.GetLength(); i++) {
			buffer[i] = line.GetAt(i);
		}
		line.Empty();
		Char2Unicode(buffer, &line);
		delete[] buffer;
		contents += line;
	}
	// 关闭文件并返回结果
	mFile.Close();
	return contents;
}

BOOL CRecordHelper::EmptyRecords(){
	// 清空文件
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeReadWrite);
	FileUnicodeEncode(mFile);
	mFile.Close();
	return true;
}