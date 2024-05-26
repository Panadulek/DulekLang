#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <coroutine>
#include <Windows.h>
class DuSettings
{
	std::vector<std::string_view> m_filenames;
public:
	void AnaylzeArgs(int argc, char** argv)
	{
		bool bReadFiles = false;
		if (argc == 1)
		{
			m_filenames.push_back("main.du");
			return;
		}
		for (std::size_t i = 1; i < argc;)
		{
			std::string_view opt = argv[i++];
			if (opt.compare("-f") && i < argc)
			{
				bReadFiles = true;
				continue;
			}
			if (bReadFiles)
			{
				m_filenames.push_back(argc[argv]);
				continue;
			}
		}
	}
	std::string_view getNextFile()
	{
		std::string_view& ret = m_filenames.front();
		m_filenames.erase(m_filenames.begin());
		return ret;
	}
private:
	friend class DulekLangTestClass;
};