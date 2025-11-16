#include "AstExpr.hpp"
#include "AstVariableDecl.hpp"
std::unique_ptr<AstExpr::IndexingArray> AstExpr::createArrayFromIndexingOp()
{
	std::unique_ptr<AstExpr::IndexingArray> arr = std::make_unique<IndexingArray>();
	AstExpr* Iterator = this;
	while (Iterator && Iterator->op() == Operation::Arr_Indexing)
	{
		if (AstVariableDecl* decl = ast_element_cast<AstVariableDecl>(Iterator->left()))
		{
			Iterator = ast_element_cast<AstExpr>(Iterator->right());
		}
		else if (AstExpr* expr = ast_element_cast<AstExpr>(Iterator->left()))
		{
			arr->emplace_back(std::make_unique<ArrayDecorator::Dimension>(expr, false));
			Iterator = ast_element_cast<AstExpr>(Iterator->right());
		}
	}
	return arr;
}