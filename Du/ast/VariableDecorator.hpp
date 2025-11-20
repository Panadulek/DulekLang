#pragma once
#include <string_view>
#include <vector>
#include <memory>
class AstExpr;
namespace q
{
	class Dimension
	{
		std::unique_ptr<AstExpr> m_dimension;
		bool m_isDynamic;
		bool m_owner;
	public:
		Dimension(AstExpr* expr, bool owner = true);
		AstExpr* getExpr();
		AstExpr* releaseExpr();
		~Dimension();
	};


	using Array = std::vector<std::unique_ptr<Dimension>>;
};