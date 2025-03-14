#include "BasicType.hpp"
#include <vector>
class CastGraph
{
	enum class ConversionType : uint32_t
	{
		NONE,
		TRUNCATE = 0x01,
		EXTEND = 0x02,
		SIGN = 0x04,
		USIGN = 0x08,
		METHOD = 0x10,
	};
	using GraphItem = std::pair<BasicTypes, ConversionType>;
	std::vector<std::vector<GraphItem>> m_paths;
	uint8_t getTypeIndex(BasicTypes t)
	{
		return static_cast<uint8_t>(t);
	}

	void createConversionsI8()
	{
		auto& subVector = getTypeIndex(BasicTypes::I8);
		subVector.emplace_back({ BasicTypes::I16, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::I32, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::I64, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::U16, ConversionType::EXTEND | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U32, ConversionType::EXTEND | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U64, ConversionType::EXTEND | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U8, ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::F32, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::F64, ConversionType::EXTEND });
	}

	void createConversionsI16()
	{
		auto& subVector = getTypeIndex(BasicTypes::I16);
		subVector.emplace_back({ BasicTypes::I8, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I32, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::I64, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::U16, ConversionType::SIGN | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U32, ConversionType::EXTEND | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U64, ConversionType::EXTEND | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U8, ConversionType::TRUNCATE | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::F32, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::F64, ConversionType::EXTEND });
	}

	void createConversionsI32()
	{
		auto& subVector = getTypeIndex(BasicTypes::I32);
		subVector.emplace_back({ BasicTypes::I8, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I16, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I64, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::U16, ConversionType::TRUNCATE | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U32, ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U64, ConversionType::EXTEND | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U8, ConversionType::TRUNCATE  | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::F32, ConversionType::NONE });
		subVector.emplace_back({ BasicTypes::F64, ConversionType::EXTEND });
	}

	void createConversionsI64()
	{
		auto& subVector = getTypeIndex(BasicTypes::I64);
		subVector.emplace_back({ BasicTypes::I16, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I32, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I8, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::U16, ConversionType::TRUNCATE | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U32, ConversionType::TRUNCATE | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U64, ConversionType::SIGN | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U8, ConversionType::TRUNCATE | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::F32, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::F64, ConversionType::EXTEND });
	}

	void createConversionsU8()
	{
		auto& subVector = getTypeIndex(BasicTypes::U8);
		subVector.emplace_back({ BasicTypes::I8,  ConversionType::USIGN });
		subVector.emplace_back({ BasicTypes::I16, ConversionType::EXTEND | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::I32, ConversionType::EXTEND | | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::I64, ConversionType::EXTEND | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U16, ConversionType::EXTEND});
		subVector.emplace_back({ BasicTypes::U32, ConversionType::EXTEND});
		subVector.emplace_back({ BasicTypes::U64, ConversionType::EXTEND});
		subVector.emplace_back({ BasicTypes::F32, ConversionType::EXTEND | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::F64, ConversionType::EXTEND | ConversionType::SIGN });
	}

	void createConversionsU16()
	{
		auto& subVector = getTypeIndex(BasicTypes::U16);
		subVector.emplace_back({ BasicTypes::I8, ConversionType::TRUNCATE | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::I16, ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::I32, ConversionType::EXTEND | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::I64, ConversionType::EXTEND | ConversionType::SIGN});
		subVector.emplace_back({ BasicTypes::U32, ConversionType::EXTEND});
		subVector.emplace_back({ BasicTypes::U64, ConversionType::EXTEND});
		subVector.emplace_back({ BasicTypes::U8, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::F32, ConversionType::EXTEND | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::F64, ConversionType::EXTEND | ConversionType::SIGN });
	}

	void createConversionsU32()
	{
		auto& subVector = getTypeIndex(BasicTypes::I32);
		subVector.emplace_back({ BasicTypes::I8, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I16, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I64, ConversionType::EXTEND });
		subVector.emplace_back({ BasicTypes::U16, ConversionType::TRUNCATE | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U32, ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U64, ConversionType::EXTEND | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U8, ConversionType::TRUNCATE | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::F32, ConversionType::NONE });
		subVector.emplace_back({ BasicTypes::F64, ConversionType::EXTEND });
	}

	void createConversionsI64()
	{
		auto& subVector = getTypeIndex(BasicTypes::I64);
		subVector.emplace_back({ BasicTypes::I16, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I32, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::I8, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::U16, ConversionType::TRUNCATE | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U32, ConversionType::TRUNCATE | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U64, ConversionType::SIGN | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::U8, ConversionType::TRUNCATE | ConversionType::SIGN });
		subVector.emplace_back({ BasicTypes::F32, ConversionType::TRUNCATE });
		subVector.emplace_back({ BasicTypes::F64, ConversionType::EXTEND });
	}
	





	std::vector<GraphItem>& getSubVector(BasicTypes t)
	{
		return m_paths[getTypeIndex(t)];
	}



public:
	CastGraph()
	{
		m_paths.resize(getTypeIndex(BasicTypes::LAST_LABEL));
		createConversionsI8();
		createConversionsI16();
		createConversionsI32();
		createConversionsI64();
	}

};