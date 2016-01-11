#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include <algorithm>
#include <locale>
#include <set>
#include "win32.util/Singleton.h"
#include "kdb+.util/util.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldMapper.h"

namespace TDB {
	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TDBDefine_Tick.
		struct Tick : public Wind::mapper::Fields<::TDBDefine_Tick> {
			typedef Singleton<Tick> accessor_map;
			typedef ::TDBDefine_Tick tdb_result_type;

			Tick() : Wind::mapper::Fields<tdb_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdb_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdb_result_type, FieldT>;
			template <typename FieldT>
			using FloatsAccessor = Wind::accessor::FloatsAccessor<tdb_result_type, FieldT>;

			// Data fields -- always keep in sync with TDBDefine_Tick
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

				registerFuturesFields();
				registerBidAskFields();
				registerIndexFields();

				addField("RESERVED", "reserve1", new FloatAccessor<int>(&tdb_result_type::nResv1));
				addField("RESERVED", "reserve2", new FloatAccessor<int>(&tdb_result_type::nResv2));
				addField("RESERVED", "reserve3", new FloatAccessor<int>(&tdb_result_type::nResv3));
			}

			void registerFuturesFields() {
				// �����
				addField("FUTURES", "SettlePrice", new FloatAccessor<int>(&tdb_result_type::nSettle, .0001));
				// �ֲ���
				addField("FUTURES", "Position", new FloatAccessor<int>(&tdb_result_type::nPosition));
				// ��ʵ��
				addField("FUTURES", "Delta", new FloatAccessor<int>(&tdb_result_type::nCurDelta));
				// �����
				addField("FUTURES", "PreSettlePrice", new FloatAccessor<int>(&tdb_result_type::nPreSettle, .0001));
				// ��ֲ�
				addField("FUTURES", "PrePosition", new FloatAccessor<int>(&tdb_result_type::nPrePosition));
			}

			void registerBidAskFields() {
				// ������
				addField("BIDASK", "Asks", new FloatsAccessor<int[10]>(&tdb_result_type::nAskPrice, .0001));
				// ������
				addField("BIDASK", "AskSizes", new FloatsAccessor<unsigned int[10]>(&tdb_result_type::nAskVolume));
				// �����
				addField("BIDASK", "Bids", new FloatsAccessor<int[10]>(&tdb_result_type::nBidPrice, .0001));
				// ������
				addField("BIDASK", "BidSizes", new FloatsAccessor<unsigned int[10]>(&tdb_result_type::nBidVolume));
				// ��Ȩƽ��������(�Ϻ�L2)
				addField("BIDASK", "AvgAsk", new FloatAccessor<int>(&tdb_result_type::nAskAvPrice, .0001));
				// ��Ȩƽ�������(�Ϻ�L2)
				addField("BIDASK", "AvgBid", new FloatAccessor<int>(&tdb_result_type::nBidAvPrice, .0001));
				// ��������(�Ϻ�L2)
				addField("BIDASK", "TotalAskSize", new FloatAccessor<__int64>(&tdb_result_type::iTotalAskVolume));
				// ��������(�Ϻ�L2)
				addField("BIDASK", "TotalBidSize", new FloatAccessor<__int64>(&tdb_result_type::iTotalBidVolume));
			}

			void registerIndexFields() {
				// ��ָ��������Ȩָ��
				addField("INDEX", "Index", new IntAccessor(&tdb_result_type::nIndex));
				// ��ָ����Ʒ������
				addField("INDEX", "StockCount", new IntAccessor(&tdb_result_type::nStocks));
				// ��ָ��������Ʒ����
				addField("INDEX", "UpCount", new IntAccessor(&tdb_result_type::nUps));
				// ��ָ�����µ�Ʒ����
				addField("INDEX", "DownCount", new IntAccessor(&tdb_result_type::nDowns));
				// ��ָ������ƽƷ����
				addField("INDEX", "HoldCount", new IntAccessor(&tdb_result_type::nHoldLines));
			}
		};

	}//namespace TDB::traits
}//namespace TDB

TDB_API K K_DECL TDB_tick_fields(K category) {
	std::string cat;
	try {
		cat = q::q2String(category);
		std::transform(cat.begin(), cat.end(), cat.begin(),
			[](char x) { return std::toupper(x, std::locale()); }
		);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	auto const mapper = TDB::traits::Tick::accessor_map::getInstance();
	if (!mapper->hasCategory(cat)) {
		std::vector<std::string> categories;
		mapper->getCategories(categories);
		std::ostringstream buffer;
		for (auto c = categories.cbegin(); c != categories.cend(); ++c) {
			buffer << "`" << *c;
		}
		return q::error2q(buffer.str());
	}
	else {
		return mapper->getFields(cat);
	}
}

TDB_API K K_DECL TDB_tick(K h, K windCode, K indicators, K date, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::traits::Tick::field_accessor const*> indis;
	::TDBDefine_ReqTick req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::traits::Tick>(indicators, indis);
		TDB::parseTdbReqCode(tdb, windCode, req);
		TDB::parseTdbReqTime(date, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	req.nAutoComplete = 0;

	return TDB::runQuery<TDB::traits::Tick, ::TDBDefine_ReqTick>(tdb, req, indis, &::TDB_GetTick);
}