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

#ifndef _MIPSCPU_H
#define _MIPSCPU_H

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <map>
#include <string>
#include <vector>

#define CALL_MEMBER(obj,fn) ((obj)->*(fn))

#define OPCODE(i) ((i & 0xFC000000) >> 26)    // extract bits 0..5
#define RS(i) ((i & 0x03E00000) >> 21)        // extract bits 6..10
#define RT(i) ((i & 0x001F0000) >> 16)        // extract bits 11..15
#define RD(i) ((i & 0x0000F800) >> 11)        // extract bits 16..20
#define SHAMT(i) ((i & 0x000001F0) >> 4)     // extract bits 21..25
#define FUNCT(i) (i & 0x0000003F)            // extract bits 26..31
#define IMMEDIATE(i) (i & 0x0000FFFF) // extract bits 16..31
#define ADDRESS(i) ((i & 0x03ffffff) << 2) // extract bits 6..31

namespace tememu 
{
    typedef boost::int_fast32_t int32;

    union int_short { short s; int32 i; };


    /**
     * @brief Maintains the state of the MIPS CPU
     */
    class MipsCPU 
    {
        typedef void (MipsCPU::*OpcodeFn)(int32);

    public:
        MipsCPU();
        ~MipsCPU() {}

    private:
        void runDecodedInstr(int32 instr);
        void advance_pc(int32 offset);
        void step() { advance_pc(sizeof(int32)); }

    public:
        void loadProgram(boost::shared_ptr< std::vector<int32> >);
        void stepProgram(int numSteps = 1);
        void runProgram();
        void reset();
        int32 gprValue(int index) const { return _GPR[index]; }
        void setGPR(int index, int32 value) { _GPR[index] = value; } // range checking?
        int32 hi() const { return _HI; }
        int32 lo() const { return _LO; }

    private:
        // arithmetic instructions
        void op_add(int32);
        void op_addi(int32);
        void op_addu(int32);
        void op_addiu(int32);
        void op_sub(int32);
        void op_subu(int32);
        void op_mult(int32);
        void op_div(int32);
        void op_divu(int32);

        // branching and jumping
        void op_beq(int32);
        void op_bne(int32);
        void op_j(int32);
        void op_jr(int32);
        void op_jal(int32);

        // moving
        void op_mfhi(int32);
        void op_mflo(int32);
        void op_mthi(int32);
        void op_mtlo(int32);

        // logical instructions
        void op_and(int32);
        void op_andi(int32);
        void op_or(int32);
        void op_ori(int32);
        void op_xor(int32);
        void op_nor(int32);
    
    private:
        std::vector<int32> _GPR, _FPR, _FCR;
        boost::shared_ptr< std::vector<int32> > _program;
        boost::unordered_map<int32, OpcodeFn> _fnMap;
#ifdef TRACE_OPCODES
        boost::unordered_map<int32, std::string> _opNameMap;
#endif
        int32 _HI, _LO, _PC, _nPC, _FCSR;
    };
    
} // tememu

#endif //include guard
