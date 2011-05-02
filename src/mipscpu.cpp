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
#include "mipscpu.h"

#include <cstring>
#include <iostream>


namespace tememu 
{
#ifndef TRACE_OPCODES
    #define REG_OP_FUNC(fn,code) this->_fnMap[(code)] = (&tememu::MipsCPU::fn)
#else
    #define REG_OP_FUNC(fn,code) this->_fnMap[(code)] = (&tememu::MipsCPU::fn); this->_opNameMap[(code)] = #fn
#endif

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
        // 0 opcode instructions internal opcodes = funct << 4.

        REG_OP_FUNC(op_add,     0x20 << 4);
        REG_OP_FUNC(op_addu,    0x21 << 4);
        REG_OP_FUNC(op_sub,     0x22 << 4);
        REG_OP_FUNC(op_subu,    0x23 << 4);
        REG_OP_FUNC(op_addi,    0x08);
        REG_OP_FUNC(op_addiu,   0x09);
        REG_OP_FUNC(op_mult,    0x18 << 4);
        REG_OP_FUNC(op_div,     0x1A << 4);
        REG_OP_FUNC(op_divu,    0x1B << 4);

        REG_OP_FUNC(op_beq,     0x04);
        REG_OP_FUNC(op_bne,     0x05);
        REG_OP_FUNC(op_j,       0x02);
        REG_OP_FUNC(op_jal,     0x03);
        REG_OP_FUNC(op_jr,      0x08 << 4);

        REG_OP_FUNC(op_mfhi,    0x10 << 4);
        REG_OP_FUNC(op_mthi,    0x11 << 4);
        REG_OP_FUNC(op_mflo,    0x12 << 4);
        REG_OP_FUNC(op_mtlo,    0x13 << 4);
    }

    void MipsCPU::reset()
    {
        for ( int i = 0; i < gpr_count; ++i ) _GPR[i] = 0;
        for ( int i = 0; i < fpr_count; ++i ) _FPR[i] = 0;
        for ( int i = 0; i < fcr_count; ++i ) _FCR[i] = 0;
        _HI = _LO = _FCSR = 0;
        _nPC = _PC = 4;
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
        std::cout << _opNameMap[internal_opcode] << "\n";
#endif

        boost::unordered_map<int32, OpcodeFn>::iterator it = _fnMap.find(internal_opcode);

        if (it != _fnMap.end())
        {
            CALL_MEMBER(this, it->second)(instr);
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
        _GPR[RD(instr)] = _GPR[RS(instr)] + _GPR[RT(instr)];
        step();
    }

    void MipsCPU::op_addu(int32 instr)
    {
        _GPR[RD(instr)] = _GPR[RS(instr)] + _GPR[RT(instr)];
        step();
    }

    void MipsCPU::op_addi(int32 instr)
    {
        int_short conv;
        conv.i = instr;

        _GPR[RT(instr)] = _GPR[RS(instr)] + conv.s;
        step();
    }

    void MipsCPU::op_addiu(int32 instr)
    {
        int_short conv;
        conv.i = instr;

        _GPR[RT(instr)] = _GPR[RS(instr)] + conv.s;
        step();
    }

    void MipsCPU::op_sub(int32 instr)
    {
        _GPR[RD(instr)] = _GPR[RS(instr)] - _GPR[RT(instr)];
        step();
    }

    void MipsCPU::op_subu(int32 instr)
    {
        _GPR[RD(instr)] = (boost::uint32_t)(_GPR[RS(instr)]) - (boost::uint32_t)(_GPR[RT(instr)]);
        step();
    }

    void MipsCPU::op_mult(int32 instr)
    {
        _LO = ((_GPR[RT(instr)] * _GPR[RS(instr)]) << 16) >> 16;
        _HI = (_GPR[RT(instr)] * _GPR[RS(instr)]) << 16;
        step();
    }

    void MipsCPU::op_div(int32 instr)
    {
        _LO = _GPR[RS(instr)] / _GPR[RT(instr)];
        _HI = _GPR[RS(instr)] % _GPR[RT(instr)];
        step();
    }

    void MipsCPU::op_divu(int32 instr)
    {
        _LO = _GPR[RS(instr)] / _GPR[RT(instr)];
        _HI = _GPR[RS(instr)] % _GPR[RT(instr)];
        step();
    }

    void MipsCPU::op_beq(int32 instr)
    {
        if (_GPR[RS(instr)] == _GPR[RT(instr)])
        {
            advance_pc(4 + 4 * IMMEDIATE(instr));
        }
        else
        {
            step();
        }
    }

    void MipsCPU::op_bne(int32 instr)
    {
        int_short conv;
        conv.i = instr;

        if (_GPR[RS(instr)] != _GPR[RT(instr)])
        {
            advance_pc(4 + conv.s * 4);
        }
        else
        {
            step();
        }
    }

    void MipsCPU::op_j(int32 instr)
    {
        _PC = _nPC;
        _nPC = (_PC & 0xf0000000) | ADDRESS(instr) + 4;
    }

    void MipsCPU::op_jal(int32 instr)
    {
        _GPR[31] = _PC + 4; // return address is the next instruction from here
        _PC = _nPC;
        _nPC = (_PC & 0xf0000000) | ADDRESS(instr) + 4;
    }

    void MipsCPU::op_jr(int32 instr)
    {
        _PC = _nPC;
        _nPC = _GPR[RS(instr)] + 4;
    }

    void MipsCPU::op_mfhi(int32 instr)
    {
        _GPR[RD(instr)] = _HI;
        step();
    }

    void MipsCPU::op_mflo(int32 instr)
    {
        _GPR[RD(instr)] = _LO;
        step();
    }

    void MipsCPU::op_mthi(int32 instr)
    {
        _HI = _GPR[RS(instr)];
        step();
    }

    void MipsCPU::op_mtlo(int32 instr)
    {
        _LO = _GPR[RS(instr)];
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

    void MipsCPU::stepProgram(int steps)
    {
        std::vector<int32>* p = _program.get();

        for (int i = 0; i < steps; ++i)
        {
            runDecodedInstr(p->at(_nPC / 4 - 1));
            if (_nPC / 4 - 1 >= p->size()) break;
        }
    }

} // tememu
