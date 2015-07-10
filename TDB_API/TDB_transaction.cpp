#include "stdafx.h"
#include "TDB_API.h"

#include "TDB_API_helper.h"

#include "win32.util/EnumUtil.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"

namespace TDB {

	// Heper to assist symbolic retrieval of tick data fields from TDBDefine_Transaction.
	struct Transaction {

		// Data fields -- always keep in sync with TDBDefine_Transaction
		enum Field {
			NIL = 0,		//`NULL' for this enum
			WindCode,		//��ô���(AG1312.SHF)
			Code,			//����������(ag1312)
			Date,			//���ڣ���Ȼ�գ���ʽYYMMDD
			Time,			//ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
			Index,			//�ɽ����(��1��ʼ������1)
			FunctionCode,	//�ɽ�����: 'C', 0
			OrderKind,		//ί�����
			BSFlag,			//BS��־
			TradePrice,		//�ɽ��۸�
			TradeVolume,	//�ɽ�����
			AskOrder,		//�������
			BidOrder,		//�������
		};

		typedef ::TDBDefine_Transaction tdb_result_type;
		typedef FieldAccessor<tdb_result_type> field_accessor_type;
		typedef CharAccessor<tdb_result_type> CharAccessor_;
		typedef DateAccessor<tdb_result_type> DateAccessor_;
		typedef TimeAccessor<tdb_result_type> TimeAccessor_;
		typedef IntAccessor<tdb_result_type, I> IntAccessor_;
		template <typename Str> using SymbolAccessor_ = SymbolAccessor<tdb_result_type, Str>;
		template <typename Val> using FloatAccessor_ = FloatAccessor<tdb_result_type, Val>;

		static std::map<Field, std::unique_ptr<field_accessor_type> > Accessors;

		// Data field names
		struct FieldName : public enum_util::EnumBase<FieldName, Field>
		{
			static void registerAll() {
#				define TRANSACTION_FIELD(name, accessor)	\
					ENUM_STRING(name);	\
					Transaction::Accessors.insert(std::make_pair(name, std::unique_ptr<field_accessor_type>(accessor)))

				TRANSACTION_FIELD(WindCode, new SymbolAccessor_<char[32]>(&tdb_result_type::chWindCode));
				TRANSACTION_FIELD(Code, new SymbolAccessor_<char[32]>(&tdb_result_type::chCode));
				TRANSACTION_FIELD(Date, new DateAccessor_(&tdb_result_type::nDate));
				TRANSACTION_FIELD(Time, new TimeAccessor_(&tdb_result_type::nTime));
				TRANSACTION_FIELD(Index, new IntAccessor_(&tdb_result_type::nIndex));
				TRANSACTION_FIELD(FunctionCode, new CharAccessor_(&tdb_result_type::chFunctionCode));
				TRANSACTION_FIELD(OrderKind, new CharAccessor_(&tdb_result_type::chOrderKind));
				TRANSACTION_FIELD(BSFlag, new CharAccessor_(&tdb_result_type::chBSFlag));
				TRANSACTION_FIELD(TradePrice, new FloatAccessor_<int>(&tdb_result_type::nTradePrice, .0001));
				TRANSACTION_FIELD(TradeVolume, new FloatAccessor_<int>(&tdb_result_type::nTradeVolume));
				TRANSACTION_FIELD(AskOrder, new IntAccessor_(&tdb_result_type::nAskOrder));
				TRANSACTION_FIELD(BidOrder, new IntAccessor_(&tdb_result_type::nBidOrder));
			}
		};
	};

}//namespace TDB


std::map<TDB::Transaction::Field, std::unique_ptr<TDB::Transaction::field_accessor_type> >
	TDB::Transaction::Accessors;

TDB_API K K_DECL TDB_transaction_fields(K _) {
	return TDB::getFields<TDB::Transaction>();
}

TDB_API K K_DECL TDB_transaction(K h, K windCode, K indicators, K begin, K end) {
	::THANDLE tdb = NULL;
	std::vector<TDB::Transaction::field_accessor_type const*> indis;
	::TDBDefine_ReqTransaction req = { 0 };
	try {
		TDB::parseTdbHandle(h, tdb);
		TDB::parseIndicators<TDB::Transaction>(indicators, indis);
		TDB::parseTdbReq(windCode, begin, end, req);
	}
	catch (std::string const& error) {
		return q::error2q(error);
	}

	return TDB::runQuery<TDB::Transaction, ::TDBDefine_ReqTransaction>(tdb, req, indis, &::TDB_GetTransaction);
}