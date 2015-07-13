#include "stdafx.h"
#include "util.h"

std::string TDF::getError(::TDF_ERR errorCode) {
	switch (errorCode) {
	case TDF_ERR_SUCCESS:				// �ɹ�
		return "OK";
	case TDF_ERR_UNKOWN:				// δ֪����
		return "unknown error";
	case TDF_ERR_INITIALIZE_FAILURE:	// ��ʼ��socket����ʧ��
		return "initialization failure";
	case TDF_ERR_NETWORK_ERROR:			// �������ӳ�������
		return "network error";
	case TDF_ERR_INVALID_PARAMS:		// ���������Ч
		return "invalid params";
	case TDF_ERR_VERIFY_FAILURE:		// ��½��֤ʧ�ܣ�ԭ��Ϊ�û�������������󣻳�����½����
		return "login failure";
	case TDF_ERR_NO_AUTHORIZED_MARKET:	// ����������г���û����Ȩ
		return "market not authorized";
	case TDF_ERR_NO_CODE_TABLE:			// ����������г����춼û�д����
		return "no code table";
	default:
		;
	};
	std::ostringstream buffer;
	buffer << "unknown errorCode=" << errorCode;
	return buffer.str();
}

void TDF::DataTypeFlag::registerAll() {
	registerEnumString(DATA_TYPE_TRANSACTION, "Transaction");
	registerEnumString(DATA_TYPE_ORDER, "Order");
	registerEnumString(DATA_TYPE_ORDERQUEUE, "OrderQueue");
}