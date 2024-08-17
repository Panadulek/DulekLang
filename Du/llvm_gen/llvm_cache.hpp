#pragma once
#include <unordered_map>
#include <string_view>
#include <llvm/IR/Instruction.h>
#include "../ast/AstElement.hpp"
#include <format>
#include "../ast/AstCast.hpp"
#include "../allocators/allocator.hpp"
#include <cassert>

template<typename Allocator = std::allocator<std::pair<const std::string, llvm::Value*>>>
class LlvmCache
{
	using llvmValMap = std::unordered_map<std::string, llvm::Value*, 
		std::hash<std::string>, std::equal_to<std::string>, Allocator>;
	
	llvmValMap m_val;
	std::string_view gen_ast_type_hash(AstElement::ElementType astType)
	{
		switch (astType)
		{
		case AstElement::ElementType::CONST_:
			return "const";
		case AstElement::ElementType::REFERENCE:
			return "ref";
		case AstElement::ElementType::DECLARATION_VARIABLE:
			return "var";
		case AstElement::ElementType::SCOPE:
			return "scope";
		}
	}

	std::string concatenation(std::string_view name, std::string_view ast_type_name, std::string_view parent_scope_name, std::string_view parent_scope_ast)
	{
		return std::format("_{}_{}_{}_{}_", parent_scope_ast, parent_scope_name, ast_type_name, name);
	}

public:
	std::string hashElement(AstElement* element)
	{
		std::string_view ast_type_hash = gen_ast_type_hash(element->getAstType());
		std::string_view ast_scope_name = element->getParent() ? element->getParent()->getName() : "null";
		std::string_view ast_scope_hash = element->getParent() ? gen_ast_type_hash(element->getParent()->getAstType()) : "null";
		return concatenation(element->getName(), ast_type_hash, ast_scope_name, ast_scope_hash);
	}

	void insertElement(llvm::Value* val, AstElement* element)
	{
		std::string hash = hashElement(element);
		llvm::Value* found = getValue(element);
		if (!found)
			m_val.insert({ hash, val });
	
	}

	llvm::Value* getValue(AstElement* element)
	{
		if (AstRef* ref = ast_element_cast<AstRef>(element))
		{
			return getValue(ref->ref());
		}
		else
		{
			std::string hash = hashElement(element);
			auto ret = m_val.find(hash);
			return (ret == m_val.end()) ? nullptr : ret->second;
		}
	}
};

template<typename Allocator = std::allocator<std::pair<const std::string, llvm::Value*>>>
LlvmCache<Allocator>& getLlvmCache()
{
	static LlvmCache<Allocator> ret;
	return ret;
}