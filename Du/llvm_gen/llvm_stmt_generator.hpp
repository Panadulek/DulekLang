#pragma once
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include "../ast/AstStatement.hpp"
#include "../ast/AstVariableDecl.hpp"
#include "../ast/BasicType.hpp"
#include "llvm_type_generator.hpp"
#include "../ast/AstCast.hpp"
#include "../ast/AstConst.hpp"
#include "../ast/AstReference.h"
#include "../ast/AstCast.hpp"
#include <format>
#include "../ast/AstCallFun.hpp"
#include "llvm_cache.hpp"
class llvmStmtGenerator
{

	static llvm::Value* _getAllocInst(std::string_view variableName, llvm::BasicBlock::iterator begin, llvm::BasicBlock::iterator end)
	{
		for (auto it = begin; it != end; it++)
		{
			llvm::Value* instr = &*it;
			if (llvm::AllocaInst* inst = llvm::dyn_cast<llvm::AllocaInst>(instr))
			{
				if (!inst->getName().compare(variableName))
				{
					return inst;
				}
			}
		}
		return nullptr;
	}
	static llvm::Value* getAllocInst(std::string_view variableName, llvm::IRBuilder<>& b)
	{
		auto bb = b.GetInsertBlock();
		return _getAllocInst(variableName, bb->begin(), bb->end());
	}
	static llvm::Value* getValueFromExpr(AstElement* element, llvm::IRBuilder<>& b)
	{
		if (AstConst* aconst = ast_element_cast<AstConst>(element))
		{
			switch (aconst->getType())
			{
			case BasicTypes::U64:
				return llvm::ConstantInt::get(LlvmTypeGenerator::convertAstToLlvmType(aconst->getType()), aconst->get_uint64_t_value());
			}
		}
		else if (AstExpr* expr = ast_element_cast<AstExpr>(element))
		{
			return generateExprInstruction(expr, b);
		}
		else if (AstRef* ref = ast_element_cast<AstRef>(element))
		{
			llvm::Value* variable = getLlvmCache<>().getValue(ref);
			if(variable->getType()->isPointerTy())
				return b.CreateLoad(LlvmTypeGenerator::convertAstToLlvmType(ref->getType().value()), variable, std::format("Load from: {}", ref->getName()));
			return
				variable;
		}
		else if (AstCallFun* callFn = dynamic_cast<AstCallFun*>(element))
		{
			llvm::Value* fun = getLlvmCache<>().getValue(const_cast<AstScope*>(callFn->getScope()));
			if (llvm::Function* llvmFun = llvm::dyn_cast<llvm::Function>(fun))
			{
				std::string comment = std::format("call {}", callFn->getName());
				std::vector<llvm::Value*> llvm_args;
				auto args = callFn->getArgs();
				for (auto& it : args)
				{
					if (AstExpr* expr = ast_unique_element_cast<AstExpr>(it))
						llvm_args.push_back(generateExprInstruction(expr, b));
				}
				return b.CreateCall(llvmFun, llvm_args, comment);
				
			}
		}
		return nullptr;
	}
	static llvm::Value* generateCallFun(AstElement* element, llvm::IRBuilder<>& b)
	{
		return nullptr;
	}
	static llvm::Value* generateExprInstruction(AstExpr* expr, llvm::IRBuilder<>& b)
	{
		llvm::Value* l = getValueFromExpr(expr->left(), b);
		llvm::Value* r = getValueFromExpr(expr->right(), b);
		switch (expr->op())
		{
		case AstExpr::Operation::Addition:
			return b.CreateAdd(l, r, std::format("add {} and {}", expr->left()->getName(), expr->right()->getName()));
		case AstExpr::Operation::Subtraction:
			return b.CreateSub(l, r, std::format("Sub {} and {} ", expr->left()->getName(), expr->right()->getName()));
		case AstExpr::Operation::Reference:
		case AstExpr::Operation::ConstValue:
			return r;
		case AstExpr::Operation::Multiplication:
			return b.CreateMul(l, r, std::format("Mul {} and {} ", expr->left()->getName(), expr->right()->getName()));
		case AstExpr::Operation::Division:
			return b.CreateSDiv(l, r, std::format("SDiv {} and {} ", expr->left()->getName(), expr->right()->getName()));
		case AstExpr::Operation::Unary_negation:
			return b.CreateNeg(l, std::format("Neg {}", expr->left()->getName()));
		case AstExpr::Operation::Call_fun:
			return l;
		default:
			break;
		}
		return nullptr;
	}
	static void generateAssgnInstruction(AstStatement* assgn, llvm::IRBuilder<>& b)
	{
		if (assgn && assgn->lhs() && assgn->rhs())
		{
			llvm::Value* exprInstruction = generateExprInstruction(assgn->rhs(), b);
			auto bb = b.GetInsertBlock();
			llvm::Value* variable = getAllocInst(assgn->lhs()->getName(), b);
			b.CreateStore(exprInstruction, variable, false);
		}
	}

	static llvm::Value* generateRhsInstruction(AstStatement* stmt, llvm::IRBuilder<>& b)
	{
		if(stmt)
			if (AstExpr* expr = ast_element_cast<AstExpr>(stmt->rhs()))
			{
				return generateExprInstruction(expr, b);
			}
		return nullptr;
	}
	
	static llvm::Value* generateRetInstruction(AstStatement* stmt, llvm::IRBuilder<>& b)
	{
		llvm::Value* retVal = generateRhsInstruction(stmt, b);
		if (retVal)
		{
			return b.CreateRet(retVal);
		}
	}

public:
	static void generateInstruction(AstElement* stmt, llvm::IRBuilder<>& b)
	{
		if (!stmt) {
			llvm::errs() << "Error: Invalid argument(s) passed to generateInstruction.\n";
			return;
		}
		if (AstStatement* _stmt = ast_element_cast<AstStatement>(stmt))
		{
			switch (_stmt->getStmtType())
			{
			case AstStatement::STMT_TYPE::ASSIGN:
				generateAssgnInstruction(_stmt, b);
				break;
			case AstStatement::STMT_TYPE::RHS_STMT:
				generateRhsInstruction(ast_element_cast<AstStatement>(stmt), b);
				break;
			case AstStatement::STMT_TYPE::RET_STMT:
				{
					llvm::Value* retVal = generateRetInstruction(ast_element_cast<AstStatement>(stmt), b);
					break;
				}
			}
			return ;
		}
        else if (AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(stmt))
        {
            llvm::Type* llvmType = LlvmTypeGenerator::convertAstToLlvmType(decl->getVarType());
            llvm::Value* val = b.CreateAlloca(llvmType, nullptr, decl->getName());
			getLlvmCache<>().insertElement(val, stmt);
		}
		return;
	}
};