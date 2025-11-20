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
#include "../Terminal/Terminal.hpp"

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
public:
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
		m_connectBasicTypesWithId.insert({ "str", BasicTypes::STR });
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
	bool isTotalType(BasicTypes t)
	{
		switch (t)
		{
			case BasicTypes::I8:
			case BasicTypes::I16:
			case BasicTypes::I32:
			case BasicTypes::I64:
			case BasicTypes::U8:
			case BasicTypes::U16:
			case BasicTypes::U32:
			case BasicTypes::U64:
				return true;
			default:
				return false;
		}
	}
	void clearFiles()
	{
		m_connectFileWithGlobalScope.clear();
	}
	friend struct AstScope::GlobalApi;
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
	if (element)
	{
		if (AstStatement* stmt = ast_unique_element_cast<AstStatement>(element))
		{
			m_stmts.emplace_back(std::move(element));
			return m_stmts.back().get();
		}
		else
		{
			auto filtredView = getFilteredViewByNotKeywordName();
			if(isNewElementUniqueByName(filtredView, element))
			{

				m_elements.emplace_back(std::move(element));
				auto decl = ast_unique_element_cast<AstVariableDecl>(m_elements.back());
				if (decl)
					m_lookupMap.insert({ decl->getName(), decl });
				return m_elements.back().get();
			}
			else
			{
				Terminal::Output()->print(Terminal::MessageType::_ERROR, Terminal::CodeList::DU002, std::format("\"{}\" \"{}\"", getName(), element->getName()));
				return nullptr;
			}
		}
	}
}

std::span<std::unique_ptr<AstElement>> AstScope::getElements()
{
	return m_elements;
}
std::span<std::unique_ptr<AstElement>> AstScope::getStmts()
{
	return m_stmts;
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
bool AstScope::GlobalApi::isTotalType(BasicTypes type)
{
	return AstScope::getGlobal().isTotalType(type);
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




