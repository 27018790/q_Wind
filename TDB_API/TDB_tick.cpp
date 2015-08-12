#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "win32.util/Singleton.h"
#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldMapper.h"

namespace TDB {
	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TDBDefine_TickAB.
		struct TickAB : public Wind::mapper::Fields<::TDBDefine_TickAB> {
			typedef Singleton<TickAB> accessor_map;
			typedef ::TDBDefine_TickAB tdb_result_type;

			TickAB() : Wind::mapper::Fields<tdb_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdb_result_type, FieldT>;
			template <typename FieldT>
			using FloatsAccessor = Wind::accessor::FloatsAccessor<tdb_result_type, FieldT>;

			// Data fields -- always keep in sync with TDBDefine_TickAB
			void registerAllFields() {
				// ��ô���(AG1312.SHF)
				addField("WindCode", new SymbolAccessor(&tdb_result_type::chWindCode));
				// ����������(ag1312)
				addField("Code", new SymbolAccessor(&tdb_result_type::chCode));
				// ���ڣ���Ȼ�գ���ʽYYMMDD
				addField("Date", new DateAccessor(&tdb_result_type::nDate));
				// ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
				addField("Time", new TimeAccessor(&tdb_result_type::nTime));
				// �ɽ��۸�
				addField("Price", new FloatAccessor<int>(&tdb_result_type::nPrice, .0001));
				// �ɽ�����
				addField("Size", new FloatAccessor<__int64>(&tdb_result_type::iVolume));
				// �ɽ���(Ԫ)
				addField("Amount", new FloatAccessor<__int64>(&tdb_result_type::iTurover));
				// �ɽ�����
				addField("MatchCount", new IntAccessor(&tdb_result_type::nMatchItems));
				// IOPV(����)����Ϣ(ծȯ)
				addField("Interest", new FloatAccessor<int>(&tdb_result_type::nInterest));
				// �ɽ���־
				addField("TradeFlag", new CharAccessor(&tdb_result_type::chTradeFlag));
				// BS��־
				addField("Side", new CharAccessor(&tdb_result_type::chBSFlag));
				// �����ۼƳɽ���
				addField("AccumSize", new FloatAccessor<__int64>(&tdb_result_type::iAccVolume));
				// ���ճɽ���(Ԫ)
				addField("AccumAmount", new FloatAccessor<__int64>(&tdb_result_type::iAccTurover));
				// ���
				addField("High", new FloatAccessor<int>(&tdb_result_type::nHigh, .0001));
				// ���
				addField("Low", new FloatAccessor<int>(&tdb_result_type::nLow, .0001));
				// ����
				addField("Open", new FloatAccessor<int>(&tdb_result_type::nOpen, .0001));
				// ǰ����
				addField("PreClose", new FloatAccessor<int>(&tdb_result_type::nPreClose, .0001));
				// ������
				addField("Asks", new FloatsAccessor<int[10]>(&tdb_result_type::nAskPrice, .0001));
				// ������
				addField("AskSizes", new FloatsAccessor<unsigned int[10]>(&tdb_result_type::nAskVolume));
				// �����
				addField("Bids", new FloatsAccessor<int[10]>(&tdb_result_type::nBidPrice, .0001));
				// ������
				addField("BidSizes", new FloatsAccessor<unsigned int[10]>(&tdb_result_type::nBidVolume));
				// ��Ȩƽ��������(�Ϻ�L2)
				addField("AvgAsk", new FloatAccessor<int>(&tdb_result_type::nAskAvPrice, .0001));
				// ��Ȩƽ�������(�Ϻ�L2)
				addField("AvgBid", new FloatAccessor<int>(&tdb_result_type::nBidAvPrice, .0001));
				// ��������(�Ϻ�L2)
				addField("TotalAskSize", new FloatAccessor<__int64>(&tdb_result_type::iTotalAskVolume));
				// ��������(�Ϻ�L2)
				addField("TotalBidSize", new FloatAccessor<__int64>(&tdb_result_type::iTotalBidVolume));
				// ��ָ��������Ȩָ��
				addField("Index", new IntAccessor(&tdb_result_type::nIndex));
				// ��ָ����Ʒ������
				addField("StockCount", new IntAccessor(&tdb_result_type::nStocks));
				// ��ָ��������Ʒ����
				addField("UpCount", new IntAccessor(&tdb_result_type::nUps));
				// ��ָ�����µ�Ʒ����
				addField("DownCount", new IntAccessor(&tdb_result_type::nDowns));
				// ��ָ������ƽƷ����
				addField("FlatCount", new IntAccessor(&tdb_result_type::nHoldLines));
			}
		};

		// Traits to assist symbolic retrieval of tick data fields from TDBDefine_FutureAB.
		struct FutureAB : public Wind::mapper::Fields<::TDBDefine_FutureAB> {
			typedef Singleton<FutureAB> accessor_map;
			typedef ::TDBDefine_FutureAB tdb_result_type;

			FutureAB() : Wind::mapper::Fields<tdb_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdb_result_type, FieldT>;
			template <typename FieldT>
			using FloatsAccessor = Wind::accessor::FloatsAccessor<tdb_result_type, FieldT>;

			// Data fields -- always keep in sync with TDBDefine_FutureAB
			void registerAllFields() {
				// ��ô���(AG1312.SHF)
				addField("WindCode", new SymbolAccessor(&tdb_result_type::chWindCode));
				// ����������(ag1312)
				addField("Code", new SymbolAccessor(&tdb_result_type::chCode));
				// ���ڣ���Ȼ�գ���ʽYYMMDD
				addField("Date", new DateAccessor(&tdb_result_type::nDate));
				// ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
				addField("Time", new TimeAccessor(&tdb_result_type::nTime));
				// �ɽ�����
				addField("Size", new FloatAccessor<__int64>(&tdb_result_type::iVolume));
				// �ɽ���(Ԫ)
				addField("Amount", new FloatAccessor<__int64>(&tdb_result_type::iTurover));
				// �����
				addField("SettlePrice", new FloatAccessor<int>(&tdb_result_type::nSettle, .0001));
				// �ֲ���
				addField("Position", new FloatAccessor<int>(&tdb_result_type::nPosition));
				// ��ʵ��
				addField("Delta", new FloatAccessor<int>(&tdb_result_type::nCurDelta));
				// �ɽ���־
				addField("TradeFlag", new CharAccessor(&tdb_result_type::chTradeFlag));
				// �����ۼƳɽ���
				addField("AccumSize", new FloatAccessor<__int64>(&tdb_result_type::iAccVolume));
				// ���ճɽ���(Ԫ)
				addField("AccumAmount", new FloatAccessor<__int64>(&tdb_result_type::iAccTurover));
				// ���
				addField("High", new FloatAccessor<int>(&tdb_result_type::nHigh, .0001));
				// ���
				addField("Low", new FloatAccessor<int>(&tdb_result_type::nLow, .0001));
				// ����
				addField("Open", new FloatAccessor<int>(&tdb_result_type::nOpen, .0001));
				// �ɽ���
				addField("Price", new FloatAccessor<int>(&tdb_result_type::nPrice, .0001));
				// ������
				addField("Asks", new FloatsAccessor<int[5]>(&tdb_result_type::nAskPrice, .0001));
				// ������
				addField("AskSizes", new FloatsAccessor<unsigned int[5]>(&tdb_result_type::nAskVolume));
				// �����
				addField("Bids", new FloatsAccessor<int[5]>(&tdb_result_type::nBidPrice, .0001));
				// ������
				addField("BidSizes", new FloatsAccessor<unsigned int[5]>(&tdb_result_type::nBidVolume));
				// ǰ����
				addField("PreClose", new FloatAccessor<int>(&tdb_result_type::nPreClose, .0001));
				// �����
				addField("PreSettlePrice", new FloatAccessor<int>(&tdb_result_type::nPreSettle, .0001));
				// ��ֲ�
				addField("PrePosition", new FloatAccessor<int>(&tdb_result_type::nPrePosition));
			}
		};

	}//namespace TDB::traits
}//namespace TDB

TDB_API K K_DECL TDB_tickAB_fields(K _) {
	return TDB::traits::TickAB::accessor_map::getInstance()->getFields();
}

TDB_API K K_DECL TDB_tickAB(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::traits::TickAB::field_accessor const*> indis;
	::TDBDefine_ReqTick req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::traits::TickAB>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::traits::TickAB, ::TDBDefine_ReqTick>(tdb, req, indis, &::TDB_GetTickAB);
}

TDB_API K K_DECL TDB_futureAB_fields(K _) {
	return TDB::traits::FutureAB::accessor_map::getInstance()->getFields();
}

TDB_API K K_DECL TDB_futureAB(K h, K windCode, K indicators, K begin, K end, K autoComplete) {
	::THANDLE tdb = NULL;
	std::vector<TDB::traits::FutureAB::field_accessor const*> indis;
	::TDBDefine_ReqFuture req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::traits::FutureAB>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
		req.nAutoComplete = !!q::q2Dec(autoComplete) ? 1 : 0;
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::traits::FutureAB, ::TDBDefine_ReqFuture>(tdb, req, indis, &::TDB_GetFutureAB);
}