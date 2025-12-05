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
            // Fallback removed for newer LLVM versions
			assert(loadType && "Compiler bug: Type for l-value not found in cache.");

			return b.CreateLoad(loadType, value);
		}
		return value;
	}

    // --- VISITOR FUNCTOR ---
    struct LlvmExprVisitor {
        llvm::IRBuilder<>& b;
        AstExpr* currentExpr; 

        LlvmExprVisitor(llvm::IRBuilder<>& builder, AstExpr* expr) : b(builder), currentExpr(expr) {}

        // Recursion helper
        llvm::Value* visit(AstExpr* expr) {
            if(!expr) return nullptr;
            LlvmExprVisitor childVisitor(b, expr);
            return expr->accept(childVisitor);
        }

        llvm::Value* operator()(AstNodes::BinaryExpr& binOp) {
             llvm::Value* l = visit(binOp.left.get());
             llvm::Value* r = visit(binOp.right.get());
             
             l = readLValueFromMemory(binOp.left.get(), l, b);
             r = readLValueFromMemory(binOp.right.get(), r, b);
             
             switch(binOp.op) {
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
             if(!type) type = cmpOp.right->getType();
             assert(type && "Missing type info for CmpOp");

             bool isFloat = type->isFloatingPoint();
             bool isUnsigned = type->isUnsigned() || type->getType() == BasicTypes::BOOL;

             switch(cmpOp.op) {
                 case AstNodes::CmpOpType::Equal: return isFloat ? b.CreateFCmpOEQ(l, r) : b.CreateICmpEQ(l, r);
                 case AstNodes::CmpOpType::NotEqual: return isFloat ? b.CreateFCmpONE(l, r) : b.CreateICmpNE(l, r);
                 case AstNodes::CmpOpType::GreaterThan: return isFloat ? b.CreateFCmpOGT(l, r) : (isUnsigned ? b.CreateICmpUGT(l, r) : b.CreateICmpSGT(l, r));
                 case AstNodes::CmpOpType::GreaterOrEq: return isFloat ? b.CreateFCmpOGE(l, r) : (isUnsigned ? b.CreateICmpUGE(l, r) : b.CreateICmpSGE(l, r));
                 case AstNodes::CmpOpType::LessThan: return isFloat ? b.CreateFCmpOLT(l, r) : (isUnsigned ? b.CreateICmpULT(l, r) : b.CreateICmpSLT(l, r));
                 case AstNodes::CmpOpType::LessOrEq: return isFloat ? b.CreateFCmpOLE(l, r) : (isUnsigned ? b.CreateICmpULE(l, r) : b.CreateICmpSLE(l, r));
             }
             return nullptr;
        }

        llvm::Value* operator()(AstNodes::LiteralExpr& lit) {
             return std::visit(overloaded{
                [&](long long v) -> llvm::Value* { return llvm::ConstantInt::get(LlvmTypeGenerator::convertAstToLlvmType(BasicTypes::U64), v); }, 
                [&](unsigned long long v) -> llvm::Value* { return llvm::ConstantInt::get(LlvmTypeGenerator::convertAstToLlvmType(BasicTypes::U64), v); }, 
                [&](bool v) -> llvm::Value* { return llvm::ConstantInt::get(LlvmTypeGenerator::convertAstToLlvmType(BasicTypes::BOOL), v); },
                [&](double v) -> llvm::Value* { return llvm::ConstantFP::get(LlvmTypeGenerator::convertAstToLlvmType(BasicTypes::F32), v); },
                [&](const std::string&) -> llvm::Value* { return nullptr; /* TODO */ }
            }, lit.value);
        }

        llvm::Value* operator()(AstNodes::VariableRefExpr& varRef) {
             if(varRef.declaration) {
                 return getLlvmCache<>().getValue(varRef.declaration);
             } else {
                 return getAllocInst(varRef.name, b);
             }
        }

        llvm::Value* operator()(AstNodes::FunctionCallExpr& call) {
             llvm::Module* module = b.GetInsertBlock()->getModule();
             llvm::Function* fun = module->getFunction(call.funcName);
             if(fun) {
                 std::vector<llvm::Value*> args;
                 for(auto& argExpr : call.args) {
                     args.push_back(visit(argExpr.get()));
                 }
                 return b.CreateCall(fun, args);
             }
             return nullptr;
        }

        llvm::Value* operator()(AstNodes::CastExpr& cast) {
            llvm::Value* srcValue = visit(cast.expr.get());
            srcValue = readLValueFromMemory(cast.expr.get(), srcValue, b);

            if (!srcValue || !currentExpr->getType()) return nullptr;
                
            llvm::Type* destType = LlvmTypeGenerator::convertAstToLlvmType(currentExpr->getType()->getType());
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

            std::visit(overloaded {
                [&](AstNodes::VariableRefExpr& v) {
                    decl = ast_element_cast<AstVariableDecl>(v.declaration);
                },
                [](auto&&) {}
            }, expr.arrayExpr->getExpression());

            if (arrBase && decl)
            {
                llvm::ArrayType* at = generateArrayTypeInstruction(decl->getVarType(), b);
                std::vector<llvm::Value*> llvmIdxs{llvm::ConstantInt::get(llvm::Type::getInt64Ty(b.getContext()), 0)};
                
                for (auto& idx : expr.indices) {
                    llvmIdxs.push_back(visit(idx.get()));
                }
                return b.CreateGEP(at, arrBase, llvmIdxs);
            }
            return nullptr;
        }
        
        template <typename T>
        llvm::Value* operator()(T& node) { return nullptr; }
    };

	static llvm::Value* generateExprInstruction(AstExpr* expr, llvm::IRBuilder<>& b)
	{
        if(!expr) return nullptr;
        LlvmExprVisitor visitor(b, expr);
        return expr->accept(visitor);
	}

	static void generateAssgnInstruction(AstStatement* assgn, llvm::IRBuilder<>& b)
	{
		if (assgn && assgn->lhs() && assgn->rhs())
		{
			llvm::Value* exprInstruction = generateExprInstruction(ast_element_cast<AstExpr>(assgn->rhs()), b);
			exprInstruction = readLValueFromMemory(assgn->rhs(), exprInstruction, b);
			
			AstElement* lhs = assgn->lhs();
			llvm::Value* variable = nullptr;
			if (AstExpr* expr = ast_element_cast<AstExpr>(lhs))
			{
                // LValue expr
                variable = generateExprInstruction(expr, b);
			}
			else
				 variable = getAllocInst(assgn->lhs()->getName(), b);
            
            if(variable && exprInstruction)
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
            retVal = readLValueFromMemory(stmt->rhs(), retVal, b); 
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
				llvm::Value* initVal = generateExprInstruction(rhs, b);
                initVal = readLValueFromMemory(rhs, initVal, b);
				b.CreateStore(initVal, val);
			}
		}
		return val;
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
				}
			}
			return ;
		}
		return;
	}
};
