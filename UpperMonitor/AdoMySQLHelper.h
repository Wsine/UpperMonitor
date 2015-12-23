#pragma once

#import "./libs/msado15.dll" no_namespace rename("EOF","ADOEOF") rename("BOF","ADOBOF")

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

// OnTable�ϻ����¼�ṹ
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
	// ���캯��
	CAdoMySQLHelper();
	// virtual����Ϊ������ʱ��ֹֻ�������������������������
	virtual ~CAdoMySQLHelper();

	// �������ݿ�
	BOOL MySQL_Connect();
	// �ر����ݿ�
	void MySQL_Close();

	// ���ݿ���ɾ�Ĳ��������
	// �����ݽ��в������
	BOOL MySQL_Insert(RemainTime record);
	BOOL MySQL_Insert(OnRecord record);
	// �����ݽ���ɾ������
	BOOL MySQL_Delete(CString uid, CString table);
	// �����ݿ���и��²���
	BOOL MySQL_UpdateRemainTime(CString uid, int updateTime);
	// �����ݿ���в�ѯ����, ��ѯ����cond = " UID='xxxx' "
	void* MySQL_Query(CString cond, CString table);
	// �����ݿ���в�ѯ����, ��ѯUID�Ƿ���ڱ���
	BOOL MySQL_QueryByUID(int uid, CString table);
	// ��ʱɨ��OnTable, ʹ��ɨ������Timer���µ�ǰ�ϻ��û���ʱ����׽��ʱ�û�
	void MySQL_ScanOnTable(int timer);

private:
	// ����򿪵����ݿ���������ָ��
	_ConnectionPtr m_pConnection;
};
