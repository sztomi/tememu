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
#include <fstream>
#include <string>

#include "../src/mipscpu.h"
#include "gtest/gtest.h"

typedef boost::int_fast32_t int32;

inline void loadMipsBinDump(const std::string& fileName, boost::shared_ptr< std::vector<int32> > data)
{
    std::ifstream is(fileName.c_str(), std::ios::in | std::ios::binary);
    std::vector<int32>* p = data.get();

    int32 x = 0;

    while (!is.eof())
    {
        is.read((char*)&x, sizeof(x));
        if (is.good()) p->push_back(x);
    }        
}

TEST(RInstruction, ExtractOpcode1)
{
	int32 inst(0xFFFFFFFF);
    EXPECT_EQ(OPCODE(inst), 63);
}

TEST(RInstruction, ExtractRs1)
{
	int32 inst(0xFFFFFFFF);
    EXPECT_EQ(RS(inst), 31);
}

TEST(RInstruction, ExtractRt1)
{
	int32 inst(0xFFFFFFFF);
    EXPECT_EQ(RT(inst), 31);
}

TEST(RInstruction, ExtractRd1)
{
	int32 inst(0xFFFFFFFF);
    EXPECT_EQ(RD(inst), 31);
}

TEST(RInstruction, ExtractShamt1)
{
	int32 inst(0xFFFFFFFF);
    EXPECT_EQ(SHAMT(inst), 31);
}

TEST(RInstruction, ExtractFunct1)
{
	int32 inst(0xFFFFFFFF);
    EXPECT_EQ(FUNCT(inst), 63);
}

TEST(RInstruction, ExtractOpcode2)
{
	int32 inst(0x0399e021);
    EXPECT_EQ(OPCODE(inst), 0);
}

TEST(RInstruction, ExtractRs2)
{
	int32 inst(0x0399e021);
    EXPECT_EQ(RS(inst), 28);
}

TEST(RInstruction, ExtractRt2)
{
	int32 inst(0x0399e021);
    EXPECT_EQ(RT(inst), 25);
}

TEST(RInstruction, ExtractRd2)
{
	int32 inst(0x0399e021);
    EXPECT_EQ(RD(inst), 28);
}

TEST(RInstruction, ExtractFunct2)
{
	int32 inst(0x0399e021);
    EXPECT_EQ(FUNCT(inst), 0x21);
}

TEST(IInstruction, ExtractOpcode1)
{
	int32 inst(0xffffffff);
    EXPECT_EQ(OPCODE(inst), 63);
}

TEST(IInstruction, ExtractRs1)
{
	int32 inst(0xffffffff);
    EXPECT_EQ(RS(inst), 31);
}

TEST(IInstruction, ExtractRd1)
{
	int32 inst(0xffffffff);
    EXPECT_EQ(RT(inst), 31);
}

TEST(IInstruction, ExtractImmediate1)
{
	int32 inst(0xffffffff);
    EXPECT_EQ(IMMEDIATE(inst), 0xffff);
}

TEST(IInstruction, ExtractOpcode2)
{
	int32 inst(0x24625bfa);
    EXPECT_EQ(OPCODE(inst), 0x9);
}

TEST(IInstruction, ExtractRs2)
{
	int32 inst(0x24625bfa);
    EXPECT_EQ(RS(inst), 3);
}

TEST(IInstruction, ExtractRd2)
{
	int32 inst(0x24625bfa);
    EXPECT_EQ(RT(inst), 2);
}

TEST(IInstruction, ExtractImmediate2)
{
	int32 inst(0x24625bfa);
    EXPECT_EQ(IMMEDIATE(inst), 23546);
}

TEST(JInstruction, ExtractOpcode1)
{
	int32 inst(0xffffffff);
    EXPECT_EQ(OPCODE(inst), 63);
}

TEST(JInstruction, ExtractAddress1)
{
	int32 inst(0xffffffff);
    EXPECT_EQ(ADDRESS(inst), 67108863 << 2);
}

TEST(JInstruction, ExtractOpcode2)
{
	int32 inst(0x08100002);
    EXPECT_EQ(OPCODE(inst), 2);
}

TEST(JInstruction, ExtractAddress2)
{
	int32 inst(0x08100002);
    EXPECT_EQ(ADDRESS(inst), 0x00400008);
}

TEST(JInstruction, ExtractAddress3)
{
	int32 inst(0x08100006);
    EXPECT_EQ(ADDRESS(inst), 0x00400018);
}

TEST(SimpleProgs, Addi1)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    program.get()->push_back(0x2084000c);
    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 12);
}

TEST(SimpleProgs, AddiAdd)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    std::vector<int32>* p = program.get();

    p->push_back(0x2084000c);
    p->push_back(0x20a50004);
    p->push_back(0x00a43020);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 12);
    EXPECT_EQ(cpu.gprValue(5), 4);
    EXPECT_EQ(cpu.gprValue(6), 16);
}

TEST(SimpleProgs, AddiSubi)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/add_sub1.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 12);
    EXPECT_EQ(cpu.gprValue(5), 4);
    EXPECT_EQ(cpu.gprValue(6), 8);
}

TEST(SimpleProgs, Divu1)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/divu.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 12);
    EXPECT_EQ(cpu.gprValue(5), 4);
    EXPECT_EQ(cpu.hi(), 0);
    EXPECT_EQ(cpu.lo(), 3);
}

TEST(Jumping, op_j)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/op_j.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 20);

}

TEST(Jumping, op_jal)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/op_jal.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 20);
    EXPECT_EQ(cpu.gprValue(31), 8);

}

TEST(Jumping, op_jr)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/op_jr.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 12);
    EXPECT_EQ(cpu.gprValue(5), 10);
}

TEST(Jumping, op_bne_true)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/op_bne_true.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 10);
    EXPECT_EQ(cpu.gprValue(5), 5);
}

TEST(Jumping, op_bne_false)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/op_bne_false.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 18);
    EXPECT_EQ(cpu.gprValue(5), 5);
}

TEST(Jumping, op_beq_true)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/op_beq_true.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 5);
    EXPECT_EQ(cpu.gprValue(5), 5);
}

TEST(Jumping, op_beq_false)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/op_beq_false.bin", program);

    cpu.loadProgram(program);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(4), 10);
    EXPECT_EQ(cpu.gprValue(5), 5);
}

TEST(Complex, fibonacci)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/fibo.bin", program);

    cpu.loadProgram(program);

    cpu.stepProgram(9);

    EXPECT_EQ(cpu.gprValue(2), 1);
    EXPECT_EQ(cpu.gprValue(4), 1);
    EXPECT_EQ(cpu.gprValue(5), 1);
    EXPECT_EQ(cpu.gprValue(6), 2);
    EXPECT_EQ(cpu.gprValue(7), 8);
    EXPECT_EQ(cpu.hi(), 2);
    EXPECT_EQ(cpu.lo(), 1);

    cpu.stepProgram();

    EXPECT_EQ(cpu.gprValue(7), 7);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(2), 1);
    EXPECT_EQ(cpu.gprValue(4), 21);
    EXPECT_EQ(cpu.gprValue(5), 34);
    EXPECT_EQ(cpu.gprValue(6), 55);
    EXPECT_EQ(cpu.gprValue(7), 0);
    EXPECT_EQ(cpu.hi(), 55);
    EXPECT_EQ(cpu.lo(), 34);
}

TEST(Complex, fibonacci2)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/fibo_2.bin", program);

    cpu.loadProgram(program);

    cpu.setGPR(7,8);

    cpu.stepProgram(9);

    EXPECT_EQ(cpu.gprValue(2), 1);
    EXPECT_EQ(cpu.gprValue(4), 1);
    EXPECT_EQ(cpu.gprValue(5), 1);
    EXPECT_EQ(cpu.gprValue(6), 1);
    EXPECT_EQ(cpu.gprValue(7), 8);
    EXPECT_EQ(cpu.hi(), 1);
    EXPECT_EQ(cpu.lo(), 1);

    cpu.stepProgram();

    EXPECT_EQ(cpu.gprValue(7), 7);

    cpu.runProgram();

    EXPECT_EQ(cpu.gprValue(2), 1);
    EXPECT_EQ(cpu.gprValue(4), 21);
    EXPECT_EQ(cpu.gprValue(5), 34);
    EXPECT_EQ(cpu.gprValue(6), 34);
    EXPECT_EQ(cpu.gprValue(7), 0);
    EXPECT_EQ(cpu.hi(), 34);
    EXPECT_EQ(cpu.lo(), 21);
}

int fibo(int n)
{
    int a = 1, b = 0, c = 0;
    for ( int i = 0; i < n; ++i )
    {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

TEST(Complex, fibonacci3)
{
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);
    tememu::MipsCPU cpu;

    loadMipsBinDump("testmips/fibo_2.bin", program);
    cpu.loadProgram(program);

    for ( int i = 1; i < 20; ++i )
    {
        cpu.setGPR(7,i); cpu.runProgram();
        EXPECT_EQ(fibo(i+1), cpu.gprValue(5));

        cpu.reset();
    }
 }

TEST(Logical, op_and)
{
    tememu::MipsCPU cpu;
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);

    program->push_back(0x00a63824); // and $a3, $a1, $a2 
    cpu.loadProgram(program);

    cpu.setGPR(5, 0xffffffff);
    cpu.setGPR(6, 0xf0000000);
    cpu.runProgram();
    EXPECT_EQ(cpu.gprValue(7), 0xf0000000);

    cpu.reset();

    cpu.setGPR(5, 0x00000000);
    cpu.setGPR(6, 0x00000000);
    cpu.runProgram();
    EXPECT_EQ(cpu.gprValue(7), 0);

    cpu.reset(); 

    cpu.setGPR(5, 0xff00ff00);
    cpu.setGPR(6, 0x0f0f0f0f);
    cpu.runProgram();
    EXPECT_EQ(cpu.gprValue(7), 0x0f000f00);
}

TEST(Logical, op_andi)
{
    tememu::MipsCPU cpu;
    boost::shared_ptr< std::vector<int32> > program(new std::vector<int32>);

    program->push_back(0x30a7ffff); // andi $a3, $a1, 65535 
    cpu.loadProgram(program);

    cpu.setGPR(5, 0xffffffff);
    cpu.runProgram();
    EXPECT_EQ(cpu.gprValue(7), 0x0000ffff);

    cpu.reset();

    cpu.setGPR(5, 0x00000000);
    cpu.runProgram();
    EXPECT_EQ(cpu.gprValue(7), 0);

    cpu.reset(); 

    cpu.setGPR(5, 0xff00ff00);
    cpu.runProgram();
    EXPECT_EQ(cpu.gprValue(7), 0x0000ff00);
}
