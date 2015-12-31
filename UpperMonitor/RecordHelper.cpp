#include "stdafx.h"
#include "RecordHelper.h"


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
	contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户退出\r\n余时：%s\r\n"),
		uid, curTime.Format(TIMEFORMAT), result, sRemainTime);
	//CString contents; // TODO: 上机时间（耗时）比较难做，待空
	//contents.Format(_T("卡号：%s\r\n时间：%s\r\n结果：%s\r\n内容：用户退出\r\n耗时:%s\r\n余时：%s\r\n"),
	//	uid, curTime.Format(TIMEFORMAT), result, sOverTime, sRemainTime);
	// 指向文件末尾并写入
	mFile.SeekToEnd();
	mFile.Write(contents, wcslen(contents)*sizeof(wchar_t));
	// 关闭文件
	mFile.Close();
}

CString CRecordHelper::LoadRecords(){
	// 打开文件
	CStdioFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeRead | CFile::typeUnicode);
	// 指向开头并循环读入
	mFile.SeekToBegin();
	CString contents, line, multiLine;
	contents.Empty();
	multiLine.Empty();
	//  倒序分段读取
	while (mFile.ReadString(line)) {
		line.Trim();
		if (line.IsEmpty()) {
			contents = multiLine + _T("\r\n") + contents;
			multiLine.Empty();
		}
		else {
			multiLine += (line + _T("\r\n"));
		}
	}
	contents = multiLine + _T("\r\n") + contents;
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