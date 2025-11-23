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
#include "../ast/CastGraph.hpp"
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
			if (AstRef* ref = ast_element_cast<AstRef>(left->right()))
			{
				if(AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(ref->ref()))
				{
					if (AstExpr* _expr = ast_element_cast<AstExpr>(expr->right()))
					{
						llvm::Value* arr = generateExprInstruction(left, b);
						llvm::ArrayType* at = generateArrayTypeInstruction(decl->getVarType(), b);
						auto dims = AstExpr::transformExprToDimsArray(_expr);
						std::vector<llvm::Value*> llvmIdxs{llvm::ConstantInt::get(llvm::Type::getInt64Ty(b.getContext()), 0)};
						if (dims)
						{
							for (auto it = dims->begin(); it != dims->end(); ++it)
							{

								llvmIdxs.push_back(generateExprInstruction(it->get()->getExpr(), b));
							}
							return b.CreateGEP(at, arr, llvmIdxs);
						}
					}
				}
			}
		}
		return nullptr;
	}
	static llvm::Value* generateCast(AstExpr* expr, llvm::IRBuilder<>& b)
	{
		if (!expr || !expr->getCastOp().has_value())
			return nullptr;
		AstElement* left = expr->left();
		if (left)
		{
			llvm::Value* value = nullptr;
			if (auto subExpr = ast_element_cast<AstExpr>(left))
			{
				value = generateExprInstruction(subExpr, b);
			}
			else 
				value = getValueFromExpr(left, b);
			if (!value)
				return nullptr;
			AstType* type = expr->getType();
			auto srcValue =  value->getType()->isPointerTy() ? b.CreateLoad(getLlvmCache<>().getType(value), value) : value;
			if (type)
			{
				llvm::Type* destType = LlvmTypeGenerator::convertAstToLlvmType(type->getType());
				auto op = expr->getCastOp().value();
				if (op & CastOp::SIToFP) {
					return b.CreateSIToFP(srcValue, destType, "cast_sitofp");
				}
				if (op & CastOp::UIToFP) {
					return b.CreateUIToFP(srcValue, destType, "cast_uitofp");
				}
				if (op & CastOp::FPToSI) {
					return b.CreateFPToSI(srcValue, destType, "cast_fptosi");
				}
				if (op & CastOp::FPToUI) {
					return b.CreateFPToUI(srcValue, destType, "cast_fptoui");
				}
				if (op & CastOp::FPExt) {
					return b.CreateFPExt(srcValue, destType, "cast_fpext");
				}
				if (op & CastOp::FPTrunc) {
					return b.CreateFPTrunc(srcValue, destType, "cast_fptrunc");
				}
				if (op & CastOp::SExt) {
					return b.CreateSExt(srcValue, destType, "cast_sext");
				}
				if (op & CastOp::ZExt) {
					return b.CreateZExt(srcValue, destType, "cast_zext");
				}
				if (op & CastOp::Trunc) {
					return b.CreateTrunc(srcValue, destType, "cast_trunc");
				}
				if (op & CastOp::BitCast) {
					if (srcValue->getType() != destType) {
						return b.CreateBitCast(srcValue, destType, "cast_bitcast");
					}
					return srcValue; // No-op
				}
			}
			return srcValue;
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
			case BasicTypes::BOOL:
				return llvm::ConstantInt::get(LlvmTypeGenerator::convertAstToLlvmType(aconst->getType()), aconst->get_bool_value());
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
			return  getLlvmCache<>().getValue(ref);
		
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

	static llvm::Value* generateBinaryOpInstruction(llvm::Value* l, llvm::Value* r, llvm::IRBuilder<>& b, AstExpr::Operation op, std::string_view description)
	{
		if (l && l->getType()->isPointerTy())
		{
			l = b.CreateLoad(getLlvmCache<>().getType(l), l);
		}
		if (r && r->getType()->isPointerTy())
		{
			r = b.CreateLoad(getLlvmCache<>().getType(r), r);
		}
		switch (op)
		{
			case AstExpr::Operation::Addition:
				return b.CreateAdd(l, r, description);
			case AstExpr::Operation::Subtraction:
				return b.CreateSub(l,r, description);
			case AstExpr::Operation::Multiplication:
				return b.CreateMul(l, r, description);
			case AstExpr::Operation::Division:
				return b.CreateSDiv(l, r, description);
			case AstExpr::Operation::Unary_negation:
				return b.CreateNeg(l, description);
		}
	}

	static llvm::Value* generateExprInstruction(AstExpr* expr, llvm::IRBuilder<>& b)
	{
		if (expr->op() == AstExpr::Operation::Arr_Indexing)
		{
			return generateArrayIndexing(expr, b);
		}
		else if (expr->op() == AstExpr::Operation::Cast)
		{
			return generateCast(expr, b);
		}
		else
		{
			llvm::Value* l = getValueFromExpr(expr->left(), b);
			llvm::Value* r = getValueFromExpr(expr->right(), b);
			std::string description =
#ifdef _DEBUG
				std::format("{}, {} and {}", static_cast<uint8_t>(expr->op()), expr->left() ? expr->left()->getName() : "null",
					expr->right() ? expr->right()->getName() : "null");
			if (l)
			{
				llvm::errs() << "left: \n";
				l->dump();
				l->getType()->dump();
			}
			if (r)
			{
				llvm::errs() << "right: \n";
				r->dump();
				r->getType()->dump();
			}
#else
				"";
#endif
		
			if (expr->isBinaryOp())
			{
				return generateBinaryOpInstruction(l, r, b, expr->op(), description);
			}
			switch (expr->op())
			{		
			case AstExpr::Operation::Call_fun:
				return l;
			case AstExpr::Operation::Reference:
			case AstExpr::Operation::ConstValue:
				return r;
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
			if (assgn->rhs()->isLValue())
			{
				exprInstruction = b.CreateLoad(getLlvmCache<>().getType(exprInstruction), exprInstruction);
			}
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
		return nullptr;
	}

	static llvm::ArrayType* generateArrayTypeInstruction(AstType* type, llvm::IRBuilder<>& b)
	{
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
			for (auto it = begin.value(); it != end.value(); it++)
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
			return at;
		}
		return nullptr;
	}

	static llvm::Type* generateDeclArrayInstruction(AstVariableDecl* stmt, llvm::IRBuilder<>& b)
	{
		AstType* type = stmt->getVarType();
		if (!type)
		{
			assert(0);
			return nullptr;
		}
		return generateArrayTypeInstruction(type, b); 
	}
		



	static llvm::Value* generateDeclInstruction(AstStatement* stmt, llvm::IRBuilder<>& b)
	{
		llvm::Value* val = nullptr;
		if (AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(stmt->lhs()))
		{
			AstExpr* rhs = stmt->rhs();
			AstType* type = decl->getVarType();
			llvm::Type* generatedType = nullptr;
			if (type)
			{
				if (type->isArray())
				{
					generatedType = generateDeclArrayInstruction(decl, b);
				}
				else
				{
					generatedType = LlvmTypeGenerator::convertAstToLlvmType(type->getType());
				}
				val = b.CreateAlloca(generatedType, nullptr, decl->getName());
			}
			else
			{
				assert(0);
				return nullptr;
			}
			getLlvmCache<>().insertElement(val, decl, generatedType);
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