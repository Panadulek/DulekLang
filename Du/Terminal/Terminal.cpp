#include "Terminal.hpp"

Terminal::Terminal() : m_isErrorAppeared(false),
	m_msgList{
		std::make_pair("DU001", "Missing declaration"),
		std::make_pair("DU002", "Redeclaration in scope"),
	}

{

}

Terminal* Terminal::s_terminal = nullptr;