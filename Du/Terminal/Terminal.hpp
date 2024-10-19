#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <format>
#include <array>
struct Terminal
{
	virtual ~Terminal() = default;
	enum class MessageType
	{
		_ERROR,
		WARNING,
		LOG,
	};
	enum CodeList : uint64_t
	{
		DU001,
		DU002,
		LAST_ERR
	};
	virtual void print(MessageType, CodeList code, std::string_view opt_message) = 0;

	static void initOutput(Terminal* term) 
	{
		if (!s_terminal)
			s_terminal = term;
	}

	static Terminal* Output()
	{
		return s_terminal;
	}
	bool errorHappend() { return m_isErrorAppeared; }
protected:
	Terminal();
	const std::string format(MessageType type, std::string_view code,  std::string_view message, std::string_view opt_message)
	{
		if(opt_message.empty())
			return std::format("{} {}: {}\n", MessageTypeToStr(type), code, message);
		return std::format("{} {}: {}, {}\n", MessageTypeToStr(type), code, message, opt_message);
	}
	std::string_view MessageTypeToStr(MessageType type)
	{
		switch (type)
		{
		case MessageType::_ERROR:
			m_isErrorAppeared = true;
			return "Error";
		case MessageType::WARNING:
			return "Warning";
		case MessageType::LOG:
			return "Log";
		}
	}
	std::array< std::pair<const char*, const char*>, LAST_ERR> m_msgList;
	bool m_isErrorAppeared;
private:
	
	static Terminal* s_terminal;
};



class StandardTerminal : public Terminal
{
	template<MessageType TYPE>
	void _print(std::string_view message);
public:
	virtual void print(MessageType type, CodeList code, std::string_view opt_message) override;
};