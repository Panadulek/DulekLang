#include "AstScope.hpp"
#include "algorithm"
#include <span>
#include <ranges>
#include <cassert>
#include <map>
#include <cstdint>
#include <stack>
#include "BasicType.hpp"
#include "AstBuildSystem.hpp"
#include <string_view>


/*
 * Global---
 */
class AstScope::Global
{
private:
	std::string m_currentFile;
	std::map<std::string_view, std::unique_ptr<AstScope>> m_connectFileWithGlobalScope;
	std::map<std::string_view, BasicTypes> m_connectBasicTypesWithId;
	void updateCurrentFile(std::string_view name)
	{
		m_currentFile = name;
	}
	std::string_view getCurrentFilename()
	{
		return m_currentFile;
	}

	AstScope* getCurrentGlobalScope()
	{
		return getGlobalScopeForFile(getCurrentFilename());
	}

	Global()
	{
		m_connectBasicTypesWithId.insert({ "i8", BasicTypes::I8 });
		m_connectBasicTypesWithId.insert({ "i16", BasicTypes::I16 });
		m_connectBasicTypesWithId.insert({ "i32", BasicTypes::I32 });
		m_connectBasicTypesWithId.insert({ "i64", BasicTypes::I64 });
		m_connectBasicTypesWithId.insert({ "u8", BasicTypes::U8 });
		m_connectBasicTypesWithId.insert({ "u16", BasicTypes::U16 });
		m_connectBasicTypesWithId.insert({ "u32", BasicTypes::U32 });
		m_connectBasicTypesWithId.insert({ "u64", BasicTypes::U64 });
		m_connectBasicTypesWithId.insert({ "f32", BasicTypes::F32 });
		m_connectBasicTypesWithId.insert({ "f64", BasicTypes::F64 });
	}
	bool addFile(std::string_view filename)
	{
		auto iterator = m_connectFileWithGlobalScope.find(filename);
		if (iterator == m_connectFileWithGlobalScope.end())
		{
			m_connectFileWithGlobalScope.insert({ filename, AstBuildSystem::Instance().getFactory().scopeFactor().createScope("", nullptr)});
			m_currentFile = filename;
			AstBuildSystem::Instance().provideNextFilename(m_currentFile);
			AstBuildSystem::Instance().getBuilder().beginScope(m_connectFileWithGlobalScope.find(filename)->second.get());
			return true;
		}
		return false;
	}

	AstScope* getGlobalScopeForFile(std::string_view filename)
	{
		auto iterator = m_connectFileWithGlobalScope.find(filename);
		if (iterator != m_connectFileWithGlobalScope.end())
		{
			return iterator->second.get();
		}
		return nullptr;
	}
	std::pair<bool, BasicTypes> isBasicType(std::string_view tname)
	{
		auto it = m_connectBasicTypesWithId.find(tname);
		if (it != m_connectBasicTypesWithId.end())
		{
			return { true, it->second };
		}
		return { false, BasicTypes::VOID_TYPE };
	}
	void clearFiles()
	{
		m_connectFileWithGlobalScope.clear();
	}
	friend struct AstScope::GlobalApi;
	friend class std::unique_ptr<Global> std::make_unique<Global>();
};
/*
 * ---GLOBAL
 */


/*
 * AST_SCOPE---
 */

AstScope::AstScope(std::string_view name, AstElement* parent) : AstElement(name, AstElement::ElementType::SCOPE, parent), m_function(nullptr)
{}
auto AstScope::getFilteredViewByNotKeywordName()
{
	
	return filterView(m_elements, [](const std::unique_ptr<AstElement>& element) {return !element->nameIsKeyword(); });
}



AstElement* AstScope::addElement(std::unique_ptr<AstElement>&& element)
{
	auto filteredView = getFilteredViewByNotKeywordName();
	if (isEmptyView(filteredView))
	{
		if (isNewElementUniqueByName(filteredView, element))
		{
			m_elements.emplace_back(std::move(element));
		}
		else
		{
			assert(false && "Name was declared");
			return nullptr;
		}
	}
	else
		m_elements.emplace_back(std::move(element));
	return m_elements.back().get();
}

std::span<std::unique_ptr<AstElement>> AstScope::getElements()
{
	return m_elements;
}

AstScope::Global& AstScope::getGlobal()
{
	static std::unique_ptr<Global> impl = std::make_unique<Global>();
	return *impl;
}


AstScope::~AstScope() = default;


/*
 * ---AST_SCOPE
 */


 /*
  * GLOBAL_API---
  */

std::pair<bool, BasicTypes> AstScope::GlobalApi::isBasicType(std::string_view tname)
{
	return AstScope::getGlobal().isBasicType(tname);
}

bool AstScope::GlobalApi::addFile(std::string_view filename)
{
	return AstScope::getGlobal().addFile(filename);
}
AstScope* AstScope::GlobalApi::getCurrentGlobalScope()
{
	return AstScope::getGlobal().getCurrentGlobalScope();
}
AstScope* AstScope::GlobalApi::getGlobalScopeForFile(std::string_view filename)
{
	return AstScope::getGlobal().getGlobalScopeForFile(filename);
}

void AstScope::GlobalApi::clearGlobalScopes()
{
	AstScope::getGlobal().clearFiles();
}

/*
 * ---GLOBAL_API
 */




