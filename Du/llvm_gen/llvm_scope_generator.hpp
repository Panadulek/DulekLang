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
		
		if ( ScopeDecorator::Function* _fun = scope->getFunctionDecorator())
		{
			std::vector<llvm::Type*> types;
			if (_fun->hasArgs())
			{
				auto args = _fun->getArgs();
				for (auto it : args)
				{
					if (AstVariableDecl* var = ast_element_cast<AstVariableDecl>(it))
					{
						types.push_back(LlvmTypeGenerator::convertAstToLlvmType(var->getVarType()));
					}
				}
			}
			auto function = llvm::Function::Create(llvm::FunctionType::get(type, types, false), llvm::GlobalValue::PrivateLinkage, scope->getName(), *m);
			getLlvmCache<>().insertElement(function, scope);
			auto bb = llvm::BasicBlock::Create(getContext(), "entry", function);
			b.SetInsertPoint(bb);
			auto elements = scope->getElements();
			llvm::Function::arg_iterator argsIt = function->arg_begin();
			if (_fun->hasArgs())
			{
				auto args = _fun->getArgs();
				for (auto it : args)
				{
					llvm::Value* val = argsIt++;
					getLlvmCache<>().insertElement(val, it);
				}
			}
			for (auto& it : elements)
			{
				llvmStmtGenerator::generateInstruction(it.get(), b);
			}
			return function;
		}
		return nullptr;
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