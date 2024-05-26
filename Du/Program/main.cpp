// DulekLang.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "Settings.h"
#include <string_view>
#include "Parser.hpp"
#include "../ast/AstScope.hpp"
int main(int argc, char** argv)
{
	DuSettings settings;
	AstScope scope("test", nullptr);
	settings.AnaylzeArgs(argc, argv);
	Parser parser(settings.getNextFile());
	parser.parse();
	std::cin.get();
	return 0;
}


