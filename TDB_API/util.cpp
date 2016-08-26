#include "stdafx.h"
#include "util.h"

#include "kdb+.util/types.h"
#include <sstream>

std::string TDB::getError(::TDB_ERROR errorCode) {
	switch (errorCode) {
	case TDB_SUCCESS:
		return "OK";
	case TDB_NETWORK_ERROR:		//�������
		return "network error";
	case TDB_NETWORK_TIMEOUT:	//���糬ʱ
		return "network timeout";
	case TDB_NO_DATA:			//û������
		return "no data";
	case TDB_OUT_OF_MEMORY:		//�ڴ�ľ�
		return "out of memory";
	case TDB_LOGIN_FAILED:		//��½ʧ��
		return "login failed";
	case TDB_INVALID_PARAMS:	//��Ч�Ĳ���(hTdb��Ч��pReqΪ�գ�pCountΪ��)
		return "invalid params";
	case TDB_INVALID_CODE_TYPE:	//��Ч�Ĵ������ͣ�������TDB_GetFuture������ڻ����ʹ��룬����֮��
		return "invalid code type";
	case TDB_WRONG_FORMULA:		//ָ�깫ʽ����
		return "invalid_formula";
	default:
		;
	}
	std::ostringstream buffer;
	buffer << "unknown errorCode=" << errorCode;
	return buffer.str();
}

void TDB::util::fillDateTime(K data, int &date, int &time) throw(std::runtime_error) {
	if (data == K_NIL) {
		throw std::runtime_error("nil date or time or datetime");
	}
	switch (data->t) {
	case -KH:
		if (data->h <= 0) {
			date = time = data->h;
			return;
		}
		break;
	case -KI:
		if (data->i <= 0) {
			date = time = data->i;
			return;
		}
		break;
	case -KJ:
		if (data->j <= 0) {
			date = time = static_cast<int>(data->j);
			return;
		}
		break;
	default:
		;//fall through
	}
	tm2DateTime(q::q2tm(data), date, time);
}

void TDB::util::tm2DateTime(q::tm_ext const& tm, int &date, int &time) {
	date = (1900 + tm.tm_year) * 10000 + (1 + tm.tm_mon) * 100 + tm.tm_mday;
	time = tm.tm_hour * 10000000 + tm.tm_min * 100000 + tm.tm_sec * 1000 + tm.tm_millis;
}