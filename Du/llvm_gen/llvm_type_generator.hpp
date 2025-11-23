#pragma once
#include "../ast/BasicType.hpp"
#include <llvm/IR/Type.h>
extern llvm::LLVMContext& getContext();
class LlvmTypeGenerator
{
private:

public:
	static llvm::Type* convertAstToLlvmType(BasicTypes bt, std::size_t countElements = -1)
	{
		switch (bt)
		{
			case BasicTypes::I8:
			case BasicTypes::U8:
				return llvm::dyn_cast<llvm::Type>(llvm::Type::getInt8Ty(getContext()));
			case BasicTypes::I16:
			case BasicTypes::U16:
				return llvm::dyn_cast<llvm::Type>(llvm::Type::getInt16Ty(getContext()));

			case BasicTypes::I32:
			case BasicTypes::U32:
				return llvm::dyn_cast<llvm::Type>(llvm::Type::getInt32Ty(getContext()));
			case BasicTypes::I64:
			case BasicTypes::U64:
				return llvm::dyn_cast<llvm::Type>(llvm::Type::getInt64Ty(getContext()));
			case BasicTypes::VOID_TYPE:
				return llvm::Type::getVoidTy(getContext());
			case BasicTypes::F32:
				return llvm::Type::getFloatTy(getContext());
			case BasicTypes::F64:
				return llvm::Type::getDoubleTy(getContext());
			case BasicTypes::STR:
			{

			}
			case BasicTypes::BOOL:
				return llvm::dyn_cast<llvm::Type>(llvm::Type::getInt1Ty(getContext()));
			default:
				return nullptr;

		}
	}

};