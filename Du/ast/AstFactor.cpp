#include "AstFactor.hpp"
#include "AstBuildSystem.hpp"
#include "AstBuilder.hpp"
#include "ScopeDecorator.hpp"


std::unique_ptr<AstScope> AstFactory::ScopeFactor::createScope(std::string_view scopeName, AstElement* parent)
{
	return std::make_unique<AstScope>(scopeName, parent);
}

std::unique_ptr<AstScope> AstFactory::ScopeFactor::createFunction(std::string_view name, AstElement* parent, std::string_view retType, ScopeDecorator::Function::CONTAINER* args)
{
	auto isBuiltInType = AstScope::GlobalApi::isBasicType(retType);
	if (isBuiltInType.first)
	{
		std::unique_ptr<AstScope> newScope = createScope(name, parent);
		newScope->setUpFunction(isBuiltInType.second, args);
		return newScope;
	}
	return nullptr;
}

std::unique_ptr<AstScope> AstFactory::ScopeFactor::createFunction(std::string_view name, std::string_view retType, ScopeDecorator::Function::CONTAINER* args)
{
	return createFunction(name, AstBuildSystem::Instance().getBuilder().getActualScope(), retType, args);
}

AstElement* AstFactory::StatementFactor::createConditionBlockStmt(AstElement* condExpr)
{
	if (condExpr)
	{
		if (AstExpr* expr = ast_element_cast<AstExpr>(condExpr))
		{
			auto* rawCondBlock = dynamic_cast<AstStatement*>(
				AstBuildSystem::Instance().getBuilder().addElement(std::make_unique<AstStatement>(static_cast<AstElement*>(nullptr), std::unique_ptr<AstExpr>(expr), AstStatement::STMT_TYPE::CONDITION_BLOCK, getActualScope()))
				);
			assert(rawCondBlock && rawCondBlock->isControlBlockStmt());
			AstBuildSystem::Instance().getBuilder().beginScope(rawCondBlock->getControlBlock()->getCurrentBranch().get());
			return rawCondBlock;
		}
	}
	return nullptr;
}

std::unique_ptr<AstElement> AstFactory::StatementFactor::createAssigmentVariable(std::string_view lhs, AstElement* rhs, AstScope* currentScope)
{
	if (AstExpr* expr = ast_element_cast<AstExpr>(rhs))
	{
		auto ref = AstBuildSystem::Instance().getFactory().exprFactor().createRef(lhs);
		return std::make_unique<AstStatement>(std::move(ref), std::unique_ptr<AstExpr>(expr), AstStatement::STMT_TYPE::ASSIGN, currentScope);
	}
	return nullptr;
}
