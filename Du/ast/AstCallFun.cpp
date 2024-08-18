#include "AstCallFun.hpp"

bool AstCallFun::resolve() 
{
	std::size_t argsCounter = getArgsSize();
	std::size_t parameterCounter = m_fun->getArgsCounter();
	if (argsCounter == parameterCounter)
	{
		if (argsCounter > 0)
		{
			m_args->goToBegin();
			auto parameters = m_fun->getArgs();
			for (std::size_t i = 0; i < parameterCounter; i++)
			{
				AstElement* arg = m_args->getFront();
				AstElement* parameter = parameters[i];
				//TODO Type compare
			}
		}
		return true;
	}
	return false;
}