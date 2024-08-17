#pragma once
#include "../ast/AstScope.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include "llvm_type_generator.hpp"
#include "llvm_stmt_generator.hpp"
class LlvmScopeGenerator
{
	llvm::Function* processFunction(AstScope* scope, llvm::Type* type, llvm::IRBuilder<>& b, std::unique_ptr<llvm::Module>& m)
	{
		auto function = llvm::Function::Create(llvm::FunctionType::get(type, false), llvm::GlobalValue::PrivateLinkage, scope->getName(), *m);
		getLlvmCache<>().insertElement(function, scope);
		auto bb = llvm::BasicBlock::Create(getContext(), "entry", function);
		b.SetInsertPoint(bb);
		auto elements = scope->getElements();
		for (auto& it : elements)
		{
			llvmStmtGenerator::generateInstruction(it.get(), b);
		}
		return function;
	}
public:
	llvm::Function* processAstScope(AstScope* scope, llvm::IRBuilder<>& b, std::unique_ptr<llvm::Module>& m)
	{
		std::optional<BasicTypes> retType = scope->getScopeType();
		if (retType.has_value())
		{
			return processFunction(scope, LlvmTypeGenerator::convertAstToLlvmType(*retType), b, m);
		}
		return nullptr;
	}

};