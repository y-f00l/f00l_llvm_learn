#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

//implement the class about local optimization
//include : Algebraic identifiers  Constant fold  Strength reductions
class LocalOpt : public FunctionPass {
public:
    static char ID;

    LocalOpt() : FunctionPass(ID),
                 algebraic_identies_times(0),
                 constant_fold_times(0),
                 strength_reduction_times(0)
    { }
    ~LocalOpt() {}

    virtual void getAnalysisUsage(AnalysisUsage &AU) const override {
        AU.setPreservesAll();
    }

    //do some initialization
    virtual bool doInitialization(Module &M) override  {
        errs() << "CMU 15-745 Local Optimization Pass\n";
        return false;
    }

    virtual bool runOnFunction(Function &F) override {
        errs() << "function: " << F.getName();
        algebraic_identies(F);
        constant_fold(F);
        strength_reduction(F);
        return false;
    }

    virtual bool doFinalization(Module &M) override {
        print_informations();
        return false;
    }
private:
    int algebraic_identies_times;
    int constant_fold_times;
    int strength_reduction_times;

    void algebraic_identies(Function &F);
    void constant_fold(Function &F);
    void strength_reduction(Function &F);
    void print_informations();
};

void LocalOpt::algebraic_identies(Function &F) {
    std::vector<Instruction*>deadInst;
    for(Function::iterator BB = F.begin();BB != F.end(); ++BB) {
        for(BasicBlock::iterator Inst = BB->begin(); Inst != BB->end(); ++Inst) {

            if(BinaryOperator* BinaryInst = dyn_cast<BinaryOperator>(&(*Inst))) {
                Value *operand0 = BinaryInst->getOperand(0);
                Value *operand1 = BinaryInst->getOperand(1);
                ConstantInt *leftvalue = nullptr;
                ConstantInt *rightvalue = nullptr;
                //operand0 and operand1 can't both be constant
                if(!isa<ConstantInt>(operand0) || !isa<ConstantInt>(operand1)) {
                    if(isa<ConstantInt>(operand0)) {
                        leftvalue = dyn_cast<ConstantInt>(operand0);
                    }
                    if(isa<ConstantInt>(operand1)) {
                        rightvalue = dyn_cast<ConstantInt>(operand1);
                    }
                    bool flag = true;
                    switch (BinaryInst->getOpcode()) {
                        case Instruction::Add:
                            //0 + x
                            if (leftvalue && leftvalue->isZero()) {
                                BinaryInst->replaceAllUsesWith(operand1);
                            }
                            //x + 0
                            else if (rightvalue && rightvalue->isZero()) {
                                BinaryInst->replaceAllUsesWith(operand0);
                            }
                            //can't algebraic identifies
                            else {
                                flag = false;
                            }
                            break;
                        case Instruction::Sub:
                            //x - 0
                            if (rightvalue && rightvalue->isZero()) {
                                BinaryInst->replaceAllUsesWith(operand0);
                            }
                            //x - x
                            else if (operand0 == operand1) {
                                BinaryInst->replaceAllUsesWith(
                                        ConstantInt::getSigned(BinaryInst->getType(),0));
                            }
                            //can't algebraic identifies
                            else {
                                flag = false;
                            }
                            break;
                        case Instruction::Mul:
                            //0 * x
                            if(leftvalue && leftvalue->isZero()){
                                BinaryInst->replaceAllUsesWith(
                                        ConstantInt::getSigned(BinaryInst->getType(), 0));
                            }
                            //x * 0
                            else if(rightvalue && rightvalue->isZero()) {
                                BinaryInst->replaceAllUsesWith(
                                        ConstantInt::getSigned(BinaryInst->getType(),0));
                            }
                            //1 * x
                            else if(leftvalue && leftvalue->isOne()) {
                                BinaryInst->replaceAllUsesWith(operand1);
                            }
                            //x * 1
                            else if(rightvalue && rightvalue->isOne()) {
                                BinaryInst->replaceAllUsesWith(operand0);
                            }
                            else {
                                flag = false;
                            }
                            break;
                        case Instruction::SDiv:
                            // 0 / x
                            if(leftvalue && leftvalue->isZero()) {
                                BinaryInst->replaceAllUsesWith(
                                        ConstantInt::getSigned(BinaryInst->getType(),0));
                            }
                            // x / 0 -> false
                            else if(rightvalue && rightvalue->isZero()) {
                                flag = false;
                            }
                            // x / 1
                            else if(rightvalue && rightvalue->isOne()) {
                                BinaryInst->replaceAllUsesWith(operand0);
                            }
                            // x / x
                            else if(operand0 == operand1) {
                                BinaryInst->replaceAllUsesWith(
                                        ConstantInt::getSigned(BinaryInst->getType(),1));
                            }
                            else {
                                flag = false;
                            }
                            break;
                        default:
                            flag = false;
                            break;
                    }
                    if(flag) {
                        deadInst.push_back(BinaryInst);
                        ++algebraic_identies_times;
                    }
                }
            }
        }
        for(auto &i:deadInst) {
            i->eraseFromParent();
        }
    }
}

//constant fold:
//eg: constA + constB -> const (const = constA + constB)
void LocalOpt::constant_fold(Function &F) {
    std::vector<Instruction*>deadInst;
    for(Function::iterator BB = F.begin(); BB != F.end(); ++BB) {
        for(BasicBlock::iterator Inst = BB->begin(); Inst != BB->end(); ++Inst) {

            if(BinaryOperator* BinaryInst = dyn_cast<BinaryOperator>(&(*Inst))) {
                long const_valA, const_valB;
                Value *operand0 = BinaryInst->getOperand(0);
                Value *operand1 = BinaryInst->getOperand(1);

                if(isa<ConstantInt>(operand0)) {
                    const_valA = dyn_cast<ConstantInt>(operand0)->getSExtValue();
                }
                if(isa<ConstantInt>(operand1)) {
                    const_valB = dyn_cast<ConstantInt>(operand1)->getSExtValue();
                }
                if(isa<ConstantInt>(operand0) && isa<ConstantInt>(operand1)) {
                    bool flag = true;
                    switch (BinaryInst->getOpcode()) {
                        case Instruction::Add:
                            BinaryInst->replaceAllUsesWith(
                                    ConstantInt::getSigned(Inst->getType(),
                                                    const_valA + const_valB));
                            break;
                        case Instruction::Sub:
                            BinaryInst->replaceAllUsesWith(
                                    ConstantInt::getSigned(Inst->getType(),
                                                    const_valA - const_valB));
                            break;
                        case Instruction::Mul:
                            BinaryInst->replaceAllUsesWith(
                                    ConstantInt::getSigned(Inst->getType(),
                                                    const_valA * const_valB));
                            break;
                        case Instruction::SDiv:
                            if(const_valB != 0) {
                                BinaryInst->replaceAllUsesWith(
                                        ConstantInt::getSigned(Inst->getType(),
                                                        const_valA / const_valB));
                            }
                            else {
                                flag = false;
                            }
                            break;
                        default:
                            flag = false;
                            break;
                    }
                    if(flag) {
                        deadInst.push_back(BinaryInst);
                        ++constant_fold_times;
                    }
                }
            }
        }
        for(auto &i:deadInst){
            i->eraseFromParent();
        }
    }
}

//strength reduction
//eg: 2 * x -> x + x
//    2 * x -> x << 1
//    x / 2 -> x * 0.5
//    x / 2 -> x >> 1
void LocalOpt::strength_reduction(Function &F) {
    std::vector<Instruction*>deadInsts;
    for(Function::iterator BB = F.begin(); BB != F.end(); ++BB) {
        for(BasicBlock::iterator Inst = BB->begin(); Inst != BB->end(); ++Inst) {

            if(BinaryOperator *BinaryInst = dyn_cast<BinaryOperator>(&(*(Inst)))) {
                Value *operand0 = BinaryInst->getOperand(0);
                Value *operand1 = BinaryInst->getOperand(1);
                ConstantInt *leftvalue = nullptr;
                ConstantInt *rightvalue = nullptr;
                if(!isa<ConstantInt>(operand0) || !isa<ConstantInt>(operand1)) {
                    if(isa<ConstantInt>(operand0)) {
                        leftvalue = dyn_cast<ConstantInt>(operand0);
                    }
                    if(isa<ConstantInt>(operand1)) {
                        rightvalue = dyn_cast<ConstantInt>(operand1);
                    }
                    bool flag = true;
                    switch (BinaryInst->getOpcode()) {
                        //ignore the sub and add
                        case Instruction::Mul:
                            //2^n * x -> x << n
                            if(leftvalue && leftvalue->getValue().isPowerOf2()) {
                                const int64_t const_value = leftvalue->getSExtValue();
                                Value *shift_value = ConstantInt::get(leftvalue->getType(),log2(const_value));
                                BinaryInst->replaceAllUsesWith(
                                        BinaryOperator::Create(Instruction::Shl,
                                            operand1,shift_value,"shl",BinaryInst));
                            }
                            //x * 2^n -> x << n
                            else if(rightvalue && rightvalue->getValue().isPowerOf2()) {
                                const int64_t const_value = rightvalue->getSExtValue();
                                Value *shift_value = ConstantInt::get(rightvalue->getType(),log2(const_value));
                                BinaryInst->replaceAllUsesWith(
                                        BinaryOperator::Create(Instruction::Shl,
                                                operand0,shift_value,"shl",BinaryInst));
                            }
                            else {
                                flag = false;
                            }
                            break;
                        case Instruction::SDiv:
                            //x / 2^n -> x >> n
                            if(rightvalue && rightvalue->getValue().isPowerOf2()) {
                                const int64_t const_value = rightvalue->getSExtValue();
                                Value *shift_value = ConstantInt::get(rightvalue->getType(),log2(const_value));
                                BinaryInst->replaceAllUsesWith(
                                        BinaryOperator::Create(Instruction::LShr,
                                                               operand0,shift_value,"lshr",BinaryInst));
                            }
                            else {
                                flag = false;
                            }
                            break;
                        default:
                            flag = false;
                            break;
                    }
                    if(flag){
                        deadInsts.push_back(BinaryInst);
                        ++strength_reduction_times;
                    }
                }
            }
        }
        for(auto &i:deadInsts) {
            i->eraseFromParent();
        }
    }
}

void LocalOpt::print_informations() {
    errs() << "Transformations applied: \n";
    errs() << "Algebraic identities: " << algebraic_identies_times << "\t\n";
    errs() << "Constant fold: " << constant_fold_times << "\t\n";
    errs() << "Strength reduction: " << strength_reduction_times << "\t\n";

}

char LocalOpt::ID = 0;
RegisterPass<LocalOpt> X ("local optimization",
                            "CMU 15-745:local optimization")   ;

}