#include "stdafx.h"
#include "AdoMySQLHelper.h"

//CAdoMySQLHelper::CAdoMySQLHelper(){
//	this->MySQL_Connect();
//}
//
//CAdoMySQLHelper::~CAdoMySQLHelper(){
//	this->MySQL_Close();
//}
//
//BOOL CAdoMySQLHelper::MySQL_Connect(){
//	// 初始化OLE/COM库环境
//	CoInitialize(NULL);
//
//	try{
//		// 通过名字创建Connection对象
//		HRESULT hr = this->m_pConnection.CreateInstance("ADODB.Connection");
//		if (FAILED(hr)){
//			AfxMessageBox(_T("创建_ConnectionPtr智能指针失败"));
//			return false;
//		}
//		// 设置连接超时时间
//		this->m_pConnection->ConnectionTimeout = 600;
//		// 设置执行命令超时时间
//		this->m_pConnection->CommandTimeout = 120;
//
//		// 连接数据库
//		this->m_pConnection->Open("DSN=MySQL5.7;Server=localhost;Database=ZD124UE_DEMO",
//			"root",
//			"root",
//			adModeUnknown);
//	}
//	catch (_com_error &e){
//		// 若连接打开，需要在异常处理中关闭和释放连接
//		if ((NULL != this->m_pConnection) && this->m_pConnection->State){
//			this->m_pConnection->Close();
//			this->m_pConnection.Release();
//			this->m_pConnection = NULL;
//		}
//		// 非CView和CDialog需要使用全局函数AfxMessageBox
//		AfxMessageBox(e.Description());
//	}
//	return true;
//}
//
//void CAdoMySQLHelper::MySQL_Close(){
//	if ((NULL != this->m_pConnection) && (this->m_pConnection->State)){
//		this->m_pConnection->Close(); // 关闭连接
//		this->m_pConnection.Release();// 释放连接
//		this->m_pConnection = NULL;
//	}
//
//	// 访问完COM库后，卸载COM库
//	CoUninitialize();
//}
//
//BOOL CAdoMySQLHelper::MySQL_Insert(RemainTime record){
//	_CommandPtr m_pCommand;
//	try{
//		m_pCommand.CreateInstance("ADODB.Command");
//
//		_variant_t vNULL;
//		vNULL.vt = VT_ERROR;
//		// 定义为无参数
//		vNULL.scode = DISP_E_PARAMNOTFOUND;
//		// 将建立的连接赋值给它
//		m_pCommand->ActiveConnection = this->m_pConnection;
//
//		// 将余时字符串化
//		CString temp;
//		temp.Format(_T("%d"), record.RemainSeconds);
//		// SQL语句
//		m_pCommand->CommandText = "insert into RemainTimeTable values(\'"
//			+ (_bstr_t)record.UID + "\',"
//			+ (_bstr_t)temp + ")";
//		// 执行SQL语句
//		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
//	}
//	catch (_com_error &e){
//		// 需要在异常处理中释放命令对象
//		if ((NULL != m_pCommand) && (m_pCommand->State)){
//			m_pCommand.Release();
//			m_pCommand = NULL;
//		}
//		// 非CView和CDialog需要使用全局函数AfxMessageBox
//		AfxMessageBox(e.Description());
//		return false;
//	}
//	return true;
//}
//
//BOOL CAdoMySQLHelper::MySQL_Insert(OnRecord record){
//	_CommandPtr m_pCommand;
//	try{
//		m_pCommand.CreateInstance("ADODB.Command");
//
//		_variant_t vNULL;
//		vNULL.vt = VT_ERROR;
//		// 定义为无参数
//		vNULL.scode = DISP_E_PARAMNOTFOUND;
//		// 将建立的连接赋值给它
//		m_pCommand->ActiveConnection = this->m_pConnection;
//
//		// 将余时字符串化
//		CString temp;
//		temp.Format(_T("%d"), record.RemainSeconds);
//		// 将超时字符串化
//		CString isOverTime;
//		isOverTime = (record.isOvertime) ? "TRUE" : "FALSE";
//		// SQL语句
//		m_pCommand->CommandText = "insert into OnTable values(\'"
//			+ (_bstr_t)record.UID + "\',"
//			+ (_bstr_t)temp + "\',"
//			+ (_bstr_t)record.StartTime + "\',"
//			+ (_bstr_t)isOverTime + ")";
//		// 执行SQL语句
//		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
//	}
//	catch (_com_error &e){
//		// 需要在异常处理中释放命令对象
//		if ((NULL != m_pCommand) && (m_pCommand->State)){
//			m_pCommand.Release();
//			m_pCommand = NULL;
//		}
//		// 非CView和CDialog需要使用全局函数AfxMessageBox
//		AfxMessageBox(e.Description());
//		return false;
//	}
//	return true;
//}
//
//BOOL CAdoMySQLHelper::MySQL_Delete(CString uid, CString table){
//	_CommandPtr m_pCommand;
//	try{
//		m_pCommand.CreateInstance("ADODB.Command");
//
//		_variant_t vNULL;
//		vNULL.vt = VT_ERROR;
//		// 定义为无参数
//		vNULL.scode = DISP_E_PARAMNOTFOUND;
//		// 将建立的连接赋值给它
//		m_pCommand->ActiveConnection = this->m_pConnection;
//
//		// SQL语句
//		m_pCommand->CommandText = "delete from " + (_bstr_t)table + " where UID=\'" + (_bstr_t)uid + "\'";
//		// 执行SQL语句
//		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
//	}
//	catch (_com_error &e){
//		// 需要在异常处理中释放命令对象
//		if ((NULL != m_pCommand) && (m_pCommand->State)){
//			m_pCommand.Release();
//			m_pCommand = NULL;
//		}
//		// 非CView和CDialog需要使用全局函数AfxMessageBox
//		AfxMessageBox(e.Description());
//		return false;
//	}
//	return true;
//}
//
//BOOL CAdoMySQLHelper::MySQL_UpdateRemainTime(CString uid, int updateTime){
//	_CommandPtr m_pCommand;
//	try{
//		m_pCommand.CreateInstance("ADODB.Command");
//
//		_variant_t vNULL;
//		vNULL.vt = VT_ERROR;
//		// 定义为无参数
//		vNULL.scode = DISP_E_PARAMNOTFOUND;
//		// 将建立的连接赋值给它
//		m_pCommand->ActiveConnection = this->m_pConnection;
//
//		// 将更新时间字符串化
//		CString str_updateTime;
//		str_updateTime.Format(_T("%d"), updateTime);
//		// SQL语句
//		m_pCommand->CommandText = "update RemainTimeTable set RemainTime=" 
//			+ (_bstr_t)str_updateTime + " where UID=\'" 
//			+ (_bstr_t)uid + "\'";
//		// 执行SQL语句
//		m_pCommand->Execute(&vNULL, &vNULL, adCmdText);
//	}
//	catch (_com_error &e){
//		// 需要在异常处理中释放命令对象
//		if ((NULL != m_pCommand) && (m_pCommand->State)){
//			m_pCommand.Release();
//			m_pCommand = NULL;
//		}
//		// 非CView和CDialog需要使用全局函数AfxMessageBox
//		AfxMessageBox(e.Description());
//		return false;
//	}
//	return true;
//}
//
//void* CAdoMySQLHelper::MySQL_Query(CString cond, CString table){
//	// 打开数据集SQL语句
//	_variant_t sql = "SELECT * FROM " + (_bstr_t)table + "WHERE " + (_bstr_t)cond;
//	
//	int uid = -1;
//	try{
//		// 定义_RecordsetPtr智能指针
//		_RecordsetPtr m_pRecordset;
//		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
//		if (FAILED(hr)){
//			AfxMessageBox(_T("创建_RecordsetPtr智能指针失败"));
//			return (void*)false;
//		}
//		//  打开连接，获取数据集
//		m_pRecordset->Open(sql,
//			_variant_t((IDispatch*)(this->m_pConnection), true),
//			adOpenForwardOnly,
//			adLockReadOnly,
//			adCmdText);
//		// 确定表不为空
//		if (!m_pRecordset->ADOEOF){
//			// 移动游标到最前
//			m_pRecordset->MoveFirst();
//			// 循环遍历数据集
//			while (!m_pRecordset->ADOEOF){
//				_variant_t varRemainTime = m_pRecordset->Fields->GetItem(_T("RemainTime"))->GetValue();
//				varRemainTime.ChangeType(VT_INT);
//				int varRemainTimeInt = varRemainTime.intVal;
//				// BUG: Let another get more seconds to live
//				if (varRemainTimeInt <= 0){
//					_variant_t varUID = m_pRecordset->Fields->GetItem(_T("UID"))->GetValue();
//					varUID.ChangeType(VT_BSTR);
//					CString strUID = varUID.bstrVal;
//					uid = _ttoi(strUID);
//					break;
//				}
//				m_pRecordset->MoveNext();
//			}
//		}
//	}
//	catch (_com_error &e){
//		AfxMessageBox(e.Description());
//	}
//	return (void*)uid;
//}
//
//BOOL CAdoMySQLHelper::MySQL_QueryByUID(int uid, CString table){
//	// 字符串化UID
//	CString str_uid;
//	str_uid.Format(_T("%d"), uid);
//	// 打开数据集SQL语句
//	_variant_t sql = "SELECT * FROM " + (_bstr_t)table
//				   + " WHERE UID=\'" + (_bstr_t)str_uid + "/'";
//	bool isExist = false;
//	try{
//		// 定义_RecordsetPtr智能指针
//		_RecordsetPtr m_pRecordset;
//		HRESULT hr = m_pRecordset.CreateInstance(__uuidof(Recordset));
//		if (FAILED(hr)){
//			AfxMessageBox(_T("创建_RecordsetPtr智能指针失败"));
//			return false;
//		}
//		//  打开连接，获取数据集
//		m_pRecordset->Open(sql,
//						   _variant_t((IDispatch*)(this->m_pConnection), true),
//						   adOpenForwardOnly,
//						   adLockReadOnly,
//						   adCmdText);
//		// 确定表不为空
//		if (!m_pRecordset->ADOEOF){
//			// 由于UID是主键，表不为空即为存在
//			isExist = true;
//		}
//	}
//	catch (_com_error &e){
//		AfxMessageBox(e.Description());
//	}
//	return isExist;
//}
//
//void CAdoMySQLHelper::MySQL_ScanOnTable(int timer) {
//
//}
