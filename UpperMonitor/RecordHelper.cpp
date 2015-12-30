#include "stdafx.h"
#include "RecordHelper.h"


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
	curTime.Format(TIMEFORMAT);
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\n时间：%s\n结果：%s\n内容：用户充值\n金额：%s\n余额：%d\n"),
		uid, curTime, result, accounts, remainings);
	// 指向文件开头并写入
	mFile.SeekToBegin();
	mFile.Write(contents, contents.GetLength());
	// 关闭文件
	mFile.Close();
}

void CRecordHelper::SaveConsumptions(CString uid, CString accounts, long remainings, CString result){
	// 打开文件
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// 获取当前时间
	CTime curTime = CTime::GetCurrentTime();
	curTime.Format(TIMEFORMAT);
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\n时间：%s\n结果：%s\n内容：用户消费\n金额：%s\n余额：%d\n"),
		uid, curTime, result, accounts, remainings);
	// 指向文件开头并写入
	mFile.SeekToBegin();
	mFile.Write(contents, contents.GetLength());
	// 关闭文件
	mFile.Close();
}

void CRecordHelper::StartNets(CString uid, int remainTime, CString result){
	// 打开文件
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// 获取当前时间
	CTime curTime = CTime::GetCurrentTime();
	curTime.Format(TIMEFORMAT);
	// 格式化余时
	CString sRemainTime;
	sRemainTime.Format(_T("%d时%d分%d秒"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\n时间：%s\n结果：%s\n内容：用户上机\n余时：%s\n"), 
						uid, curTime, result, sRemainTime);
	// 指向文件开头并写入
	mFile.SeekToBegin();
	mFile.Write(contents, contents.GetLength());
	// 关闭文件
	mFile.Close();
}

void CRecordHelper::ExitNets(CString uid, int remainTime, int overSeconds, CString result){
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	// 获取当前时间
	CTime curTime = CTime::GetCurrentTime();
	curTime.Format(TIMEFORMAT);
	// 格式化上机时间
	CString sOverTime;
	sOverTime.Format(_T("%d时%d分%d秒"), overSeconds / 3600, (overSeconds % 3600) / 60, overSeconds % 60);
	// 格式化余时
	CString sRemainTime;
	sRemainTime.Format(_T("%d时%d分%d秒"), remainTime / 3600, (remainTime % 3600) / 60, remainTime % 60);
	// 格式化输出
	CString contents;
	contents.Format(_T("卡号：%s\n时间：%s\n结果：%s\n内容：用户退出\n上机时间:%s\n余时：%s\n"),
		uid, curTime, result, sOverTime, sRemainTime);
	// 指向文件开头并写入
	mFile.SeekToBegin();
	mFile.Write(contents, contents.GetLength());
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
		contents += (line + _T("\r\n"));
	}
	// 关闭文件并返回结果
	mFile.Close();
	return contents;
}

BOOL CRecordHelper::EmptyRecords(){
	// 清空文件
	CFile mFile(this->mSaveFile, CFile::modeCreate | CFile::modeReadWrite);
	//mFile.Write("\xff\xfe", 2); Unicode
	mFile.Close();
	return true;
}