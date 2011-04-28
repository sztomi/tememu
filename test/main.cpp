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

#include <boost/cstdint.hpp>

#include <bitset>
#include <iostream>
#include "../src/mipscpu.h"
#include "../src/instruction.h"
#include "gtest/gtest.h"

typedef boost::int_fast32_t int32;

TEST(RInstruction, ExtractOpcode1)
{
    tememu::RInstruction inst(0xFFFFFFFF);
    EXPECT_EQ(inst.opcode, 63);
}

TEST(RInstruction, ExtractRs1)
{
    tememu::RInstruction inst(0xFFFFFFFF);
    EXPECT_EQ(inst.rs, 31);
}

TEST(RInstruction, ExtractRt1)
{
    tememu::RInstruction inst(0xFFFFFFFF);
    EXPECT_EQ(inst.rt, 31);
}

TEST(RInstruction, ExtractRd1)
{
    tememu::RInstruction inst(0xFFFFFFFF);
    EXPECT_EQ(inst.rd, 31);
}

TEST(RInstruction, ExtractShamt1)
{
    tememu::RInstruction inst(0xFFFFFFFF);
    EXPECT_EQ(inst.shamt, 31);
}

TEST(RInstruction, ExtractFunct1)
{
    tememu::RInstruction inst(0xFFFFFFFF);
    EXPECT_EQ(inst.funct, 63);
}

TEST(RInstruction, ExtractOpcode2)
{
    tememu::RInstruction inst(0x0399e021);
    EXPECT_EQ(inst.opcode, 0);
}

TEST(RInstruction, ExtractRs2)
{
    tememu::RInstruction inst(0x0399e021);
    EXPECT_EQ(inst.rs, 28);
}

TEST(RInstruction, ExtractRt2)
{
    tememu::RInstruction inst(0x0399e021);
    EXPECT_EQ(inst.rt, 25);
}

TEST(RInstruction, ExtractRd2)
{
    tememu::RInstruction inst(0x0399e021);
    EXPECT_EQ(inst.rd, 28);
}

TEST(RInstruction, ExtractFunct2)
{
    tememu::RInstruction inst(0x0399e021);
    EXPECT_EQ(inst.funct, 0x21);
}

TEST(IInstruction, ExtractOpcode1)
{
    tememu::IInstruction inst(0xffffffff);
    EXPECT_EQ(inst.opcode, 63);
}

TEST(IInstruction, ExtractRs1)
{
    tememu::IInstruction inst(0xffffffff);
    EXPECT_EQ(inst.rs, 31);
}

TEST(IInstruction, ExtractRd1)
{
    tememu::IInstruction inst(0xffffffff);
    EXPECT_EQ(inst.rt, 31);
}

TEST(IInstruction, ExtractImmediate1)
{
    tememu::IInstruction inst(0xffffffff);
    EXPECT_EQ(inst.immediate, 0xffff);
}

TEST(IInstruction, ExtractOpcode2)
{
    tememu::IInstruction inst(0x24625bfa);
    EXPECT_EQ(inst.opcode, 0x9);
}

TEST(IInstruction, ExtractRs2)
{
    tememu::IInstruction inst(0x24625bfa);
    EXPECT_EQ(inst.rs, 3);
}

TEST(IInstruction, ExtractRd2)
{
    tememu::IInstruction inst(0x24625bfa);
    EXPECT_EQ(inst.rt, 2);
}

TEST(IInstruction, ExtractImmediate2)
{
    tememu::IInstruction inst(0x24625bfa);
    EXPECT_EQ(inst.immediate, 23546);
}

TEST(JInstruction, ExtractOpcode1)
{
    tememu::JInstruction inst(0xffffffff);
    EXPECT_EQ(inst.opcode, 63);
}

TEST(JInstruction, ExtractAddress1)
{
    tememu::JInstruction inst(0xffffffff);
    EXPECT_EQ(inst.address, 67108863 << 2);
}

TEST(JInstruction, ExtractOpcode2)
{
    tememu::JInstruction inst(0x08100002);
    EXPECT_EQ(inst.opcode, 2);
}

TEST(JInstruction, ExtractAddress2)
{
    tememu::JInstruction inst(0x08100002);
    EXPECT_EQ(inst.address, 0x00400008);
}

TEST(JInstruction, ExtractAddress3)
{
    tememu::JInstruction inst(0x08100006);
    EXPECT_EQ(inst.address, 0x00400018);
}
