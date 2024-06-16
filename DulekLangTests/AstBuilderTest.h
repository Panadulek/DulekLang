#include "../Du/ast/AstBuilder.hpp"
#include "../Du/ast/AstScope.hpp"
#include "../Du/ast/AstVariableDecl.hpp"
#include<gtest/gtest.h>
#include <string_view>
struct AstBuilderTest
{
	static bool AddElementsAndScope()
	{
		const std::string fname("addScopesElement.du");
		AstScope::GlobalApi::addFile(fname);
		AstBuilder builder(fname);
		builder.addElement(std::make_unique<AstScope>("fun1", builder.getActualScope()));
		constexpr std::size_t sizeInFun = 2;
		builder.addElement(std::make_unique<AstVariableDecl>("u8", "var1"));
		builder.addElement(std::make_unique<AstVariableDecl>("u8", "var2"));
		std::size_t size = builder.getActualScope()->getElementsCounter();
		builder.exitScope();
		return sizeInFun == size && builder.getActualScope()->getElementsCounter() == 1;
	}
	static bool AddElementsAndScope2()
	{
		const std::string fname("addScopesElement.du");
		AstScope::GlobalApi::addFile(fname);
		AstBuilder builder(fname);
		builder.addElement(std::make_unique<AstScope>("fun1", builder.getActualScope()));
		constexpr std::size_t sizeInFun = 2;
		builder.addElement(std::make_unique<AstVariableDecl>("u8", "var1"));
		builder.addElement(std::make_unique<AstVariableDecl>("u8", "var2"));
		std::size_t size = builder.getActualScope()->getElementsCounter();
		builder.exitScope();
		builder.addElement(std::make_unique<AstVariableDecl>("u8", "var3"));
		return sizeInFun == size && builder.getActualScope()->getElementsCounter() == 2;
	}
};

TEST(AST_BUILDER, ADD_SCOPES_AND_ELEMENTS1)
{
	EXPECT_TRUE(AstBuilderTest::AddElementsAndScope());
}


TEST(AST_BUILDER, ADD_SCOPES_AND_ELEMENTS2)
{
	EXPECT_TRUE(AstBuilderTest::AddElementsAndScope2());
}