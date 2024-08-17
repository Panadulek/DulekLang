// DulekLang.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "Settings.h"
#include <string_view>
#include "Parser.hpp"
#include "../ast/AstScope.hpp"
#include "../ast/AstFactor.hpp"
#include "../ast/AstBuilder.hpp"
#include <filesystem>
#include <iostream>
#include "../ast/AstConst.hpp"

#include "../ast/AstList.hpp"
#include "../ast/AstCallFun.hpp"

#include "../llvm_gen/llvm_generator.h"
#include "../llvm_gen/llvm_output_process.h"


#define NOMINMAX


int main(int argc, char** argv)
{
	DuSettings settings;
	std::cout << std::filesystem::current_path().string() << std::endl;
	settings.AnaylzeArgs(argc, argv);
	std::string_view filename = settings.getNextFile();
	Parser parser(filename, settings.is_yyDebug());
	parser.parse();
	LlvmGen gen(filename.data());
	gen.generateIr();
	std::unique_ptr<llvmOutputProcess> m_outputProcess = std::make_unique<llvmIrOutputProcess>(gen, "main");
	return 0;
}


