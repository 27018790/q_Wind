#include "stdafx.h"
#include "TDF_API_handler.h"

#include "util.h"

#include "win32.util/Singleton.h"
#include "Wind.util/FieldMapper.h"

#include <iostream>

namespace TDF {
	namespace util {

		struct LogHeader {
			::THANDLE const tdf_;
			int const connectId_;
			LogHeader(::THANDLE const tdf, int connectId) : tdf_(tdf), connectId_(connectId) {}
		};

		template <typename Char, typename Traits>
		std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os, LogHeader const& info) {
			return os << "<TDF> #" << info.connectId_ << " 0x" << ::util::hexBytes(info.tdf_);
		}

	}//namespace TDF::util
}//namespace TDF

TDF::TickHandler* TDF::TickHandler::getInstance() {
	return singleton_type::getInstance();
}

TDF::TickHandler::TickHandler() {
}

TDF::TickHandler::~TickHandler() {
}

void TDF::TickHandler::DataMsgCallback(::THANDLE tdf, ::TDF_MSG* msg) {
	assert(tdf != NULL);
	assert(msg != NULL);
	assert(msg->pData != NULL);
	switch (msg->nDataType) {
	case MSG_DATA_INDEX:
		processMsg<::TDF_INDEX_DATA>(tdf, *msg, &TickHandler::onDataIndex);
		break;
	case MSG_DATA_MARKET:
		processMsg<::TDF_MARKET_DATA>(tdf, *msg, &TickHandler::onDataMarket);
		break;
	case MSG_DATA_FUTURE:
		processMsg<::TDF_FUTURE_DATA>(tdf, *msg, &TickHandler::onDataFuture);
		break;
	case MSG_DATA_TRANSACTION:
		processMsg<::TDF_TRANSACTION>(tdf, *msg, &TickHandler::onDataTransaction);
		break;
	case MSG_DATA_ORDERQUEUE:
		processMsg<::TDF_ORDER_QUEUE>(tdf, *msg, &TickHandler::onDataOrderQueue);
		break;
	case MSG_DATA_ORDER:
		processMsg<::TDF_ORDER>(tdf, *msg, &TickHandler::onDataOrder);
		break;
	case MSG_DATA_BBQTRANSACTION:
		processMsg<::TDF_BBQTRANSACTION_DATA>(tdf, *msg, &TickHandler::onDataBbqTransaction);
		break;
	case MSG_DATA_BBQBID:
		processMsg<::TDF_BBQBID_DATA>(tdf, *msg, &TickHandler::onDataBbqBid);
		break;
	default:
		std::cerr << util::LogHeader(tdf, msg->nConnectId) << "received unknown data message "
			<< "0x" << ::util::hexBytes(msg) << ", "
			<< "type=" << msg->nDataType << ' '
			<< "(0x" << ::util::hexBytes(msg->pData) << " +" << msg->nDataLen << ')'
			<< std::endl;
	}
}

void TDF::TickHandler::SystemMsgCallback(::THANDLE tdf, ::TDF_MSG* msg) {
	if (tdf == NULL) return;
	if (msg == NULL) return;
	size_t const count = msg->pAppHead->nItemCount;
	size_t const size = msg->pAppHead->nItemSize;
	switch (msg->nDataType) {
	case MSG_SYS_DISCONNECT_NETWORK:
		processMsg(tdf, *msg, &TickHandler::onSysDisconnect);
		break;
	case MSG_SYS_CONNECT_RESULT:
		processMsg<::TDF_CONNECT_RESULT>(tdf, *msg, &TickHandler::onSysConnect);
		break;
	case MSG_SYS_LOGIN_RESULT:
		processMsg<::TDF_LOGIN_RESULT>(tdf, *msg, &TickHandler::onSysLogin);
		break;
	case MSG_SYS_CODETABLE_RESULT:
		processMsg<::TDF_CODE_RESULT>(tdf, *msg, &TickHandler::onSysCodeTable);
		break;
	case MSG_SYS_QUOTATIONDATE_CHANGE:
		processMsg<::TDF_QUOTATIONDATE_CHANGE>(tdf, *msg, &TickHandler::onSysQuoteDateChange);
		break;
	case MSG_SYS_MARKET_CLOSE:
		processMsg<::TDF_MARKET_CLOSE>(tdf, *msg, &TickHandler::onSysMarketClose);
		break;
	case MSG_SYS_HEART_BEAT:
		processMsg(tdf, *msg, &TickHandler::onSysHeartbeat);
		break;
	default:
		std::cerr << util::LogHeader(tdf, msg->nConnectId) << " received unknown system message "
			<< "0x" << ::util::hexBytes(msg) << ", "
			<< "type=" << msg->nDataType << ' '
			<< "(0x" << ::util::hexBytes(msg->pData) << " +" << msg->nDataLen << ')'
			<< std::endl;
	}
}

template <typename Data>
void TDF::TickHandler::processMsg(::THANDLE const tdf, ::TDF_MSG const& msg,
	void(TDF::TickHandler::*processor)(::THANDLE const, int, Data const&))
{
	assert(msg.pAppHead->nItemCount >= 0);
	assert(msg.pAppHead->nItemSize == sizeof(Data));
	assert(msg.pAppHead->nItemCount * msg.pAppHead->nItemSize == msg.nDataLen);
	Data const* contents = static_cast<Data const*>(msg.pData);
	assert(contents != NULL);
	TickHandler* instance = getInstance();
	assert(instance != NULL);
	for (int i = 0; i < msg.pAppHead->nItemCount; ++i, ++contents) {
		(instance->*processor)(tdf, msg.nConnectId, *contents);
	}
}

void TDF::TickHandler::processMsg(::THANDLE const tdf, ::TDF_MSG const& msg,
	void(TDF::TickHandler::*processor)(::THANDLE const, int))
{
	assert(msg.pAppHead->nItemCount == 0);
	//assert(msg.pData == NULL);
	TickHandler* instance = getInstance();
	assert(instance != NULL);
	(instance->*processor)(tdf, msg.nConnectId);
}

namespace TDF {
	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TDBDefine_Order.
		struct Index : public Wind::mapper::Fields<::TDF_INDEX_DATA> {
			typedef Singleton<Index> accessor_map;
			typedef ::TDF_INDEX_DATA tdf_result_type;

			Index() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdf_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdf_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdf_result_type> TimeAccessor;
			typedef Wind::accessor::IntAccessor<tdf_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdf_result_type> CharAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdf_result_type, FieldT>;

			// Data fields -- always keep in sync with TDBDefine_Order
			void registerAllFields() {
				// 600001.SH
				addField("WindCode", new SymbolAccessor(&tdf_result_type::szWindCode));
				// ԭʼCode
				addField("Code", new SymbolAccessor(&tdf_result_type::szCode));
				// ҵ������(��Ȼ��)
				addField("Date", new DateAccessor(&tdf_result_type::nActionDay));
				// ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
				addField("Time", new TimeAccessor(&tdf_result_type::nTime));
				// ������
				addField("TradeDate", new DateAccessor(&tdf_result_type::nTradingDay));
				// ����ָ��
				addField("Open", new FloatAccessor<int>(&tdf_result_type::nOpenIndex));
				// ���ָ��
				addField("High", new FloatAccessor<int>(&tdf_result_type::nHighIndex));
				// ���ָ��
				addField("Low", new FloatAccessor<int>(&tdf_result_type::nLowIndex));
				// ����ָ��
				addField("Last", new FloatAccessor<int>(&tdf_result_type::nLastIndex));
				// ���������Ӧָ���Ľ�������
				addField("Volume", new FloatAccessor<__int64>(&tdf_result_type::iTotalVolume));
				// ���������Ӧָ���ĳɽ����
				addField("Turnover", new FloatAccessor<__int64>(&tdf_result_type::iTurnover));
				// ǰ��ָ��
				addField("PreClose", new FloatAccessor<int>(&tdf_result_type::nPreCloseIndex));
						/*
					CodeType,	//TDF_CODE_INFO: ֤ȯ����
					Name,		//TDF_CODE_INFO: ����֤ȯ����
					*/
			}
		};

	}//nmespace TDF::traits
}//namespace TDF

// ָ������
void TDF::TickHandler::onDataIndex(::THANDLE const tdf, int connectId, ::TDF_INDEX_DATA const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " received TDF_INDEX_DATA 0x" << ::util::hexBytes(&data) << std::endl;
}

// ��������
void TDF::TickHandler::onDataMarket(::THANDLE const tdf, int connectId, ::TDF_MARKET_DATA const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " received TDF_MARKET_DATA 0x" << ::util::hexBytes(&data) << std::endl;
}

// �ڻ�����
void TDF::TickHandler::onDataFuture(::THANDLE const tdf, int connectId, ::TDF_FUTURE_DATA const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " received TDF_FUTURE_DATA 0x" << ::util::hexBytes(&data) << std::endl;
}

// ��ʳɽ�
void TDF::TickHandler::onDataTransaction(::THANDLE const tdf, int connectId, ::TDF_TRANSACTION const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " received TDF_TRANSACTION 0x" << ::util::hexBytes(&data) << std::endl;
}

// ί�ж���
void TDF::TickHandler::onDataOrderQueue(::THANDLE const tdf, int connectId, ::TDF_ORDER_QUEUE const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " received TDF_ORDER_QUEUE 0x" << ::util::hexBytes(&data) << std::endl;
}

// ���ί��
void TDF::TickHandler::onDataOrder(::THANDLE const tdf, int connectId, ::TDF_ORDER const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " received TDF_ORDER 0x" << ::util::hexBytes(&data) << std::endl;
}

// BBQ��ȯ�ɽ�����
void TDF::TickHandler::onDataBbqTransaction(::THANDLE const tdf, int connectId, ::TDF_BBQTRANSACTION_DATA const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " received TDF_BBQTRANSACTION_DATA 0x" << ::util::hexBytes(&data) << std::endl;
}

// BBQ��ȯ��������
void TDF::TickHandler::onDataBbqBid(::THANDLE const tdf, int connectId, ::TDF_BBQBID_DATA const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " received TDF_BBQBID_DATA 0x" << ::util::hexBytes(&data) << std::endl;
}

// ����Ͽ��¼�, ��Ӧ�Ľṹ��ΪNULL
void TDF::TickHandler::onSysDisconnect(::THANDLE const tdf, int connectId) {
	std::cerr << util::LogHeader(tdf, connectId) << " network disconnected!" << std::endl;
}

// �����������ӵĽ��
void TDF::TickHandler::onSysConnect(::THANDLE const tdf, int connectId, ::TDF_CONNECT_RESULT const& data) {
	if (data.nConnResult) {
		std::cout << util::LogHeader(tdf, connectId) << " connected to "
			<< data.szIp << ':' << data.szPort << " as " << data.szUser << '.'
			<< std::endl;
	}
	else {
		std::cerr << util::LogHeader(tdf, connectId) << " connection to "
			<< data.szIp << ':' << data.szPort << " as " << data.szUser << " failed!"
			<< std::endl;
	}
}

//��½Ӧ��
void TDF::TickHandler::onSysLogin(::THANDLE const tdf, int connectId, ::TDF_LOGIN_RESULT const& data) {
	if (data.nLoginResult) {
		std::cout << util::LogHeader(tdf, connectId) << " login successful: " << data.szInfo;
		assert((0 <= data.nMarkets) && (data.nMarkets <= _countof(data.szMarket)));
		for (int i = 0; i < data.nMarkets; ++i) {
			std::cout << "\n<TDF> >>\t" << data.szMarket[i] << "  " << util::formatDate(data.nDynDate[i]);
		}
		std::cout << std::endl;
	}
	else {
		std::cerr << util::LogHeader(tdf, connectId) << " login failed: " << data.szInfo << std::endl;
	}
}

// ��ȡ�������
void TDF::TickHandler::onSysCodeTable(::THANDLE const tdf, int connectId, ::TDF_CODE_RESULT const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " got code table: " << data.szInfo;
	assert((0 <= data.nMarkets) && (data.nMarkets <= _countof(data.szMarket)));
	for (int i = 0; i < data.nMarkets; ++i) {
		std::cout << "\n<TDF> >>\t" << data.szMarket[i] << "  "
			<< util::formatDate(data.nCodeDate[i]) << "  " << data.nCodeCount[i];
	}
	std::cout << std::endl;
}

// �������ڱ��֪ͨ
void TDF::TickHandler::onSysQuoteDateChange(::THANDLE const tdf, int connectId, ::TDF_QUOTATIONDATE_CHANGE const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " quotation date changed: " << data.szMarket
		<< "  " << util::formatDate(data.nOldDate) << " => " << util::formatDate(data.nNewDate)
		<< std::endl;
}

// ����
void TDF::TickHandler::onSysMarketClose(::THANDLE const tdf, int connectId, ::TDF_MARKET_CLOSE const& data) {
	std::cout << util::LogHeader(tdf, connectId) << " market closed: " << data.chInfo
		<< "\n<TDF> >>\t" << data.szMarket << " (" << util::formatTime(data.nTime)
		<< std::endl;
}

// ������������Ϣ, ��Ӧ�Ľṹ��ΪNULL
void TDF::TickHandler::onSysHeartbeat(::THANDLE const tdf, int connectId) {
	std::cout << util::LogHeader(tdf, connectId) << " heartbeat." << std::endl;
}