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
		llvm::Function* res = nullptr;
		scope->accept(
			overloaded{
				[&](ScopeDecorator::Function& func) -> void
				{
			std::vector<llvm::Type*> types;
			if (func.hasArgs())
			{
				auto args = func.getArgs();
				for (auto it : args)
				{
					if (AstVariableDecl* var = ast_element_cast<AstVariableDecl>(it))
					{
						AstType* type = var->getVarType();
						if (type)
						{
							types.push_back(LlvmTypeGenerator::convertAstToLlvmType(type->getType()));
						}
						else
						{
							assert(0);
						}
					}
				}
			}


			llvm::GlobalValue::LinkageTypes linkage = llvm::GlobalValue::PrivateLinkage;
			if (scope->getName() == "main") {
				linkage = llvm::GlobalValue::ExternalLinkage;
			}
			auto function = llvm::Function::Create(llvm::FunctionType::get(type, types, false), linkage, scope->getName(), *m);
			getLlvmCache<>().insertElement(function, scope);
			auto bb = llvm::BasicBlock::Create(getContext(), "entry", function);
			b.SetInsertPoint(bb);
			llvm::Function::arg_iterator argsIt = function->arg_begin();
			if (func.hasArgs())
			{
				auto args = func.getArgs();
				for (auto it : args)
				{
					llvm::Value* val = argsIt++;
					getLlvmCache<>().insertElement(val, it);
				}
			}
			auto elements = scope->getStmts();
			for (auto& it : elements)
			{
				llvmStmtGenerator::generateInstruction(it.get(), b);
			}
			res = function;
				},
				[&](auto&&) -> void
				{
					assert(0 && "Unsupported scope decorator for function processing");
					res = nullptr;
				}
			}
		);

		return res;
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