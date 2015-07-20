#ifndef __TDF_API_HELPER_H__
#define __TDF_API_HELPER_H__

#include "win32.util/Singleton.h"
#include "kdb+.util/K_ptr.h"
#include "kdb+.util/type_convert.h"
#include "Wind.util/FieldMapper.h"

#ifdef _MSC_VER
//@ref https://msdn.microsoft.com/query/dev12.query?appId=Dev12IDEF1&l=EN-US&k=k(C4290);k(VS.ErrorList)&rd=true
#pragma warning(disable: 4290)
#endif

namespace TDF {

	void parseTdfHandle(K h, ::THANDLE& tdf) throw(std::string);

	namespace accessor {
		
		template <typename T>
		struct CodeInfoAccessor : public Wind::accessor::NestedAccessor<T, ::TDF_CODE_INFO*> {
			typedef Wind::accessor::NestedAccessor<T, ::TDF_CODE_INFO*> base_type;
			CodeInfoAccessor(typename base_type::field_accessor_type* field)
				: base_type(field, &T::pCodeInfo) {}
		};

		typedef Wind::accessor::IntAccessor<::TDF_CODE_INFO, G> CodeTypeAccessor;
		typedef Wind::accessor::SymbolAccessor<::TDF_CODE_INFO, char[64], Wind::encoder::GB18030_UTF8> CodeNameAccessor;

		template <typename T>
		struct ExCodeInfoAccessor : public Wind::accessor::NestedAccessor<T, ::TDF_CODE_INFO*, ::TD_EXCODE_INFO> {
			typedef Wind::accessor::NestedAccessor<T, ::TDF_CODE_INFO*, ::TD_EXCODE_INFO> base_type;
			ExCodeInfoAccessor(typename base_type::field_accessor_type* field)
				: base_type(field, &T::pCodeInfo, &::TDF_CODE_INFO::exCodeInfo) {}
		};

		typedef Wind::accessor::DateAccessor<::TD_EXCODE_INFO> ExCodeDateAccessor;
		typedef Wind::accessor::CharAccessor<::TD_EXCODE_INFO> ExCodeCharAccessor;
		typedef Wind::accessor::IntAccessor<::TD_EXCODE_INFO, I> ExCodeIntAccessor;
		typedef Wind::accessor::FloatAccessor<::TD_EXCODE_INFO, int> ExCodeFloatAccessor;
		typedef Wind::accessor::SymbolAccessor<::TD_EXCODE_INFO, char[32]> ExCodeSymbolAccessor;

	}//namespace TDF::accessor

	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TDF_INDEX_DATA.
		struct Index : public Wind::mapper::Fields<::TDF_INDEX_DATA> {
			typedef Singleton<Index> accessor_map;
			typedef ::TDF_INDEX_DATA tdf_result_type;

			Index() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdf_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdf_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdf_result_type> TimeAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdf_result_type, FieldT>;

			typedef TDF::accessor::CodeInfoAccessor<tdf_result_type> CodeInfoAccessor;
			typedef TDF::accessor::CodeTypeAccessor CodeTypeAccessor;
			typedef TDF::accessor::CodeNameAccessor CodeNameAccessor;

			// Data fields -- always keep in sync with TDF_INDEX_DATA
			void registerAllFields() {
				// 600001.SH
				addField("WindCode", new SymbolAccessor(&tdf_result_type::szWindCode));
				// ԭʼCode
				addField("Code", new SymbolAccessor(&tdf_result_type::szCode));
				// ҵ������(��Ȼ��)
				addField("Date", new DateAccessor(&tdf_result_type::nActionDay));
				// ������
				addField("TradeDate", new DateAccessor(&tdf_result_type::nTradingDay));
				// ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
				addField("Time", new TimeAccessor(&tdf_result_type::nTime));
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
				addField("Turnover", new FloatAccessor<__int64>(&tdf_result_type::iTurnover, .01));
				// ǰ��ָ��
				addField("PreClose", new FloatAccessor<int>(&tdf_result_type::nPreCloseIndex));
				//��TDF_CODE_INFO��֤ȯ����
				addField("CodeType", new CodeInfoAccessor(new CodeTypeAccessor(&::TDF_CODE_INFO::nType)));
				//��TDF_CODE_INFO������֤ȯ����
				addField("CodeName", new CodeInfoAccessor(new CodeNameAccessor(&::TDF_CODE_INFO::chName)));
			}
		};

		// Traits to assist symbolic retrieval of tick data fields from TDF_MARKET_DATA.
		struct Market : public Wind::mapper::Fields<::TDF_MARKET_DATA> {
			typedef Singleton<Index> accessor_map;
			typedef ::TDF_MARKET_DATA tdf_result_type;

			Market() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdf_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdf_result_type> DateAccessor;
			typedef Wind::accessor::TimeAccessor<tdf_result_type> TimeAccessor;
			template <typename FieldT>
			using FloatAccessor = Wind::accessor::FloatAccessor<tdf_result_type, FieldT>;
			template <typename FieldT>
			using FloatAccessors = Wind::accessor::FloatsAccessor<tdf_result_type, FieldT>;

			typedef TDF::accessor::CodeInfoAccessor<tdf_result_type> CodeInfoAccessor;
			typedef TDF::accessor::CodeTypeAccessor CodeTypeAccessor;
			typedef TDF::accessor::CodeNameAccessor CodeNameAccessor;

			// Data fields -- always keep in sync with TDF_MARKET_DATA
			void registerAllFields() {
				// 600001.SH
				addField("WindCode", new SymbolAccessor(&tdf_result_type::szWindCode));
				// ԭʼCode
				addField("Code", new SymbolAccessor(&tdf_result_type::szCode));
				// ҵ������(��Ȼ��)
				addField("Date", new DateAccessor(&tdf_result_type::nActionDay));
				// ������
				addField("TradeDate", new DateAccessor(&tdf_result_type::nTradingDay));
				// ʱ�䣨HHMMSSmmm������94500000 ��ʾ 9��45��00��000����
				addField("Time", new TimeAccessor(&tdf_result_type::nTime));
				/*
				int			 nStatus;				//״̬

				// ǰ���̼�
				addField("PreClose", new FloatAccessor<unsigned int>(&tdf_result_type::nPreClose));

				unsigned int nOpen;					//���̼�
				unsigned int nHigh;					//��߼�
				unsigned int nLow;					//��ͼ�
				unsigned int nMatch;				//���¼�
				unsigned int nAskPrice[10];			//������
				unsigned int nAskVol[10];			//������
				unsigned int nBidPrice[10];			//�����
				unsigned int nBidVol[10];			//������
				unsigned int nNumTrades;			//�ɽ�����
				__int64		 iVolume;				//�ɽ�����
				__int64		 iTurnover;				//�ɽ��ܽ��
				__int64		 nTotalBidVol;			//ί����������
				__int64		 nTotalAskVol;			//ί����������
				unsigned int nWeightedAvgBidPrice;	//��Ȩƽ��ί��۸�
				unsigned int nWeightedAvgAskPrice;  //��Ȩƽ��ί���۸�
				int			 nIOPV;					//IOPV��ֵ��ֵ
				int			 nYieldToMaturity;		//����������
				unsigned int nHighLimited;			//��ͣ��
				unsigned int nLowLimited;			//��ͣ��
				char		 chPrefix[4];			//֤ȯ��Ϣǰ׺
				int			 nSyl1;					//��ӯ��1
				int			 nSyl2;					//��ӯ��2
				int			 nSD2;					//����2���Ա���һ�ʣ�


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
				//��TDF_CODE_INFO��֤ȯ����
				addField("CodeType", new CodeInfoAccessor(new CodeTypeAccessor(&::TDF_CODE_INFO::nType)));
				//��TDF_CODE_INFO������֤ȯ����
				addField("CodeName", new CodeInfoAccessor(new CodeNameAccessor(&::TDF_CODE_INFO::chName)));
				*/
			}
		};

	}//nmespace TDF::traits

	/*
	namespace traits {

		// Traits to assist symbolic retrieval of tick data fields from TD_EXCODE_INFO::TD_OptionCodeInfo.
		struct OptionCodeInfo : public Wind::mapper::Fields<::TD_EXCODE_INFO::TD_OptionCodeInfo> {
			typedef Singleton<OptionCodeInfo> accessor_map;
			typedef ::TD_EXCODE_INFO::TD_OptionCodeInfo tdf_result_type;

			OptionCodeInfo() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::SymbolAccessor<tdf_result_type, char[32]> SymbolAccessor;
			typedef Wind::accessor::DateAccessor<tdf_result_type> DateAccessor;
			typedef Wind::accessor::IntAccessor<tdf_result_type, I> IntAccessor;
			typedef Wind::accessor::CharAccessor<tdf_result_type> CharAccessor;
			typedef Wind::accessor::FloatAccessor<tdf_result_type, int> FloatAccessor;

			// Data fields -- always keep in sync with TD_EXCODE_INFO::TD_OptionCodeInfo
			void registerAllFields() {
				//��Option����Ȩ��Լ����C19
				addField("Contract", new SymbolAccessor(&tdf_result_type::chContractID));
				//��Option�����֤ȯ����
				addField("Underlying", new SymbolAccessor(&tdf_result_type::szUnderlyingSecurityID));
				//��Option�����֤ȯ����C3    0-A�� 1-ETF (EBS �C ETF�� ASH �C A ��)
				addField("UnderlyingType", new CharAccessor(&tdf_result_type::chUnderlyingType));
				//��Option��ŷʽ��ʽC1        ��Ϊŷʽ��Ȩ�����ֶ�Ϊ��E������Ϊ��ʽ��Ȩ�����ֶ�Ϊ��A��
				addField("OptionType", new CharAccessor(&tdf_result_type::chOptionType));
				//��Option���Ϲ��Ϲ�C1        �Ϲ������ֶ�Ϊ��C������Ϊ�Ϲ������ֶ�Ϊ��P��
				addField("CallPut", new CharAccessor(&tdf_result_type::chCallOrPut));
				//��Option���ǵ�����������C1 ��N����ʾ���ǵ�����������, ��R����ʾ���ǵ�����������
				addField("PriceLimitType", new CharAccessor(&tdf_result_type::chPriceLimitType));
				//��Option����Լ��λ,         ������Ȩ��Ϣ������ĺ�Լ��λ, һ��������
				addField("Multiplier", new IntAccessor(&tdf_result_type::nContractMultiplierUnit));
				//��Option����Ȩ��Ȩ��,       ������Ȩ��Ϣ���������Ȩ��Ȩ�ۣ��Ҷ��룬��ȷ����
				addField("ExePrice", new FloatAccessor(&tdf_result_type::nExercisePrice, .001));
				//��Option����Ȩ�׸�������,YYYYMMDD
				addField("StartDate", new DateAccessor(&tdf_result_type::nStartDate));
				//��Option����Ȩ�������/��Ȩ�գ�YYYYMMDD
				addField("EndDate", new DateAccessor(&tdf_result_type::nEndDate));
				//��Option����Ȩ��Ȩ�գ�YYYYMMDD
				addField("ExeDate", new DateAccessor(&tdf_result_type::nExerciseDate));
				//��Option����Ȩ�����գ�YYYYMMDD
				addField("ExpDate", new DateAccessor(&tdf_result_type::nExpireDate));
			}
		};

		// Traits to assist symbolic retrieval of tick data fields from TD_EXCODE_INFO.
		struct ExCodeInfo : public Wind::mapper::Fields<::TD_EXCODE_INFO> {
			typedef Singleton<ExCodeInfo> accessor_map;
			typedef ::TD_EXCODE_INFO tdf_result_type;

			ExCodeInfo() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			// Data fields -- always keep in sync with TD_EXCODE_INFO
			void registerAllFields() {
				addField("Option",
					new Wind::accessor::NestedAccessor<tdf_result_type, OptionCodeInfo::tdf_result_type>(
						&tdf_result_type::Option));
			}
		};

		// Traits to assist symbolic retrieval of tick data fields from TDF_CODE_INFO.
		struct CodeInfo : public Wind::mapper::Fields<::TDF_CODE_INFO> {
			typedef Singleton<CodeInfo> accessor_map;
			typedef ::TDF_CODE_INFO tdf_result_type;

			CodeInfo() : Wind::mapper::Fields<tdf_result_type>() { registerAllFields(); }

			typedef Wind::accessor::IntAccessor<tdf_result_type, G> TypeAccessor;
			template <typename FieldT, typename Encoder = Wind::encoder::Passthrough>
			using SymbolAccessor = Wind::accessor::SymbolAccessor<tdf_result_type, FieldT, Encoder>;
			typedef Wind::accessor::NestedAccessor<tdf_result_type, OptionCodeInfo> NestedAccessor;

			// Data fields -- always keep in sync with TDF_CODE_INFO
			void registerAllFields() {
				// ֤ȯ����
				addField("Type", new TypeAccessor(&tdf_result_type::nType));
				// ֤ȯ����
				addField("Code", new SymbolAccessor<char[32]>(&tdf_result_type::chCode));
				// ����֤ȯ����
				addField("Name", new SymbolAccessor<char[64], Wind::encoder::GB18030_UTF8>(&tdf_result_type::chName));
				//>> OptionCodeInfo
				//OptionCodeInfo const* optionCode = OptionCodeInfo::accessor_map::getInstance();
				/*
				addField("Contract", new NestedAccessor(&tdf_result_type::chContractID));
				addField("Underlying", new SymbolAccessor(&tdf_result_type::szUnderlyingSecurityID));
				addField("UnderlyingType", new CharAccessor(&tdf_result_type::chUnderlyingType));
				addField("OptionType", new CharAccessor(&tdf_result_type::chOptionType));
				addField("CallPut", new CharAccessor(&tdf_result_type::chCallOrPut));
				addField("PriceLimitType", new CharAccessor(&tdf_result_type::chPriceLimitType));
				addField("Multiplier", new IntAccessor(&tdf_result_type::nContractMultiplierUnit));
				addField("ExePrice", new FloatAccessor(&tdf_result_type::nExercisePrice, .001));
				addField("StartDate", new DateAccessor(&tdf_result_type::nStartDate));
				addField("EndDate", new DateAccessor(&tdf_result_type::nEndDate));
				addField("ExeDate", new DateAccessor(&tdf_result_type::nExerciseDate));
				addField("ExpDate", new DateAccessor(&tdf_result_type::nExpireDate));
				* /
			}
		};

	}//namespace TDB::traits
	*/
}//naemspace TDF

#endif//__TDF_API_HELPER_H__