#pragma once
#include <string_view>
#include <vector>
#include <memory>
namespace VariableDecorator
{
	class Dimension
	{
		std::unique_ptr<AstExpr> m_dimension;
		bool m_isDynamic;
	public:
		Dimension(AstExpr* expr) : m_dimension(expr)
		{
			switch (expr->op())
			{
			case AstExpr::Operation::ConstValue:
				m_isDynamic = false;
				break;
			default:
				m_isDynamic = true;
				break;
			}
		}
		AstExpr* getExpr()
		{
			return m_dimension.get();
		}
	};


	using Array = std::vector<std::unique_ptr<Dimension>>;
};