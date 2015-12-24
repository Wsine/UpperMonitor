#pragma once

#import "./libs/msado15.dll" no_namespace rename("EOF","ADOEOF") rename("BOF","ADOBOF")

#define DEFAULTREMAINTIME 20
#define ONTABLE _T("OnTable")
#define REMAINTIMETABLE _T("RemainTimeTable")
#define TIMEFORMAT _T("%Y/%m/%d %H:%M:%S")

// RemainTimeTable
struct RemainTime
{
	CString UID;
	int RemainSeconds;
	RemainTime(CString _UID, int _RemainSeconds){
		UID = _UID;
		RemainSeconds = _RemainSeconds;
	}
};

// OnTable上机表记录结构
struct OnRecord
{
	CString UID;
	int RemainSeconds;
	CString StartTime;
	BOOL isOvertime;
	OnRecord(CString _UID, int _RemainSeconds, CString _StartTime, BOOL _isOverTime = false){
		UID = _UID;
		RemainSeconds = _RemainSeconds;
		StartTime = _StartTime;
		isOvertime = _isOverTime;
	}
};

class CAdoMySQLHelper
{
public:
	// 构造函数
	CAdoMySQLHelper();
	// virtual是因为在析构时防止只析构基类而不析构派生类的情况
	virtual ~CAdoMySQLHelper();

	// 连接数据库
	BOOL MySQL_Connect();
	// 关闭数据库
	void MySQL_Close();

	// 数据库增删改查操作函数
	// 对数据进行插入操作
	BOOL MySQL_Insert(RemainTime record);
	BOOL MySQL_Insert(OnRecord record);
	// 对数据进行删除操作
	BOOL MySQL_Delete(CString uid, CString table);
	// 对数据库进行更新操作
	BOOL MySQL_UpdateRemainTime(CString uid, int updateTime, CString table);
	// 对数据库进行查询操作, 查询条件cond = " UID='xxxx' "
	void* MySQL_Query(CString cond, CString table);
	// 对数据库进行查询操作, 查询UID是否存在表中
	BOOL MySQL_QueryByUID(CString uid, CString table);
	// 定时扫描OnTable, 使用扫描周期Timer更新当前上机用户余时，捕捉超时用户
	void MySQL_ScanOnTable(int timer);

private:
	// 保存打开的数据库连接智能指针
	_ConnectionPtr m_pConnection;
};
