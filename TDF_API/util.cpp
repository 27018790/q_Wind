#include "stdafx.h"
#include "util.h"

#include "kdb+.util/type_convert.h"
#include "kdb+.util/Cookbook.inl"

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

unsigned int TDF::util::q2time(K time) throw(std::string) {
	if (time == K_NIL) throw std::string("nil time");

	char hh, mm, ss;
	short millis;
	switch (time->t) {
	case -KT:
		q::Cookbook::tsms(time->i, &hh, &mm, &ss, &millis);
		return hh * 10000 + mm * 100 + ss;
	default:
		throw std::string("not a time");
	}
}

void TDF::DataTypeFlag::registerAll() {
	registerEnumString(DATA_TYPE_TRANSACTION, "Transaction");
	registerEnumString(DATA_TYPE_ORDER, "Order");
	registerEnumString(DATA_TYPE_ORDERQUEUE, "OrderQueue");
}