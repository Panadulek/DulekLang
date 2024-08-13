#pragma once
#include <llvm/IR/Module.h>
#include <memory>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include "llvm_generator.h"
#include "llvm/ExecutionEngine/GenericValue.h"
class llvmOutputProcess
{
protected:
	std::unique_ptr<llvm::Module> m_module;
	llvm::Function* m_start;
public:
	llvmOutputProcess(std::unique_ptr<llvm::Module> m, std::string_view startFunctionName) : m_module(std::move(m))
	{
		m_start = m_module->getFunction(startFunctionName);
	}
	virtual void process() = 0;
	void genFile(std::string_view m_pathToOutputFile)
	{}
	virtual ~llvmOutputProcess() = default;
};


class llvmAsmOutputProcess : public llvmOutputProcess
{
public:
	virtual ~llvmAsmOutputProcess() = default;
};


class llvmIrOutputProcess : public llvmOutputProcess
{
	std::unique_ptr<llvm::ExecutionEngine> m_engine;
	std::string m_errs;

public:

	llvmIrOutputProcess(LlvmGen& gen, std::string_view startFunctionName) : llvmOutputProcess(gen.releaseModule(), startFunctionName) 
	{
		m_engine.reset(llvm::EngineBuilder(std::move(m_module)).setErrorStr(&m_errs).create());
	}
	void process() override
	{
		m_engine->runFunction(m_start, std::vector<llvm::GenericValue>());
	}

	virtual ~llvmIrOutputProcess() = default;

};