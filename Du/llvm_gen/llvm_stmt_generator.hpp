#pragma once
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Module.h>
#include <cassert>
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
#include "../ast/AstVisitor.hpp"
#include "../ast/AstControlBlock.h"
#include <iostream>

class llvmStmtGenerator
{
public:
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
	static llvm::Value* readLValueFromMemory(AstElement* element, llvm::Value* value, llvm::IRBuilder<>& b)
	{
		if (element && element->isLValue())
		{
			assert(value && value->getType()->isPointerTy() && "Compiler bug: An l-value must be a pointer here.");

			llvm::Type* loadType = getLlvmCache<>().getType(value);
			assert(loadType && "Compiler bug: Type for l-value not found in cache.");

			return b.CreateLoad(loadType, value);
		}
		return value;
	}

	static llvm::Value* createImplicitCast(llvm::Value* val, llvm::Type* destType, llvm::IRBuilder<>& b) {
		if (!val || !destType || val->getType() == destType) return val;

		llvm::Type* srcType = val->getType();

		if (srcType->isIntegerTy() && destType->isIntegerTy()) {
			if (srcType->getIntegerBitWidth() > destType->getIntegerBitWidth()) {
				return b.CreateTrunc(val, destType, "implicit_trunc");
			}
			else if (srcType->getIntegerBitWidth() < destType->getIntegerBitWidth()) {
				return b.CreateSExt(val, destType, "implicit_sext");
			}
		}

		if (srcType->isFloatingPointTy() && destType->isFloatingPointTy()) {
			if (srcType->getTypeID() > destType->getTypeID()) return b.CreateFPTrunc(val, destType);
			else return b.CreateFPExt(val, destType);
		}

		if (srcType->isIntegerTy() && destType->isFloatingPointTy()) return b.CreateSIToFP(val, destType);
		if (srcType->isFloatingPointTy() && destType->isIntegerTy()) return b.CreateFPToSI(val, destType);

		return val;
	}

	struct LlvmExprVisitor {
		llvm::IRBuilder<>& b;
		AstExpr* currentExpr;
		AstType* expectedType;

		LlvmExprVisitor(llvm::IRBuilder<>& builder, AstExpr* expr, AstType* expected)
			: b(builder), currentExpr(expr), expectedType(expected) {
		}

		llvm::Value* visit(AstExpr* expr) {
			if (!expr) return nullptr;
			LlvmExprVisitor childVisitor(b, expr, nullptr);
			return expr->accept(childVisitor);
		}

		llvm::Value* operator()(AstNodes::BinaryExpr& binOp) {
			std::cout << "BinaryExpr Address: " << &binOp << " Left: " << binOp.left.get() << " Right: " << binOp.right.get() << std::endl;
			llvm::Value* l = visit(binOp.left.get());
			llvm::Value* r = visit(binOp.right.get());

			l = readLValueFromMemory(binOp.left.get(), l, b);
			r = readLValueFromMemory(binOp.right.get(), r, b);

			if (l->getType() != r->getType()) {
				if (l->getType()->isIntegerTy() && r->getType()->isIntegerTy()) {
					if (l->getType()->getIntegerBitWidth() < r->getType()->getIntegerBitWidth())
						l = createImplicitCast(l, r->getType(), b);
					else
						r = createImplicitCast(r, l->getType(), b);
				}
			}

			switch (binOp.op) {
			case AstNodes::BinaryOpType::Addition: return b.CreateAdd(l, r, "add");
			case AstNodes::BinaryOpType::Subtraction: return b.CreateSub(l, r, "sub");
			case AstNodes::BinaryOpType::Multiplication: return b.CreateMul(l, r, "mul");
			case AstNodes::BinaryOpType::Division: return b.CreateSDiv(l, r, "div");
			}
			return nullptr;
		}

		llvm::Value* operator()(AstNodes::CmpExpr& cmpOp) {
			llvm::Value* l = visit(cmpOp.left.get());
			llvm::Value* r = visit(cmpOp.right.get());
			l = readLValueFromMemory(cmpOp.left.get(), l, b);
			r = readLValueFromMemory(cmpOp.right.get(), r, b);

			AstType* type = cmpOp.left->getType();
			if (!type) type = cmpOp.right->getType();

			bool isFloat = (l->getType()->isFloatingPointTy());
			bool isUnsigned = type ? type->isUnsigned() : false;

			switch (cmpOp.op) {
			case AstNodes::CmpOpType::Equal: return isFloat ? b.CreateFCmpOEQ(l, r) : b.CreateICmpEQ(l, r);
			case AstNodes::CmpOpType::NotEqual: return isFloat ? b.CreateFCmpONE(l, r) : b.CreateICmpNE(l, r);
			case AstNodes::CmpOpType::GreaterThan: return isFloat ? b.CreateFCmpOGT(l, r) : (isUnsigned ? b.CreateICmpUGT(l, r) : b.CreateICmpSGT(l, r));
			case AstNodes::CmpOpType::GreaterOrEq: return isFloat ? b.CreateFCmpOGE(l, r) : (isUnsigned ? b.CreateICmpUGE(l, r) : b.CreateICmpSGE(l, r));
			case AstNodes::CmpOpType::LessThan: return isFloat ? b.CreateFCmpOLT(l, r) : (isUnsigned ? b.CreateICmpULT(l, r) : b.CreateICmpSLT(l, r));
			case AstNodes::CmpOpType::LessOrEq: return isFloat ? b.CreateFCmpOLE(l, r) : (isUnsigned ? b.CreateICmpULE(l, r) : b.CreateICmpSLE(l, r));
			}
			return nullptr;
		}

		llvm::Value* operator()(AstNodes::UnaryExpr& unOp) {
			llvm::Value* operand = visit(unOp.operand.get());
			operand = readLValueFromMemory(unOp.operand.get(), operand, b);
			switch (unOp.op) {
			case AstNodes::UnaryOpType::Negation: return b.CreateNeg(operand, "neg");
			}
			return nullptr;
		}

		llvm::Value* operator()(AstNodes::LiteralExpr& lit) {
			llvm::Type* literalLlvmType = nullptr;

			if (expectedType) {
				literalLlvmType = LlvmTypeGenerator::convertAstToLlvmType(expectedType->getType());
			}
			else if (currentExpr && currentExpr->getType()) {
				literalLlvmType = LlvmTypeGenerator::convertAstToLlvmType(currentExpr->getType()->getType());
			}

			if (!literalLlvmType) {
				return std::visit(overloaded{
				   [&](long long v) -> llvm::Value* { return llvm::ConstantInt::get(llvm::Type::getInt64Ty(b.getContext()), v); },
				   [&](unsigned long long v) -> llvm::Value* { return llvm::ConstantInt::get(llvm::Type::getInt64Ty(b.getContext()), v); },
				   [&](bool v) -> llvm::Value* { return llvm::ConstantInt::get(llvm::Type::getInt1Ty(b.getContext()), v); },
				   [&](double v) -> llvm::Value* { return llvm::ConstantFP::get(llvm::Type::getFloatTy(b.getContext()), v); },
				   [&](const std::string&) -> llvm::Value* { return nullptr; }
					}, lit.value);
			}

			return std::visit(overloaded{
			   [&](long long v) -> llvm::Value* {
				   return llvm::ConstantInt::get(literalLlvmType, v);
			   },
			   [&](unsigned long long v) -> llvm::Value* {
				   return llvm::ConstantInt::get(literalLlvmType, v);
			   },
			   [&](bool v) -> llvm::Value* {
				   return llvm::ConstantInt::get(literalLlvmType, v);
			   },
			   [&](double v) -> llvm::Value* {
				   return llvm::ConstantFP::get(literalLlvmType, v);
			   },
			   [&](const std::string&) -> llvm::Value* { return nullptr; }
				}, lit.value);
		}

		llvm::Value* operator()(AstNodes::VariableRefExpr& varRef) {
			if (varRef.declaration) {
				return getLlvmCache<>().getValue(varRef.declaration);
			}
			else {
				return getAllocInst(varRef.name, b);
			}
		}

		llvm::Value* operator()(AstNodes::FunctionCallExpr& call) {
			llvm::Module* module = b.GetInsertBlock()->getModule();
			llvm::Function* fun = module->getFunction(call.funcName);
			if (fun) {
				std::vector<llvm::Value*> args;
				auto* funType = fun->getFunctionType();
				for (size_t i = 0; i < call.args.size(); ++i) {
					llvm::Value* argVal = visit(call.args[i].get());
					argVal = readLValueFromMemory(call.args[i].get(), argVal, b);
					
					if (i < funType->getNumParams()) {
						llvm::Type* expectedType = funType->getParamType(i);
						if (argVal->getType() != expectedType) {
							argVal = createImplicitCast(argVal, expectedType, b);
						}
					}
					args.push_back(argVal);
				}
				return b.CreateCall(fun, args);
			}
			return nullptr;
		}

		llvm::Value* operator()(AstNodes::CastExpr& cast) {
			llvm::Value* srcValue = visit(cast.expr.get());
			srcValue = readLValueFromMemory(cast.expr.get(), srcValue, b);

			llvm::Type* destType = nullptr;
			if (currentExpr && currentExpr->getType())
				destType = LlvmTypeGenerator::convertAstToLlvmType(currentExpr->getType()->getType());

			if (!destType && expectedType)
				destType = LlvmTypeGenerator::convertAstToLlvmType(expectedType->getType());

			if (!srcValue || !destType) return nullptr;

			auto op = cast.castOp;

			if (op & CastOp::SIToFP) return b.CreateSIToFP(srcValue, destType, "cast_sitofp");
			if (op & CastOp::UIToFP) return b.CreateUIToFP(srcValue, destType, "cast_uitofp");
			if (op & CastOp::FPToSI) return b.CreateFPToSI(srcValue, destType, "cast_fptosi");
			if (op & CastOp::FPToUI) return b.CreateFPToUI(srcValue, destType, "cast_fptoui");
			if (op & CastOp::FPExt)  return b.CreateFPExt(srcValue, destType, "cast_fpext");
			if (op & CastOp::FPTrunc) return b.CreateFPTrunc(srcValue, destType, "cast_fptrunc");
			if (op & CastOp::SExt)   return b.CreateSExt(srcValue, destType, "cast_sext");
			if (op & CastOp::ZExt)   return b.CreateZExt(srcValue, destType, "cast_zext");
			if (op & CastOp::Trunc)  return b.CreateTrunc(srcValue, destType, "cast_trunc");

			if (op & CastOp::BitCast) {
				if (srcValue->getType() != destType) return b.CreateBitCast(srcValue, destType, "cast_bitcast");
				return srcValue;
			}

			return srcValue;
		}

		llvm::Value* operator()(AstNodes::ArrayIndexingExpr& expr) {
			llvm::Value* arrBase = visit(expr.arrayExpr.get());
			AstVariableDecl* decl = nullptr;

			std::visit(overloaded{
				[&](AstNodes::VariableRefExpr& v) {
					decl = ast_element_cast<AstVariableDecl>(v.declaration);
				},
				[](auto&&) {}
				}, expr.arrayExpr->getExpression());

			if (arrBase && decl)
			{
				llvm::ArrayType* at = generateArrayTypeInstruction(decl->getVarType(), b);
				std::vector<llvm::Value*> llvmIdxs{ llvm::ConstantInt::get(llvm::Type::getInt64Ty(b.getContext()), 0) };

				for (auto& idx : expr.indices) {
					llvmIdxs.push_back(visit(idx.get()));
				}
				return b.CreateGEP(at, arrBase, llvmIdxs);
			}
			return nullptr;
		}

		template <typename T>
		llvm::Value* operator()(T& node) {
			assert(false && "Unhandled AST Node in LLVM Expr Visitor");
			return nullptr;
		}
	};

	static llvm::Value* generateExprInstruction(AstExpr* expr, llvm::IRBuilder<>& b, AstType* expectedType = nullptr)
	{
		if (!expr) return nullptr;
		LlvmExprVisitor visitor(b, expr, expectedType);
		return expr->accept(visitor);
	}

	static void generateAssgnInstruction(AstStatement* assgn, llvm::IRBuilder<>& b)
	{
		if (assgn && assgn->lhs() && assgn->rhs())
		{
			AstElement* lhs = assgn->lhs();
			llvm::Value* variable = nullptr;
			AstType* targetAstType = nullptr;

			if (AstExpr* expr = ast_element_cast<AstExpr>(lhs))
			{
				variable = generateExprInstruction(expr, b);
				targetAstType = expr->getType();

				if (!targetAstType) {
					std::visit(overloaded{
					   [&](AstNodes::VariableRefExpr& v) {
						   if (auto* decl = ast_element_cast<AstVariableDecl>(v.declaration))
							   targetAstType = decl->getVarType();
					   },
					   [&](AstNodes::ArrayIndexingExpr& idx) {
							// Infer type if possible
					   },
					   [](auto&&) {}
						}, expr->getExpression());
				}
			}
			else {
				variable = getAllocInst(assgn->lhs()->getName(), b);
				if (AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(lhs)) {
					targetAstType = decl->getVarType();
				}
			}

			llvm::Value* exprInstruction = generateExprInstruction(ast_element_cast<AstExpr>(assgn->rhs()), b, targetAstType);
			exprInstruction = readLValueFromMemory(assgn->rhs(), exprInstruction, b);

			if (variable && targetAstType) {
				llvm::Type* valType = LlvmTypeGenerator::convertAstToLlvmType(targetAstType->getType());

				if (exprInstruction->getType() != valType) {
					exprInstruction = createImplicitCast(exprInstruction, valType, b);
				}
			}

			if (variable && exprInstruction)
				b.CreateStore(exprInstruction, variable, false);
		}
	}

	static llvm::Value* generateRhsInstruction(AstStatement* stmt, llvm::IRBuilder<>& b, AstType* expectedType = nullptr)
	{
		if (stmt)
			if (AstExpr* expr = ast_element_cast<AstExpr>(stmt->rhs()))
			{
				return generateExprInstruction(expr, b, expectedType);
			}
		return nullptr;
	}

	static llvm::Value* generateRetInstruction(AstStatement* stmt, llvm::IRBuilder<>& b)
	{
		llvm::Value* retVal = generateRhsInstruction(stmt, b);
		if (retVal)
		{
			retVal = readLValueFromMemory(stmt->rhs(), retVal, b);

			llvm::Function* func = b.GetInsertBlock()->getParent();
			llvm::Type* retType = func->getReturnType();
			if (retVal->getType() != retType) {
				retVal = createImplicitCast(retVal, retType, b);
			}

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
				dims[idx++] = generateExprInstruction(ast_element_cast<AstExpr>(it->get()->getExpr()), b);
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
				llvm::Value* initVal = generateExprInstruction(rhs, b, type);
				initVal = readLValueFromMemory(rhs, initVal, b);

				initVal = createImplicitCast(initVal, generatedType, b);

				b.CreateStore(initVal, val);
			}
		}
		return val;
	}

	static void generateConditionBlockInstruction(AstStatement* stmt, llvm::IRBuilder<>& b)
	{
		if (!stmt || !stmt->isControlBlockStmt()) return;
		auto& controlBlock = stmt->getControlBlock();
		if (!controlBlock) return;

		llvm::Function* TheFunction = b.GetInsertBlock()->getParent();

		auto getCondVal = [&](llvm::IRBuilder<>& builder) -> llvm::Value* {
			llvm::Value* condVal = generateExprInstruction(stmt->rhs(), builder);
			if (!condVal) return nullptr;
			return readLValueFromMemory(stmt->rhs(), condVal, builder);
		};

		if (controlBlock->getBlockType() == AstControlBlock::type::CONDITION_BLOCK)
		{
			llvm::Value* condVal = getCondVal(b);
			if (!condVal) return;

			llvm::BasicBlock* ThenBB = llvm::BasicBlock::Create(b.getContext(), "then", TheFunction);
			llvm::BasicBlock* ElseBB = controlBlock->getOtherBranch() ? llvm::BasicBlock::Create(b.getContext(), "else") : nullptr;
			llvm::BasicBlock* MergeBB = llvm::BasicBlock::Create(b.getContext(), "ifcont");

			b.CreateCondBr(condVal, ThenBB, ElseBB ? ElseBB : MergeBB);

			// Emit Then
			b.SetInsertPoint(ThenBB);

			auto& thenScope = controlBlock->getBranchScope();
			if (thenScope) {
				for (auto& el : thenScope->getStmts()) {
					generateInstruction(el.get(), b);
				}
			}

			if (!ThenBB->getTerminator())
				b.CreateBr(MergeBB);

			if (ElseBB) {
				TheFunction->insert(TheFunction->end(), ElseBB);
				b.SetInsertPoint(ElseBB);

				auto& elseScope = controlBlock->getOtherBranch();
				if (elseScope) {
					for (auto& el : elseScope->getStmts()) {
						generateInstruction(el.get(), b);
					}
				}

				if (!ElseBB->getTerminator())
					b.CreateBr(MergeBB);
			}

			TheFunction->insert(TheFunction->end(), MergeBB);
			b.SetInsertPoint(MergeBB);
		}
		else if (controlBlock->getBlockType() == AstControlBlock::type::LOOP_BLOCK)
		{
			llvm::BasicBlock* CondBB = llvm::BasicBlock::Create(b.getContext(), "loopcond", TheFunction);
			llvm::BasicBlock* BodyBB = llvm::BasicBlock::Create(b.getContext(), "loopbody", TheFunction);
			llvm::BasicBlock* EndBB = llvm::BasicBlock::Create(b.getContext(), "loopend");

			b.CreateBr(CondBB);

			b.SetInsertPoint(CondBB);
			llvm::Value* loopCondVal = getCondVal(b);
			if (!loopCondVal) return;

			b.CreateCondBr(loopCondVal, BodyBB, EndBB);

			b.SetInsertPoint(BodyBB);
			auto& bodyScope = controlBlock->getBranchScope();
			if (bodyScope) {
				for (auto& el : bodyScope->getStmts()) {
					generateInstruction(el.get(), b);
				}
			}

			if (!b.GetInsertBlock()->getTerminator())
				b.CreateBr(CondBB);

			TheFunction->insert(TheFunction->end(), EndBB);
			b.SetInsertPoint(EndBB);
		}
	}

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
				break;
			}
			case AstStatement::STMT_TYPE::CONDITION_BLOCK:
			{
				generateConditionBlockInstruction(_stmt, b);
				break;
			}
			case AstStatement::STMT_TYPE::LOOP_BLOCK:
			{
				generateConditionBlockInstruction(_stmt, b);
				break;
			}
			}
			return;
		}
		return;
	}
};