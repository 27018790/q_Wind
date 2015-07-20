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
				*/
			}
		};

	}//namespace TDB::traits
}//naemspace TDF

#endif//__TDF_API_HELPER_H__