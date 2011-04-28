/*
 *    The MIT License
 *    
 *    Copyright (c) 2011, Tamás Szelei
 *    
 *    Permission is hereby granted, free of charge, to any person obtaining a copy
 *    of this software and associated documentation files (the "Software"), to deal
 *    in the Software without restriction, including without limitation the rights
 *    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the Software is
 *    furnished to do so, subject to the following conditions:
 *    
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *    
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *    THE SOFTWARE.
 */
 
#include "consts.h"
#include "instruction.h"
#include "mipscpu.h"

#include <iostream>

namespace tememu 
{
    #define REG_OP_FUNC(fn,code) this->_fnMap[(code)] = (&tememu::MipsCPU::fn)

    MipsCPU::MipsCPU()
        : _HI(0), _LO(0), _PC(4), _nPC(4), _FCSR(0)
    {
        _GPR.reserve(gpr_count); 
        _FPR.reserve(fpr_count);
        _FCR.reserve(fcr_count);

        for ( int i = 0; i < gpr_count; ++i ) _GPR.push_back(0);
        for ( int i = 0; i < fpr_count; ++i ) _FPR.push_back(0);
        for ( int i = 0; i < fcr_count; ++i ) _FCR.push_back(0);

        // set up instruction table
        // see runDecodedInstr for the construction of the constants

        REG_OP_FUNC(op_add,     0x20 << 4);
        REG_OP_FUNC(op_addu,    0x21 << 4);
        REG_OP_FUNC(op_sub,     0x22 << 4);
        REG_OP_FUNC(op_subu,    0x23 << 4);
        REG_OP_FUNC(op_addi,    0x8);
        REG_OP_FUNC(op_addiu,   0x9);
        REG_OP_FUNC(op_mult,    0x18 << 4);
        REG_OP_FUNC(op_div,     0x1A << 4);
        REG_OP_FUNC(op_divu,    0x1B << 4);
    }

    /**
     * @brief Extracts the internal opcode for the function and dispatches the call.
     *
     * @param instr The raw instruction.
     */
    void MipsCPU::runDecodedInstr(int32 instr)
    {
        int32 opcode = (instr & 0x03fffffff) >> 26;
        int32 internal_opcode = 0, func = 0; 

        if (opcode == 0)
        {
            func = instr & 0x0000003F;
            internal_opcode = func << 4;
        }
        else
        {
            internal_opcode = opcode; 
        }

#if defined(DEBUG) && defined(TRACE_OPCODES)
        std::cout << "opcode = " << opcode << ", func = " << func << ", internal_opcode = " << internal_opcode << "\n";
#endif

        boost::unordered_map<int32, OpcodeFn>::iterator it = _fnMap.find(internal_opcode);

        if (it != _fnMap.end())
        {
            CALL_MEMBER(this, (*it).second)(instr);
        }
        else
        {
#ifdef DEBUG
            std::cout << "Unknown internal_opcode: " << internal_opcode << "\n"; 
#endif
            step();
        }
    }

    void MipsCPU::advance_pc(int32 offset)
    {
        _PC = _nPC;
        _nPC += offset;
    }

    void MipsCPU::op_add(int32 instr)
    {
        RInstruction i(instr);
        _GPR[i.rd] = _GPR[i.rs] + _GPR[i.rt];
        step();
    }

    void MipsCPU::op_addu(int32 instr)
    {
        RInstruction i(instr);
        _GPR[i.rd] = (boost::uint32_t)(_GPR[i.rs]) + (boost::uint32_t)(_GPR[i.rt]);
        step();
    }

    void MipsCPU::op_addi(int32 instr)
    {
        IInstruction i(instr);
        _GPR[i.rt] = _GPR[i.rs] + i.immediate;
        step();
    }

    void MipsCPU::op_addiu(int32 instr)
    {
        IInstruction i(instr);
        _GPR[i.rt] = _GPR[i.rs] + i.immediate;
        step();
    }

    void MipsCPU::op_sub(int32 instr)
    {
        RInstruction i(instr);
        _GPR[i.rd] = _GPR[i.rs] - _GPR[i.rt];
        step();
    }

    void MipsCPU::op_subu(int32 instr)
    {
        RInstruction i(instr);
        _GPR[i.rd] = (boost::uint32_t)(_GPR[i.rs]) - (boost::uint32_t)(_GPR[i.rt]);
        step();
    }

    void MipsCPU::op_mult(int32 instr)
    {
        RInstruction i(instr);
        _LO = ((_GPR[i.rt] * _GPR[i.rs]) << 16) >> 16;
        _HI = (_GPR[i.rt] * _GPR[i.rs]) << 16;
        step();
    }

    void MipsCPU::op_div(int32 instr)
    {
        RInstruction i(instr);
        _LO = _GPR[i.rs] / _GPR[i.rt];
        _HI = _GPR[i.rs] % _GPR[i.rt];
        step();
    }

    void MipsCPU::op_divu(int32 instr)
    {
        RInstruction i(instr);
        _LO = _GPR[i.rs] / _GPR[i.rt];
        _HI = _GPR[i.rs] % _GPR[i.rt];
        step();
    }

    void MipsCPU::loadProgram(boost::shared_ptr< std::vector<int32> > program)
    {
        _program = program;
    }

    void MipsCPU::runProgram()
    {
        std::vector<int32>* p = _program.get();

        while (_nPC / 4 - 1 < p->size())
            runDecodedInstr(p->at(_nPC / 4 - 1));
    }

} // tememu
