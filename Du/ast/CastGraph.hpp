#pragma once // Standardowy guard w nowoczesnych kompilatorach

#include "BasicType.hpp"
#include <cstdint>
#include <array>
#include <type_traits>


enum class CastOp : uint32_t
{
	NoOp = 0x0000, 
	Trunc = 0x0001,
	SExt = 0x0002,
	ZExt = 0x0004,
	BitCast = 0x0008,
	SIToFP = 0x0010,
	UIToFP = 0x0020,
	FPToSI = 0x0040,
	FPToUI = 0x0080,
	FPExt = 0x0100,
	FPTrunc = 0x0200,
	Custom = 0x0400,
};

constexpr CastOp operator|(CastOp a, CastOp b) {
	return static_cast<CastOp>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

constexpr bool operator&(CastOp a, CastOp b) {
	return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
}


class CastGraph
{
public:

	static constexpr size_t TypeCount = static_cast<size_t>(BasicTypes::LAST_LABEL);

	static CastOp getCastOp(BasicTypes from, BasicTypes to)
	{
		if (from == to) return CastOp::NoOp;

		auto f = static_cast<size_t>(from);
		auto t = static_cast<size_t>(to);

		if (f >= TypeCount || t >= TypeCount) return CastOp::NoOp;

		return getTable()[f][t];
	}

private:

	using TableType = std::array<std::array<CastOp, TypeCount>, TypeCount>;

	static const TableType& getTable()
	{
		static const TableType table = createTable();
		return table;
	}

	static TableType createTable()
	{
		TableType t = {};

		auto add = [&](BasicTypes src, BasicTypes dst, CastOp op) {
			t[static_cast<size_t>(src)][static_cast<size_t>(dst)] = op;
			};

		// --- Konwersje I8 ---
		add(BasicTypes::I8, BasicTypes::I16, CastOp::SExt);
		add(BasicTypes::I8, BasicTypes::I32, CastOp::SExt);
		add(BasicTypes::I8, BasicTypes::I64, CastOp::SExt);
		add(BasicTypes::I8, BasicTypes::U8, CastOp::BitCast);
		add(BasicTypes::I8, BasicTypes::U16, CastOp::SExt | CastOp::BitCast);
		add(BasicTypes::I8, BasicTypes::U32, CastOp::SExt | CastOp::BitCast);
		add(BasicTypes::I8, BasicTypes::U64, CastOp::SExt | CastOp::BitCast);
		add(BasicTypes::I8, BasicTypes::F32, CastOp::SIToFP);
		add(BasicTypes::I8, BasicTypes::F64, CastOp::SIToFP);

		// --- Konwersje I16 ---
		add(BasicTypes::I16, BasicTypes::I8, CastOp::Trunc);
		add(BasicTypes::I16, BasicTypes::I32, CastOp::SExt);
		add(BasicTypes::I16, BasicTypes::I64, CastOp::SExt);
		add(BasicTypes::I16, BasicTypes::U8, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::I16, BasicTypes::U16, CastOp::BitCast);
		add(BasicTypes::I16, BasicTypes::U32, CastOp::SExt | CastOp::BitCast);
		add(BasicTypes::I16, BasicTypes::U64, CastOp::SExt | CastOp::BitCast);
		add(BasicTypes::I16, BasicTypes::F32, CastOp::SIToFP);
		add(BasicTypes::I16, BasicTypes::F64, CastOp::SIToFP);

		// --- Konwersje I32 ---
		add(BasicTypes::I32, BasicTypes::I8, CastOp::Trunc);
		add(BasicTypes::I32, BasicTypes::I16, CastOp::Trunc);
		add(BasicTypes::I32, BasicTypes::I64, CastOp::SExt);
		add(BasicTypes::I32, BasicTypes::U8, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::I32, BasicTypes::U16, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::I32, BasicTypes::U32, CastOp::BitCast);
		add(BasicTypes::I32, BasicTypes::U64, CastOp::SExt | CastOp::BitCast);
		add(BasicTypes::I32, BasicTypes::F32, CastOp::SIToFP);
		add(BasicTypes::I32, BasicTypes::F64, CastOp::SIToFP);

		// --- Konwersje I64 ---
		add(BasicTypes::I64, BasicTypes::I8, CastOp::Trunc);
		add(BasicTypes::I64, BasicTypes::I16, CastOp::Trunc);
		add(BasicTypes::I64, BasicTypes::I32, CastOp::Trunc);
		add(BasicTypes::I64, BasicTypes::U8, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::I64, BasicTypes::U16, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::I64, BasicTypes::U32, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::I64, BasicTypes::U64, CastOp::BitCast);
		add(BasicTypes::I64, BasicTypes::F32, CastOp::SIToFP | CastOp::Trunc); // Uwaga: dziwna operacja, ale zachowana z orygina�u
		add(BasicTypes::I64, BasicTypes::F64, CastOp::SIToFP);

		// --- Konwersje U8 ---
		add(BasicTypes::U8, BasicTypes::I8, CastOp::BitCast);
		add(BasicTypes::U8, BasicTypes::I16, CastOp::ZExt | CastOp::BitCast);
		add(BasicTypes::U8, BasicTypes::I32, CastOp::ZExt | CastOp::BitCast);
		add(BasicTypes::U8, BasicTypes::I64, CastOp::ZExt | CastOp::BitCast);
		add(BasicTypes::U8, BasicTypes::U16, CastOp::ZExt);
		add(BasicTypes::U8, BasicTypes::U32, CastOp::ZExt);
		add(BasicTypes::U8, BasicTypes::U64, CastOp::ZExt);
		add(BasicTypes::U8, BasicTypes::F32, CastOp::UIToFP);
		add(BasicTypes::U8, BasicTypes::F64, CastOp::UIToFP);

		// --- Konwersje U16 ---
		add(BasicTypes::U16, BasicTypes::I8, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::U16, BasicTypes::I16, CastOp::BitCast);
		add(BasicTypes::U16, BasicTypes::I32, CastOp::ZExt | CastOp::BitCast);
		add(BasicTypes::U16, BasicTypes::I64, CastOp::ZExt | CastOp::BitCast);
		add(BasicTypes::U16, BasicTypes::U8, CastOp::Trunc);
		add(BasicTypes::U16, BasicTypes::U32, CastOp::ZExt);
		add(BasicTypes::U16, BasicTypes::U64, CastOp::ZExt);
		add(BasicTypes::U16, BasicTypes::F32, CastOp::UIToFP);
		add(BasicTypes::U16, BasicTypes::F64, CastOp::UIToFP);

		// --- Konwersje U32 ---
		add(BasicTypes::U32, BasicTypes::I8, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::U32, BasicTypes::I16, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::U32, BasicTypes::I32, CastOp::BitCast);
		add(BasicTypes::U32, BasicTypes::I64, CastOp::ZExt | CastOp::BitCast);
		add(BasicTypes::U32, BasicTypes::U8, CastOp::Trunc);
		add(BasicTypes::U32, BasicTypes::U16, CastOp::Trunc);
		add(BasicTypes::U32, BasicTypes::U64, CastOp::ZExt);
		add(BasicTypes::U32, BasicTypes::F32, CastOp::UIToFP);
		add(BasicTypes::U32, BasicTypes::F64, CastOp::UIToFP);

		// --- Konwersje U64 ---
		add(BasicTypes::U64, BasicTypes::I8, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::U64, BasicTypes::I16, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::U64, BasicTypes::I32, CastOp::Trunc | CastOp::BitCast);
		add(BasicTypes::U64, BasicTypes::I64, CastOp::BitCast);
		add(BasicTypes::U64, BasicTypes::U8, CastOp::Trunc);
		add(BasicTypes::U64, BasicTypes::U16, CastOp::Trunc);
		add(BasicTypes::U64, BasicTypes::U32, CastOp::Trunc);
		add(BasicTypes::U64, BasicTypes::F32, CastOp::UIToFP | CastOp::Trunc); // Uwaga: z orygina�u
		add(BasicTypes::U64, BasicTypes::F64, CastOp::UIToFP);

		// --- Konwersje F32 ---
		add(BasicTypes::F32, BasicTypes::I8, CastOp::FPToSI | CastOp::Trunc);
		add(BasicTypes::F32, BasicTypes::I16, CastOp::FPToSI | CastOp::Trunc);
		add(BasicTypes::F32, BasicTypes::I32, CastOp::FPToSI);
		add(BasicTypes::F32, BasicTypes::I64, CastOp::FPToSI | CastOp::FPExt); // Uwaga: z orygina�u
		add(BasicTypes::F32, BasicTypes::U8, CastOp::FPToUI | CastOp::Trunc);
		add(BasicTypes::F32, BasicTypes::U16, CastOp::FPToUI | CastOp::Trunc);
		add(BasicTypes::F32, BasicTypes::U32, CastOp::FPToUI);
		add(BasicTypes::F32, BasicTypes::U64, CastOp::FPToUI | CastOp::FPExt); // Uwaga: z orygina�u
		add(BasicTypes::F32, BasicTypes::F64, CastOp::FPExt);

		// --- Konwersje F64 ---
		add(BasicTypes::F64, BasicTypes::I8, CastOp::FPToSI | CastOp::Trunc);
		add(BasicTypes::F64, BasicTypes::I16, CastOp::FPToSI | CastOp::Trunc);
		add(BasicTypes::F64, BasicTypes::I32, CastOp::FPToSI | CastOp::Trunc);
		add(BasicTypes::F64, BasicTypes::I64, CastOp::FPToSI);
		add(BasicTypes::F64, BasicTypes::U8, CastOp::FPToUI | CastOp::Trunc);
		add(BasicTypes::F64, BasicTypes::U16, CastOp::FPToUI | CastOp::Trunc);
		add(BasicTypes::F64, BasicTypes::U32, CastOp::FPToUI | CastOp::Trunc);
		add(BasicTypes::F64, BasicTypes::U64, CastOp::FPToUI);
		add(BasicTypes::F64, BasicTypes::F32, CastOp::FPTrunc);

		// --- Konwersje BOOL ---
		add(BasicTypes::BOOL, BasicTypes::I8, CastOp::ZExt);
		add(BasicTypes::BOOL, BasicTypes::I16, CastOp::ZExt);
		add(BasicTypes::BOOL, BasicTypes::I32, CastOp::ZExt);
		add(BasicTypes::BOOL, BasicTypes::I64, CastOp::ZExt);
		add(BasicTypes::BOOL, BasicTypes::U8, CastOp::ZExt);
		add(BasicTypes::BOOL, BasicTypes::U16, CastOp::ZExt);
		add(BasicTypes::BOOL, BasicTypes::U32, CastOp::ZExt);
		add(BasicTypes::BOOL, BasicTypes::U64, CastOp::ZExt);
		add(BasicTypes::BOOL, BasicTypes::F32, CastOp::UIToFP);
		add(BasicTypes::BOOL, BasicTypes::F64, CastOp::UIToFP);

		add(BasicTypes::I8,   BasicTypes::BOOL, CastOp::Trunc);
		add(BasicTypes::I16,  BasicTypes::BOOL, CastOp::Trunc);
		add(BasicTypes::I32,  BasicTypes::BOOL, CastOp::Trunc);
		add(BasicTypes::I64,  BasicTypes::BOOL, CastOp::Trunc);
		add(BasicTypes::U8,   BasicTypes::BOOL, CastOp::Trunc);
		add(BasicTypes::U16,  BasicTypes::BOOL, CastOp::Trunc);
		add(BasicTypes::U32,  BasicTypes::BOOL, CastOp::Trunc);
		add(BasicTypes::U64,  BasicTypes::BOOL, CastOp::Trunc);
		add(BasicTypes::F32,  BasicTypes::BOOL, CastOp::FPToUI);
		add(BasicTypes::F64,  BasicTypes::BOOL, CastOp::FPToUI);

		return t;
	}
};

