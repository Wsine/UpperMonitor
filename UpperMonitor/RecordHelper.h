#pragma once

#define SAVEFILENAME _T("./log/records.txt")
#define TIMEFORMAT _T("%Y/%m/%d %H:%M:%S")

class CRecordHelper {
public:
	CRecordHelper(CString _mSaveFile=SAVEFILENAME);
	virtual ~CRecordHelper();

	// 电子钱包消费 充值 操作记录保存
	void SaveRecharges(CString uid, CString accounts, long remainings, CString result);
	void SaveConsumptions(CString uid, CString accounts, long remainings, CString result);
	// 上机网管上机 退出操作记录保存
	void StartNets(CString uid, int remainTime, CString result);
	void ExitNets(CString uid, int remainTime, int overSeconds, CString result);
	// 加载记录
	CString LoadRecords();
	// 清空记录
	BOOL EmptyRecords();
private:
	// 保存历史记录文件名
	CString mSaveFile;
};

