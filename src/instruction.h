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

#ifndef _INSTRUCTION_H
#define _INSTRUCTION_H

#include <boost/cstdint.hpp>

namespace tememu
{
    typedef boost::int_fast32_t int32;

    struct RInstruction
    {
        int32 opcode, rs, rt, rd, shamt, funct;

        RInstruction(int32 instr) 
        {
            opcode = (instr & 0xFC000000) >> 26;    // extract bits 0..5
            rs = (instr & 0x03E00000) >> 21;        // extract bits 6..10
            rt = (instr & 0x001F0000) >> 16;        // extract bits 11..15
            rd = (instr & 0x0000F800) >> 11;        // extract bits 16..20
            shamt = (instr & 0x000001F0) >> 4;     // extract bits 21..25
            funct = instr & 0x0000003F;     // extract bits 26..31
        }
    };

    struct IInstruction
    {
        int32 opcode, rs, rt, immediate;

        IInstruction(int32 instr)
        {
            opcode = (instr & 0xFC000000) >> 26;    // extract bits 0..5
            rs = (instr & 0x03E00000) >> 21;        // extract bits 6..10
            rt = (instr & 0x001F0000) >> 16;        // extract bits 11..15
            immediate = instr & 0x0000FFFF; // extract bits 16..31
        }
    };
    
    struct JInstruction
    {
        int32 opcode, address;

        JInstruction(int32 instr)
        {
            opcode = instr & 0xFB000000;     // extract bits 0..5
            address = instr & ~(0xFB000000); // extract bits 6..31
        }
    };
}

#endif
