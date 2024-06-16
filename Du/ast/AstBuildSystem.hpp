
#pragma once
#include "AstBuilder.hpp"
#include "AstFactor.hpp"
#include <string_view>
class AstBuildSystem
{
	std::unique_ptr<AstBuilder> m_builder;
	AstFactory m_factory;
	AstBuildSystem() : m_builder(nullptr)
	{}
public:

	void provideNextFilename(std::string_view filename) { m_builder.reset(new AstBuilder(filename)); }
	AstFactory& getFactory() { return m_factory; }
	AstBuilder& getBuilder() { return *m_builder; }
	static AstBuildSystem& Instance()
	{
		static AstBuildSystem _instance;
		return _instance;
	}

};