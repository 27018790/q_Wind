#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/EnumUtil.h"
#include "Wind.util/FieldAccessors.h"

namespace TDB {

	// Heper to assist symbolic retrieval of tick data fields from TDBDefine_TickAB.
	struct TickAB {

		// Data fields -- always keep in sync with TDBDefine_TickAB
		enum Field {
			NIL = 0,		//`NULL' for this enum
			WindCode,		//��ô���(AG1312.SHF)
			Code,			//����������(ag1312)
			Date,			//���ڣ���Ȼ�գ���ʽYYMMDD
			Time,			//ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
			Price,			//�ɽ���
			Volume,			//�ɽ���
			Turover,		//�ɽ���(Ԫ)
			MatchItems,		//�ɽ�����
			Interest,		//IOPV(����)����Ϣ(ծȯ)
			TradeFlag,		//�ɽ���־
			BSFlag,			//BS��־
			AccVolume,		//�����ۼƳɽ���
			AccTurover,		//���ճɽ���(Ԫ)
			High,			//���
			Low,			//���
			Open,			//����
			PreClose,		//ǰ����
			AskPrices,		//������
			AskVolumes,		//������
			BidPrices,		//�����
			BidVolumes,		//������
			AskAvPrice,		//��Ȩƽ��������(�Ϻ�L2)
			BidAvPrice,		//��Ȩƽ�������(�Ϻ�L2)
			TotalAskVolume,	//��������(�Ϻ�L2)
			TotalBidVolume,	//��������(�Ϻ�L2)
			Index,			//��ָ��������Ȩָ��
			Stocks,			//��ָ����Ʒ������
			Ups,			//��ָ��������Ʒ����
			Downs,			//��ָ�����µ�Ʒ����
			HoldLines,		//��ָ������ƽƷ����
		};

		typedef ::TDBDefine_TickAB tdb_result_type;
		typedef Wind::accessor::FieldAccessor<tdb_result_type> field_accessor_type;
		typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor_;
		typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor_;
		typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor_;
		typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor_;
		template <typename Str>
		using SymbolAccessor_ = Wind::accessor::SymbolAccessor<tdb_result_type, Str>;
		template <typename Val>
		using FloatAccessor_ = Wind::accessor::FloatAccessor<tdb_result_type, Val>;
		template <typename Vals>
		using FloatsAccessor_ = Wind::accessor::FloatsAccessor<tdb_result_type, Vals>;

		static std::map<Field, std::unique_ptr<field_accessor_type> > Accessors;

		// Data field names
		struct FieldName : public enum_util::EnumBase<FieldName, Field>
		{
			static void registerAll() {
#				define TICKAB_FIELD(name, accessor)	\
					ENUM_STRING(name);	\
					TickAB::Accessors.insert(std::make_pair(name, std::unique_ptr<field_accessor_type>(accessor)))

				TICKAB_FIELD(WindCode, new SymbolAccessor_<char[32]>(&tdb_result_type::chWindCode));
				TICKAB_FIELD(Code, new SymbolAccessor_<char[32]>(&tdb_result_type::chCode));
				TICKAB_FIELD(Date, new DateAccessor_(&tdb_result_type::nDate));
				TICKAB_FIELD(Time, new TimeAccessor_(&tdb_result_type::nTime));
				TICKAB_FIELD(Price, new FloatAccessor_<int>(&tdb_result_type::nPrice, .0001));
				TICKAB_FIELD(Volume, new FloatAccessor_<__int64>(&tdb_result_type::iVolume));
				TICKAB_FIELD(Turover, new FloatAccessor_<__int64>(&tdb_result_type::iTurover));
				TICKAB_FIELD(MatchItems, new IntAccessor_(&tdb_result_type::nMatchItems));
				TICKAB_FIELD(Interest, new FloatAccessor_<int>(&tdb_result_type::nInterest));
				TICKAB_FIELD(TradeFlag, new CharAccessor_(&tdb_result_type::chTradeFlag));
				TICKAB_FIELD(BSFlag, new CharAccessor_(&tdb_result_type::chBSFlag));
				TICKAB_FIELD(AccVolume, new FloatAccessor_<__int64>(&tdb_result_type::iAccVolume));
				TICKAB_FIELD(AccTurover, new FloatAccessor_<__int64>(&tdb_result_type::iAccTurover));
				TICKAB_FIELD(High, new FloatAccessor_<int>(&tdb_result_type::nHigh, .0001));
				TICKAB_FIELD(Low, new FloatAccessor_<int>(&tdb_result_type::nLow, .0001));
				TICKAB_FIELD(Open, new FloatAccessor_<int>(&tdb_result_type::nOpen, .0001));
				TICKAB_FIELD(PreClose, new FloatAccessor_<int>(&tdb_result_type::nPreClose, .0001));
				TICKAB_FIELD(AskPrices, new FloatsAccessor_<int[10]>(&tdb_result_type::nAskPrice, .0001));
				TICKAB_FIELD(AskVolumes, new FloatsAccessor_<unsigned int[10]>(&tdb_result_type::nAskVolume));
				TICKAB_FIELD(BidPrices, new FloatsAccessor_<int[10]>(&tdb_result_type::nBidPrice, .0001));
				TICKAB_FIELD(BidVolumes, new FloatsAccessor_<unsigned int[10]>(&tdb_result_type::nBidVolume));
				TICKAB_FIELD(AskAvPrice, new FloatAccessor_<int>(&tdb_result_type::nAskAvPrice, .0001));
				TICKAB_FIELD(BidAvPrice, new FloatAccessor_<int>(&tdb_result_type::nBidAvPrice, .0001));
				TICKAB_FIELD(TotalAskVolume, new FloatAccessor_<__int64>(&tdb_result_type::iTotalAskVolume));
				TICKAB_FIELD(TotalBidVolume, new FloatAccessor_<__int64>(&tdb_result_type::iTotalBidVolume));
				TICKAB_FIELD(Index, new IntAccessor_(&tdb_result_type::nIndex));
				TICKAB_FIELD(Stocks, new IntAccessor_(&tdb_result_type::nStocks));
				TICKAB_FIELD(Ups, new IntAccessor_(&tdb_result_type::nUps));
				TICKAB_FIELD(Downs, new IntAccessor_(&tdb_result_type::nDowns));
				TICKAB_FIELD(HoldLines, new IntAccessor_(&tdb_result_type::nHoldLines));
			}
		};
	};

	// Heper to assist symbolic retrieval of tick data fields from TDBDefine_FutureAB.
	struct FutureAB {

		// Data fields -- always keep in sync with TDBDefine_FutureAB
		enum Field {
			NIL = 0,		//`NULL' for this enum
			WindCode,		//��ô���(AG1312.SHF)
			Code,			//����������(ag1312)
			Date,			//���ڣ���Ȼ�գ���ʽ��YYMMDD
			Time,			//ʱ�䣨��ȷ�����룬HHMMSSmmm��
			Volume,			//�ɽ���
			Turnover,		//�ɽ���(Ԫ)
			Settle,			//�����
			Position,		//�ֲ���
			CurDelta,		//��ʵ��
			TradeFlag,		//�ɽ���־
			AccVolume,		//�����ۼƳɽ���
			AccTurnover,	//���ճɽ���(Ԫ)
			Open,			//����
			High,			//���
			Low,			//���
			Price,			//�ɽ���
			AskPrices,		//������
			AskVolumes,		//������
			BidPrices,		//�����
			BidVolumes,		//������
			PreClose,		//ǰ����
			PreSettle,		//�����
			PrePosition,	//��ֲ�
		};

		typedef ::TDBDefine_FutureAB tdb_result_type;
		typedef Wind::accessor::FieldAccessor<tdb_result_type> field_accessor_type;
		typedef Wind::accessor::CharAccessor<tdb_result_type> CharAccessor_;
		typedef Wind::accessor::DateAccessor<tdb_result_type> DateAccessor_;
		typedef Wind::accessor::TimeAccessor<tdb_result_type> TimeAccessor_;
		typedef Wind::accessor::IntAccessor<tdb_result_type, I> IntAccessor_;
		template <typename Str>
		using SymbolAccessor_ = Wind::accessor::SymbolAccessor<tdb_result_type, Str>;
		template <typename Val>
		using FloatAccessor_ = Wind::accessor::FloatAccessor<tdb_result_type, Val>;
		template <typename Vals>
		using FloatsAccessor_ = Wind::accessor::FloatsAccessor<tdb_result_type, Vals>;

		static std::map<Field, std::unique_ptr<field_accessor_type> > Accessors;

		// Data field names
		struct FieldName : public enum_util::EnumBase<FieldName, Field>
		{
			static void registerAll() {
#				define FUTUREAB_FIELD(name, accessor)	\
					ENUM_STRING(name);	\
					FutureAB::Accessors.insert(std::make_pair(name, std::unique_ptr<field_accessor_type>(accessor)))

				FUTUREAB_FIELD(WindCode, new SymbolAccessor_<char[32]>(&tdb_result_type::chWindCode));
				FUTUREAB_FIELD(Code, new SymbolAccessor_<char[32]>(&tdb_result_type::chCode));
				FUTUREAB_FIELD(Date, new DateAccessor_(&tdb_result_type::nDate));
				FUTUREAB_FIELD(Time, new TimeAccessor_(&tdb_result_type::nTime));
				FUTUREAB_FIELD(Volume, new FloatAccessor_<__int64>(&tdb_result_type::iVolume));
				FUTUREAB_FIELD(Turnover, new FloatAccessor_<__int64>(&tdb_result_type::iTurover));
				FUTUREAB_FIELD(Settle, new FloatAccessor_<int>(&tdb_result_type::nSettle, .0001));
				FUTUREAB_FIELD(Position, new FloatAccessor_<int>(&tdb_result_type::nPosition));
				FUTUREAB_FIELD(CurDelta, new FloatAccessor_<int>(&tdb_result_type::nCurDelta));
				FUTUREAB_FIELD(TradeFlag, new CharAccessor_(&tdb_result_type::chTradeFlag));
				FUTUREAB_FIELD(AccVolume, new FloatAccessor_<__int64>(&tdb_result_type::iAccVolume));
				FUTUREAB_FIELD(AccTurnover, new FloatAccessor_<__int64>(&tdb_result_type::iAccTurover));
				FUTUREAB_FIELD(Open, new FloatAccessor_<int>(&tdb_result_type::nOpen, .0001));
				FUTUREAB_FIELD(High, new FloatAccessor_<int>(&tdb_result_type::nHigh, .0001));
				FUTUREAB_FIELD(Low, new FloatAccessor_<int>(&tdb_result_type::nLow, .0001));
				FUTUREAB_FIELD(Price, new FloatAccessor_<int>(&tdb_result_type::nPrice, .0001));
				FUTUREAB_FIELD(AskPrices, new FloatsAccessor_<int[5]>(&tdb_result_type::nAskPrice, .0001));
				FUTUREAB_FIELD(AskVolumes, new FloatsAccessor_<unsigned int[5]>(&tdb_result_type::nAskVolume));
				FUTUREAB_FIELD(BidPrices, new FloatsAccessor_<int[5]>(&tdb_result_type::nBidPrice, .0001));
				FUTUREAB_FIELD(BidVolumes, new FloatsAccessor_<unsigned int[5]>(&tdb_result_type::nBidVolume));
				FUTUREAB_FIELD(PreClose, new FloatAccessor_<int>(&tdb_result_type::nPreClose, .0001));
				FUTUREAB_FIELD(PreSettle, new FloatAccessor_<int>(&tdb_result_type::nPreSettle, .0001));
				FUTUREAB_FIELD(PrePosition, new FloatAccessor_<int>(&tdb_result_type::nPrePosition));
			}
		};
	};

}//namespace TDB


std::map<TDB::TickAB::Field, std::unique_ptr<TDB::TickAB::field_accessor_type> >
	TDB::TickAB::Accessors;

TDB_API K K_DECL TDB_tickAB_fields(K _) {
	return TDB::getFields<TDB::TickAB>();
}

TDB_API K K_DECL TDB_tickAB(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::TickAB::field_accessor_type const*> indis;
	::TDBDefine_ReqTick req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::TickAB>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::TickAB, ::TDBDefine_ReqTick>(tdb, req, indis, &::TDB_GetTickAB);
}


std::map<TDB::FutureAB::Field, std::unique_ptr<TDB::FutureAB::field_accessor_type> >
TDB::FutureAB::Accessors;

TDB_API K K_DECL TDB_futureAB_fields(K _) {
	return TDB::getFields<TDB::FutureAB>();
}

TDB_API K K_DECL TDB_futureAB(K h, K windCode, K indicators, K begin, K end, K autoComplete) {
	::THANDLE tdb = NULL;
	std::vector<TDB::FutureAB::field_accessor_type const*> indis;
	::TDBDefine_ReqFuture req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::FutureAB>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
		req.nAutoComplete = !!q::q2Dec(autoComplete) ? 1 : 0;
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::FutureAB, ::TDBDefine_ReqFuture>(tdb, req, indis, &::TDB_GetFutureAB);
}