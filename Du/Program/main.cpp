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
int main(int argc, char** argv)
{
	DuSettings settings;
	std::cout << std::filesystem::current_path().string() << std::endl;
	settings.AnaylzeArgs(argc, argv);
	Parser parser(settings.getNextFile());
	parser.parse();
	std::cin.get();
	return 0;
}


