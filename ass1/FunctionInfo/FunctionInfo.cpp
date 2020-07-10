#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;


namespace {

class FunctionInfo final : public ModulePass
{
public:
	static char ID;

	FunctionInfo() : ModulePass(ID) {}
	virtual ~FunctionInfo() override {}

  	// We don't modify the program, so we preserve all analysis.
	virtual void getAnalysisUsage(AnalysisUsage & AU) const override
	{
		AU.setPreservesAll();
	}

	virtual bool runOnModule(Module & M) override
	{
        outs() << "CSCD70 Functions Information Pass" << "\n";
        outs() << "------------------------------------------\n";
        outs() << "Name\t " ;
        outs() << "#Args\t" ;
        outs() << "#Calls\t" ;
        outs() << "#Blocks\t" ;
        outs() << "#Insts\t\n";
        outs() << "------------------------------------------\n";

        Module::FunctionListType &func_list = M.getFunctionList();

        for(Module::FunctionListType::iterator func = func_list.begin();
            func != func_list.end(); ++func) {
            outs() << func->getName() << "\t";
            outs() << func->arg_size() << "\t";
            outs() << func->getNumUses() << "\t";

            Function::BasicBlockListType &block_list = func->getBasicBlockList();

            outs() << block_list.size() << "\t";
            int insts = 0;
            for(Function::BasicBlockListType::iterator block = block_list.begin();
                block != block_list.end(); ++block) {
                insts += block->getInstList().size();
            }
            outs() << insts << "\n";
        }
		return false;
	}
};

char FunctionInfo::ID = 0;
RegisterPass < FunctionInfo > X (
	"function-info",
	"CSCD70: Functions Information");

}  // namespace anonymous
