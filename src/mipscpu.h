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
#include <vector>

#define CALL_MEMBER(obj,fn) ((obj)->*(fn))

namespace tememu 
{
    typedef boost::int_fast32_t int32;
    /**
     * @brief Maintains the state of the MIPS CPU
     */
    class MipsCPU 
    {
        typedef void (MipsCPU::*OpcodeFn)(int32);

    public:
        MipsCPU();
        ~MipsCPU();

    private:
        void runDecodedInstr(int32 instr);
        void advance_pc(int32 offset);

    private:
        void op_add(int32);
    
    private:
        std::vector<int32> _GPR, _FPR, _FCR;
        boost::unordered_map<int32, OpcodeFn> _fnMap;
        int32 _HI, _LO, _PC, _nPC, _FCSR;
    };
    
} // tememu

#endif //include guard
