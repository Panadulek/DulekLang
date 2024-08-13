#include<gtest/gtest.h>
#ifndef NDEBUG
#define NDEBUG
#endif
#include "../Du/ast/AstScope.hpp"
#include <string_view>
#include <intrin.h>
#include "../Du/ast/AstBuildSystem.hpp"
class AstScopeUnitTests
{
	using GlobalApi = AstScope::GlobalApi;

public:

	static bool TestAstScopeGlobalApiAddFile()
	{
		bool ret = AstScope::GlobalApi::addFile("file1.du");
		GlobalApi::clearGlobalScopes();
		return ret;
	}
	static bool TestAstScopeGlobalApiAddFile2()
	{
		AstScope::GlobalApi::addFile("file2.du");
		bool ret = AstScope::GlobalApi::addFile("file2.du");
		GlobalApi::clearGlobalScopes();
		return ret;
	}

	static bool TestAstScopeGlobalApiGetScope1()
	{
		const std::string filename = "file.du";
		AstScope::GlobalApi::addFile(filename.c_str());
		bool ret = GlobalApi::getGlobalScopeForFile(filename.c_str());
		GlobalApi::clearGlobalScopes();
		return ret;
	}

	static bool TestAstScopeGlobalApiGetScope2()
	{
		const std::string filename = "file.du";
		bool ret = GlobalApi::getGlobalScopeForFile(filename.c_str());
		GlobalApi::clearGlobalScopes();
		return ret;
	}


	static bool TestUpdateScope()
	{
		const std::string filename = "file.du";
		bool ret = GlobalApi::addFile(filename);
		AstBuildSystem::Instance().provideNextFilename(filename);
		auto newScope = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("scope", AstScope::GlobalApi::getCurrentScope());
		auto ptrScope = newScope.get();
		AstBuildSystem::Instance().getBuilder().addElement(std::move(newScope));
		return AstBuildSystem::Instance().getBuilder().getActualScope() == ptrScope;
	}


};



TEST(GLOBAL_API_ADD_FILE_TO_SCOPE, GLOBAL_API_ADD_FILE1)
{
	EXPECT_TRUE(AstScopeUnitTests::TestAstScopeGlobalApiAddFile());
}

TEST(GLOBAL_API_ADD_FILE_TO_SCOPE, GLOBAL_API_ADD_FILE2)
{
	EXPECT_FALSE(AstScopeUnitTests::TestAstScopeGlobalApiAddFile2());
}

TEST(GLOBAL_API_GET_SCOPE_FOR_FILE, GLOBAL_API_GET_SCOPE1)
{
	EXPECT_TRUE(AstScopeUnitTests::TestAstScopeGlobalApiGetScope1());
}

TEST(GLOBAL_API_GET_SCOPE_FOR_FILE, GLOBAL_API_GET_SCOPE2)
{
	EXPECT_FALSE(AstScopeUnitTests::TestAstScopeGlobalApiGetScope2());
}

TEST(SCOPE, UPDATE_SCOPE)
{
	EXPECT_TRUE(AstScopeUnitTests::TestUpdateScope());
}