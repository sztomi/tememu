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
    MipsCPU::MipsCPU()
        : _HI(0), _LO(0), _PC(0), _nPC(0), _FCSR(0)
    {
        _GPR.reserve(gpr_count); 
        _FPR.reserve(fpr_count);
        _FCR.reserve(fcr_count);

        for ( int i = 0; i < gpr_count; ++i ) _GPR.push_back(0);
        for ( int i = 0; i < fpr_count; ++i ) _FPR.push_back(0);
        for ( int i = 0; i < fcr_count; ++i ) _FCR.push_back(0);

        // set up instruction table
        // see runDecodedInstr for the construction of the constants

        _fnMap[0x00000020] = &tememu::MipsCPU::op_add;
    }

    /**
     * @brief Extracts the internal opcode for the function and dispatches the call.
     *
     * @param instr The raw instruction.
     */
    void MipsCPU::runDecodedInstr(int32 instr)
    {
        int32 opcode = (instr & 0x03fffffff) >> 20;
        int32 func = instr & 0x0000003F;
        int32 internal_opcode = opcode | func;

        CALL_MEMBER(this, _fnMap[internal_opcode])(instr);
    }

    void MipsCPU::advance_pc(int32 offset)
    {
        _PC = _nPC;
        _nPC += offset;
    }

    void MipsCPU::op_add(int32 instr)
    {

    }
} // tememu
