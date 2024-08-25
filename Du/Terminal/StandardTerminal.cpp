#include "Terminal.hpp"



template<>
void StandardTerminal::_print<Terminal::MessageType::_ERROR>(std::string_view message)
{
	std::cerr << message;
}

template<>
void StandardTerminal::_print<Terminal::MessageType::WARNING>(std::string_view message)
{
	std::clog << message;
}

template<>
void StandardTerminal::_print<Terminal::MessageType::LOG>(std::string_view message)
{
	std::cout << message;
}

void StandardTerminal::print(MessageType type, CodeList code, std::string_view opt_message)
{
	auto strs = m_msgList[code];
	std::string_view strCode = strs.first;
	std::string_view message = strs.second;
	switch (type)
	{
	case MessageType::_ERROR:
		_print<MessageType::_ERROR>(format(type, strCode, message, opt_message));
		break;
	case MessageType::WARNING:
		_print<MessageType::WARNING>(format(type, strCode, message, opt_message));
		break;
	case MessageType::LOG:
		_print<MessageType::LOG>(format(type, strCode, message, opt_message));
		break;
	}
}
