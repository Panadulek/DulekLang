#pragma once

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <string_view>
#include "../ast/AstScope.hpp"
#include "../ast/AstCast.hpp"
#include "llvm_scope_generator.hpp"
extern llvm::LLVMContext& getContext();
class LlvmGen
{
	std::unique_ptr<llvm::Module> m_module;
	llvm::IRBuilder<> m_builder;
	std::string m_filename;
	
public:
	LlvmGen(std::string filename) :  m_builder(getContext())
	{
		m_module = std::make_unique<llvm::Module>(filename, getContext());
		AstScope* scope = AstScope::GlobalApi::getGlobalScopeForFile(filename);
		m_filename = filename;
	}
	void generateIr()
	{
		AstScope* scope = AstScope::GlobalApi::getGlobalScopeForFile(m_filename);
		if (!scope)
			return;
		auto globalChilds = scope->getElements();
		for (auto &it : globalChilds)
		{
			if (AstScope* scope = ast_unique_element_cast<AstScope>(it))
			{
				LlvmScopeGenerator gen;
				gen.processAstScope(scope, m_builder, m_module);
			}
		}
		m_module->print(llvm::outs(), nullptr);
	}
	std::unique_ptr<llvm::Module>&& releaseModule()
	{
		return std::move(m_module);
	}

};