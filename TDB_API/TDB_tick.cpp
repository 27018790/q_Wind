#include "stdafx.h"
#include "TDB_API.h"

#include "util.h"

#include "win32.util/EnumUtil.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"

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

		typedef FieldAccessor<::TDBDefine_TickAB> FieldAccessor_;
		typedef CharAccessor<::TDBDefine_TickAB> CharAccessor_;
		typedef DateAccessor<::TDBDefine_TickAB> DateAccessor_;
		typedef TimeAccessor<::TDBDefine_TickAB> TimeAccessor_;
		typedef IntAccessor<::TDBDefine_TickAB, I> IntAccessor_;
		template <typename Str> using SymbolAccessor_ = SymbolAccessor<::TDBDefine_TickAB, Str>;
		template <typename Val> using FloatAccessor_ = FloatAccessor<::TDBDefine_TickAB, Val>;
		template <typename Vals> using FloatsAccessor_ = FloatsAccessor<::TDBDefine_TickAB, Vals>;

		static std::map<Field, std::unique_ptr<FieldAccessor_> > accessors_;

		// Data field names
		struct FieldName :
			public enum_util::EnumBase<FieldName, Field>
		{
			static void registerAll() {
#				define TICKAB_FIELD(name, accessor)	\
					ENUM_STRING(name);	\
					TickAB::accessors_.insert(std::make_pair(name, std::unique_ptr<FieldAccessor_>(accessor)))

				TICKAB_FIELD(WindCode, new SymbolAccessor_<char[32]>(&::TDBDefine_TickAB::chWindCode));
				TICKAB_FIELD(Code, new SymbolAccessor_<char[32]>(&::TDBDefine_TickAB::chCode));
				TICKAB_FIELD(Date, new DateAccessor_(&::TDBDefine_TickAB::nDate));
				TICKAB_FIELD(Time, new TimeAccessor_(&::TDBDefine_TickAB::nTime));
				TICKAB_FIELD(Price, new FloatAccessor_<int>(&::TDBDefine_TickAB::nPrice, .0001));
				TICKAB_FIELD(Volume, new FloatAccessor_<__int64>(&::TDBDefine_TickAB::iVolume));
				TICKAB_FIELD(Turover, new FloatAccessor_<__int64>(&::TDBDefine_TickAB::iTurover));
				TICKAB_FIELD(MatchItems, new IntAccessor_(&::TDBDefine_TickAB::nMatchItems));
				TICKAB_FIELD(Interest, new FloatAccessor_<int>(&::TDBDefine_TickAB::nInterest));
				TICKAB_FIELD(TradeFlag, new CharAccessor_(&::TDBDefine_TickAB::chTradeFlag));
				TICKAB_FIELD(BSFlag, new CharAccessor_(&::TDBDefine_TickAB::chBSFlag));
				TICKAB_FIELD(AccVolume, new FloatAccessor_<__int64>(&::TDBDefine_TickAB::iAccVolume));
				TICKAB_FIELD(AccTurover, new FloatAccessor_<__int64>(&::TDBDefine_TickAB::iAccTurover));
				TICKAB_FIELD(High, new FloatAccessor_<int>(&::TDBDefine_TickAB::nHigh, .0001));
				TICKAB_FIELD(Low, new FloatAccessor_<int>(&::TDBDefine_TickAB::nLow, .0001));
				TICKAB_FIELD(Open, new FloatAccessor_<int>(&::TDBDefine_TickAB::nOpen, .0001));
				TICKAB_FIELD(PreClose, new FloatAccessor_<int>(&::TDBDefine_TickAB::nPreClose, .0001));
				TICKAB_FIELD(AskPrices, new FloatsAccessor_<int[10]>(&::TDBDefine_TickAB::nAskPrice, .0001));
				TICKAB_FIELD(AskVolumes, new FloatsAccessor_<unsigned[10]>(&::TDBDefine_TickAB::nAskVolume));
				TICKAB_FIELD(BidPrices, new FloatsAccessor_<int[10]>(&::TDBDefine_TickAB::nBidPrice, .0001));
				TICKAB_FIELD(BidVolumes, new FloatsAccessor_<unsigned[10]>(&::TDBDefine_TickAB::nBidVolume));
				TICKAB_FIELD(AskAvPrice, new FloatAccessor_<int>(&::TDBDefine_TickAB::nAskAvPrice, .0001));
				TICKAB_FIELD(BidAvPrice, new FloatAccessor_<int>(&::TDBDefine_TickAB::nBidAvPrice, .0001));
				TICKAB_FIELD(TotalAskVolume, new FloatAccessor_<__int64>(&::TDBDefine_TickAB::iTotalAskVolume));
				TICKAB_FIELD(TotalBidVolume, new FloatAccessor_<__int64>(&::TDBDefine_TickAB::iTotalBidVolume));
				TICKAB_FIELD(Index, new IntAccessor_(&::TDBDefine_TickAB::nIndex));
				TICKAB_FIELD(Stocks, new IntAccessor_(&::TDBDefine_TickAB::nStocks));
				TICKAB_FIELD(Ups, new IntAccessor_(&::TDBDefine_TickAB::nUps));
				TICKAB_FIELD(Downs, new IntAccessor_(&::TDBDefine_TickAB::nDowns));
				TICKAB_FIELD(HoldLines, new IntAccessor_(&::TDBDefine_TickAB::nHoldLines));
			}
		};
	};

}//namespace TDB


std::map<TDB::TickAB::Field, std::unique_ptr<TDB::TickAB::FieldAccessor_> >
	TDB::TickAB::accessors_;

TDB_API K K_DECL TDB_tickAB_fields(K _) {
	std::vector<std::string> const fieldNames = TDB::TickAB::FieldName::getAllStrings();
	q::K_ptr result(ktn(KS, fieldNames.size()));
	for (std::size_t i = 0; i < fieldNames.size(); ++i) {
		kS(result.get())[i] = ss(const_cast<S>(fieldNames[i].c_str()));
	}
	return result.release();
}

TDB_API K K_DECL TDB_tickAB(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<std::string> indis;
	::TDBDefine_ReqTick req = { 0 };
	try {
		tdb = reinterpret_cast<::THANDLE>(q::q2Dec(h));

		std::string const code = q::q2String(windCode);
		std::copy(code.begin(), code.end(), req.chCode);
		req.chCode[code.size()] = '\0';

		indis = q::qList2String(indicators);

		TDB::util::tm2DateTime(q::q2tm(begin), req.nBeginDate, req.nBeginTime);
		TDB::util::tm2DateTime(q::q2tm(end),   req.nEndDate,   req.nEndTime);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}
	if (!tdb) {
		return q::error2q("null THANDLE");
	}

	// Get all requested field accessors
	std::vector<TDB::TickAB::FieldAccessor_ const*> fields;
	fields.reserve(indis.size());
	for (auto i = indis.cbegin(); i != indis.cend(); ++i) {
		auto const f = TDB::TickAB::FieldName::fromString(*i);
		if (f == TDB::TickAB::NIL) {
			return q::error2q(*i);
		}
		fields.push_back(TDB::TickAB::accessors_[f].get());
	}

	// Query
	int tickCount = 0;
	::TDBDefine_TickAB* t = NULL;
	int const result = ::TDB_GetTickAB(tdb, &req, &t, &tickCount);
	TDB::Ptr<::TDBDefine_TickAB> ticks(t);
	if (result != TDB_SUCCESS) {
		return q::error2q(TDB::getError(result));
	}
	assert(tickCount >= 0);
	assert(ticks);

	// Convert each requested field
	q::K_ptr data(ktn(0, fields.size()));
	for (std::size_t i = 0; i < fields.size(); ++i) {
		kK(data.get())[i] = fields[i]->extract(ticks.get(), tickCount);
	}
	return data.release();
}