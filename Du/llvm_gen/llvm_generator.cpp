
#include <llvm/IR/LLVMContext.h>
#include "llvm_generator.h"
llvm::LLVMContext& getContext()
{
	static llvm::LLVMContext context;
	return context;
}