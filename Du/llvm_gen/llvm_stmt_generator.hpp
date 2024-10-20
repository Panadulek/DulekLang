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
	static llvm::Value* dump_op(llvm::Value* val)
	{
		val->dump();
		return val;
	} 
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
	static llvm::Value* generateArrayIndexing(AstExpr* expr, llvm::IRBuilder<>& b)
	{
		if (!expr)
			return nullptr;
		AstExpr* left = ast_element_cast<AstExpr>(expr->left());
		if (left && left->op() == AstExpr::Operation::Reference)
		{
			llvm::Value* arr = generateExprInstruction(left, b);
			llvm::Type* elementType = arr->getType();
			llvm::Type* _elementType = elementType;
			if (AstExpr* _expr = ast_element_cast<AstExpr>(expr->right()))
			{
				auto dims = AstExpr::transformExprToDimsArray(_expr);
				std::vector<llvm::Value*> llvmIdxs;
				if (dims)
				{
					for (auto it = dims->begin(); it != dims->end(); ++it)
					{
						if (it != dims->begin())
							elementType = elementType->getPointerTo();
						llvmIdxs.push_back(generateExprInstruction(it->get()->getExpr(), b));
					}
					return b.CreateGEP(_elementType, arr, llvmIdxs);
					
				}
			}			
		}
		return nullptr;
	}
	static llvm::Value* getValueFromExpr(AstElement* element, llvm::IRBuilder<>& b)
	{
		if (AstConst* aconst = ast_element_cast<AstConst>(element))
		{
			switch (aconst->getType())
			{
			case BasicTypes::U64:
				return llvm::ConstantInt::get(LlvmTypeGenerator::convertAstToLlvmType(aconst->getType()), aconst->get_uint64_t_value());
			case BasicTypes::STR:
			{
				
			}
			}
		}
		else if (AstExpr* expr = ast_element_cast<AstExpr>(element))
		{
				return generateExprInstruction(expr, b);
		}
		else if (AstRef* ref = ast_element_cast<AstRef>(element))
		{
			llvm::Value* variable = getLlvmCache<>().getValue(ref);
			return variable;
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
		if (expr->op() == AstExpr::Operation::Arr_Indexing)
		{
			return generateArrayIndexing(expr, b);
		}
		else
		{
			llvm::Value* l = getValueFromExpr(expr->left(), b);
			llvm::Value* r = getValueFromExpr(expr->right(), b);
			std::string description =
#ifdef _DEBUG
				std::format("{}, {} and {}", static_cast<uint8_t>(expr->op()), expr->left() ? expr->left()->getName() : "null",
					expr->right() ? expr->right()->getName() : "null");
#else
				"";
#endif
			switch (expr->op())
			{
			case AstExpr::Operation::Addition:
				return b.CreateAdd(l, r, description);
			case AstExpr::Operation::Subtraction:
				return b.CreateSub(l, r, description);
			case AstExpr::Operation::Reference:
			case AstExpr::Operation::ConstValue:
				return r;
			case AstExpr::Operation::Multiplication:
				return b.CreateMul(l, r, description);
			case AstExpr::Operation::Division:
				return b.CreateSDiv(l, r, description);
			case AstExpr::Operation::Unary_negation:
				return b.CreateNeg(l, description);
			case AstExpr::Operation::Call_fun:
				return l;
			default:
				break;
			}
			return nullptr;
		}
	}
	static void generateAssgnInstruction(AstStatement* assgn, llvm::IRBuilder<>& b)
	{
		if (assgn && assgn->lhs() && assgn->rhs())
		{
			llvm::Value* exprInstruction = generateExprInstruction(assgn->rhs(), b);
			auto bb = b.GetInsertBlock();
			AstElement* lhs = assgn->lhs();
			llvm::Value* variable = nullptr;
			if (AstExpr* expr = ast_element_cast<AstExpr>(lhs))
			{
				switch (expr->op())
				{
					case AstExpr::Operation::Arr_Indexing:
					case AstExpr::Operation::Reference:
					{
						variable = generateExprInstruction(expr, b);
						break;
					}
					default:
						assert(0 && "Expr != able to assign");
						break;
				}					
			}
			else
				 variable = getAllocInst(assgn->lhs()->getName(), b);
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
	static llvm::Value* generateDeclArrayInstruction(AstVariableDecl* stmt, llvm::IRBuilder<>& b)
	{
		AstType* type = stmt->getVarType();
		if (!type)
		{
			assert(0);
			return nullptr;
		}
		auto begin = type->beginArrayRange();
		auto end = type->endArrayRange();
		if (begin.has_value() && end.has_value() && begin != end)
		{
			std::vector<llvm::Value*> dims(type->getDimensionCounter());
			std::size_t idx = 0;
			for(auto it = begin.value(); it != end.value(); it++)
			{
				dims[idx++] = generateExprInstruction(it->get()->getExpr(), b);
			}
			llvm::ArrayType* at = nullptr;
	
			for (std::size_t i = dims.size() - 1; i != UINT64_MAX; --i)
			{
				if (llvm::ConstantInt* _const = llvm::dyn_cast<llvm::ConstantInt>(dims[i]))
				{
					at = llvm::ArrayType::get(at ? at : LlvmTypeGenerator::convertAstToLlvmType(type->getType()), _const->getZExtValue());
				}
				else
				{
					assert(0);
					return nullptr;
				}
			}
			if (at)
				return dump_op(b.CreateAlloca(at, nullptr, stmt->getName()));
		}
		return nullptr;
	}
	static llvm::Value* generateDeclInstruction(AstStatement* stmt, llvm::IRBuilder<>& b)
	{
		llvm::Value* val = nullptr;
		if (AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(stmt->lhs()))
		{
			AstExpr* rhs = stmt->rhs();
			AstType* type = decl->getVarType();
			if (type)
			{
				if (type->isArray())
				{
					val = generateDeclArrayInstruction(decl, b);
				}
				else
				{
					val = b.CreateAlloca(LlvmTypeGenerator::convertAstToLlvmType(type->getType()), nullptr, decl->getName());
				}
			}
			else
			{
				assert(0);
				return nullptr;
			}
			getLlvmCache<>().insertElement(val, decl);
			if (rhs)
			{
				llvm::Value* initVal = generateExprInstruction(rhs, b);
				b.CreateStore(initVal, val);
			}
		}
		return val;
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
				generateRhsInstruction(_stmt, b);
				break;
			case AstStatement::STMT_TYPE::RET_STMT:
				{
					llvm::Value* retVal = generateRetInstruction(_stmt, b);
					break;
				}
			case AstStatement::STMT_TYPE::DECLARATION:
				{
				generateDeclInstruction(_stmt, b);
				}
			}
			return ;
		}
		return;
	}
};