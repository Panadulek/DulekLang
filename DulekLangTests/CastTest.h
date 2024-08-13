#include "../Du/ast/AstCast.hpp"
#include "../Du/ast/AstScope.hpp"
#include "../Du/ast/AstVariableDecl.hpp"
#include "../Du/ast/AstBuildSystem.hpp"
#include <gtest/gtest.h>
#include <inttypes.h>
struct AstCastTest
{
	
	static bool castTestScopeToScope()
	{
		AstElement* ast = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("test", nullptr).release();
		bool ret = ast_element_cast<AstScope>(ast);
		delete ast;
		return ret;
	
	}
	static bool castTestVariableToVariable()
	{
		AstElement* ast = AstBuildSystem::Instance().getFactory().varFactor().createVariable("i8", "test", nullptr).release();
		bool ret = ast_element_cast<AstVariableDecl>(ast);
		delete ast;
		return ret;
	}

	static bool castTestScopeToVariable()
	{
		AstElement* ast = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("test", nullptr).release();
		bool ret = ast_element_cast<AstVariableDecl>(ast);
		delete ast;
		return ret;
	}
	static bool castTestVariableToScope()
	{
		AstElement* ast = AstBuildSystem::Instance().getFactory().varFactor().createVariable("i8", "test", nullptr).release();
		bool ret = ast_element_cast<AstScope>(ast);
		delete ast;
		return ret;
	}

	static bool uniqueCastTestScopeToScope()
	{

		std::unique_ptr<AstElement> ast = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("test", nullptr);
		bool ret = ast_unique_element_cast<AstScope>(ast);
		return ret;

	}
	static bool uniqueCastTestVariableToVariable()
	{
		std::unique_ptr<AstElement> ast = AstBuildSystem::Instance().getFactory().varFactor().createVariable("i8", "test", nullptr);
		bool ret = ast_unique_element_cast<AstVariableDecl>(ast);
		return ret;
	}

	static bool uniqueCastTestVariableToScope()
	{
		std::unique_ptr<AstElement> ast = AstBuildSystem::Instance().getFactory().varFactor().createVariable("i8", "test", nullptr);
		bool ret = ast_unique_element_cast<AstScope>(ast);
		return ret;

	}
	static bool uniqueCastTestScopeToVariable()
	{
		
		std::unique_ptr<AstElement> ast = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("test", nullptr);
		bool ret = ast_unique_element_cast<AstVariableDecl>(ast);
		return ret;
	}

	static bool astCastFasterThanDynamic()
	{
		size_t astCast = performanceAstCast();
		size_t dynCast = performanceDynamicCast();
		printf("cycle of ast = %u, cycle of dynamic = %u\n", astCast, dynCast);
		return astCast < dynCast;
	}
	static bool uniqueAstCastFasterThanDynamic()
	{
		return performanceUniqueAstCast() < performanceUniqueDynamicCast();
	}
private:
	static constexpr std::size_t s_performaceLoopCounter = 10000000;
	static std::size_t performanceDynamicCast()
	{
		std::size_t begin = __rdtsc();
		for (std::size_t i = 0; i < s_performaceLoopCounter; i++)
		{
			AstElement* ptr = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("", nullptr).release();
			AstScope* casted = dynamic_cast<AstScope*>(ptr);
			delete ptr;
		}
		return __rdtsc() - begin;
	}

	static std::size_t performanceAstCast()
	{
		std::size_t begin = __rdtsc();
		for (std::size_t i = 0; i < s_performaceLoopCounter; i++)
		{
			AstElement* ptr = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("", nullptr).release();
			AstScope* casted = ast_element_cast<AstScope>(ptr);
			delete ptr;
		}
		return __rdtsc() - begin;
	}
	static std::size_t performanceUniqueDynamicCast()
	{
		std::size_t begin = __rdtsc();
		for (std::size_t i = 0; i < s_performaceLoopCounter; i++)
		{
			std::unique_ptr<AstElement> ptr = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("test", nullptr);
			AstScope* casted = dynamic_cast<AstScope*>(ptr.get());
		}
		return __rdtsc() - begin;
	}


	static std::size_t performanceUniqueAstCast()
	{
		std::size_t begin = __rdtsc();
		for (std::size_t i = 0; i < s_performaceLoopCounter; i++)
		{
			std::unique_ptr<AstElement> ptr = AstBuildSystem::Instance().getFactory().scopeFactor().createScope("test", nullptr);
			AstScope* casted = ast_unique_element_cast<AstScope>(ptr);
		}
		return __rdtsc() - begin;
	}

};


TEST(AST_CAST, ScopeToScope)
{
	EXPECT_TRUE(AstCastTest::castTestScopeToScope());
}

TEST(AST_CAST, VariableToVariable)
{
	EXPECT_TRUE(AstCastTest::castTestVariableToVariable());
}

TEST(AST_CAST, ScopeToVariable)
{
	EXPECT_FALSE(AstCastTest::castTestScopeToVariable());
}

TEST(AST_CAST, VariableToScope)
{
	EXPECT_FALSE(AstCastTest::castTestVariableToScope());
}

TEST(AST_CAST_PERFORMANCE, DYNAMIC_VS_AST_CAST)
{
	EXPECT_TRUE(AstCastTest::astCastFasterThanDynamic());
}

TEST(AST_CAST_PERFORMANCE, DYNAMIC_VS_AST_UNIQUE_CAST)
{
	EXPECT_TRUE(AstCastTest::uniqueAstCastFasterThanDynamic());
}


TEST(AST_CAST_UNIQUE, UniqueVariableToVariable)
{
	EXPECT_TRUE(AstCastTest::uniqueCastTestVariableToVariable());
}

TEST(AST_CAST_UNIQUE, UniqueScopeToScope)
{
	EXPECT_TRUE(AstCastTest::uniqueCastTestScopeToScope());
}

TEST(AST_CAST_UNIQUE, UniqueScopeToVariable)
{
	EXPECT_FALSE(AstCastTest::uniqueCastTestScopeToVariable());
}

TEST(AST_CAST_UNIQUE, UniqueVariableToScope)
{
	EXPECT_FALSE(AstCastTest::uniqueCastTestVariableToScope());
}
