#pragma once
#include <llvm/IR/Module.h>
#include <memory>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include "llvm_generator.h"
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/MC/MCTargetOptions.h>
#include <llvm/Pass.h>
#include <llvm/Target/CGPassBuilderOption.h>
#include <llvm/IR/LegacyPassManager.h>
class llvmOutputProcess
{
protected:
	std::unique_ptr<llvm::Module> m_module;
	llvm::Function* m_start;
	std::string m_errs;
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




class llvmIrOutputProcess : public llvmOutputProcess
{
	std::unique_ptr<llvm::ExecutionEngine> m_engine;
	

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

class llvmAsmOutputProcess : public llvmOutputProcess
{
public:
	enum class TargetPlatform
	{
		X86,
		currentPlatform,
	};


	llvmAsmOutputProcess(LlvmGen& gen, std::string_view startFunctionName, TargetPlatform tp) : m_platform(tp), llvmOutputProcess(gen.releaseModule(), startFunctionName)
	{
		llvm::InitializeAllTargetInfos();
		llvm::InitializeAllTargets();
		llvm::InitializeAllTargetMCs();
		llvm::InitializeAllAsmParsers();
		llvm::InitializeAllAsmPrinters();
	}
	void process() override
	{
		llvm::StringRef targetStr = llvm::Triple::getArchTypeName(llvm::Triple::ArchType::x86_64);
		m_target = llvm::TargetRegistry::lookupTarget(targetStr, m_errs);
		if (!m_target)
		{
			llvm::errs() << "Error: " << m_errs;
			return;
		}
		llvm::TargetOptions opt;
		llvm::TargetMachine* tm = m_target->createTargetMachine(targetStr, "generic", "", opt, llvm::Reloc::PIC_);
		m_module->setDataLayout(tm->createDataLayout());
		m_module->setTargetTriple(targetStr);


		std::string filename = "output.o";
		std::error_code ec;
		llvm::raw_fd_ostream dest(filename, ec);

		if (ec) {
			llvm::errs() << "Could not open file: " << ec.message() << "\n";
			return;
		}

		// Tworzenie pass managera
		llvm::legacy::PassManager pm;

		// Emitowanie pliku obiektowego

		if (tm->addPassesToEmitFile(pm, dest, nullptr, llvm::CodeGenFileType::AssemblyFile)) {
			llvm::errs() << "TargetMachine can't emit a file of this type!\n";
			return;

		}
		if (m_module)
		{
			pm.run(*m_module);
			llvm::outs() << "Wygenerowano plik obiektowy: " << filename << "\n";
		}
	}

	void showUpArchitecture()
	{
		for (const llvm::Target& target : llvm::TargetRegistry::targets()) {
			llvm::outs() << "Target: " << target.getName()
				<< ", Triple: " << target.getShortDescription() << "\n";
		}

	}
	virtual ~llvmAsmOutputProcess() = default;

private:

	TargetPlatform m_platform;
	const llvm::Target* m_target;

	std::string_view createTargetString()
	{
		switch (m_platform)
		{
		case TargetPlatform::X86:

		default:
			break;
		}
	}
};




