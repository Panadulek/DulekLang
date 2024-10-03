#pragma once
#include <string_view>
#include <vector>
#include <memory>
class AstExpr;
namespace ArrayDecorator
{
	class Dimension
	{
		std::unique_ptr<AstExpr> m_dimension;
		bool m_isDynamic;
	public:
		Dimension(AstExpr* expr);
		AstExpr* getExpr();
		AstExpr* releaseExpr();
	};


	using Array = std::vector<std::unique_ptr<Dimension>>;
};