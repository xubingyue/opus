/* Copyright (C) 2007-2009 Xiph.Org Foundation
   Copyright (C) 2003-2008 Jean-Marc Valin
   Copyright (C) 2007-2008 CSIRO
   Copyright (C) 2013      Parrot */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef FIXED_ARMv5E_H
#define FIXED_ARMv5E_H

#include "fixed_armv4.h"

#ifdef USE_MSVS_ARM_INTRINCICS
/** 16x32 multiplication, followed by a 16-bit shift right. Results fits in 32 bits */
#define MULT16_32_Q16_armv5e(a,b)   _arm_smulwb((b), (a))
/** 16x32 multiplication, followed by a 15-bit shift right. Results fits in 32 bits */
#define MULT16_32_Q15_armv5e(a,b)   _arm_smulwb(((b) << 1), (a))
/** 16x32 multiply, followed by a 15-bit shift right and 32-bit add.
b must fit in 31 bits.
Result fits in 32 bits. */
#define MAC16_32_Q15_armv5e(c,a,b)  _arm_smlawb(((b) << 1), (a), (c))
/** 16x16 multiply-add where the result fits in 32 bits */
#define MAC16_16_armv5e(c,a,b)      _arm_smlabb((a), (b), (c))
/** 16x16 multiplication where the result fits in 32 bits */
#define MULT16_16_armv5e(a,b)       _arm_smulbb((a), (b))
#else
/** 16x32 multiplication, followed by a 16-bit shift right. Results fits in 32 bits */
static OPUS_INLINE opus_val32 MULT16_32_Q16_armv5e(opus_val16 a, opus_val32 b)
{
  int res;
  __asm__(
      "#MULT16_32_Q16\n\t"
      "smulwb %0, %1, %2\n\t"
      : "=r"(res)
      : "r"(b),"r"(a)
  );
  return res;
}

/** 16x32 multiplication, followed by a 15-bit shift right. Results fits in 32 bits */
static OPUS_INLINE opus_val32 MULT16_32_Q15_armv5e(opus_val16 a, opus_val32 b)
{
  int res;
  __asm__(
      "#MULT16_32_Q15\n\t"
      "smulwb %0, %1, %2\n\t"
      : "=r"(res)
      : "r"(b), "r"(a)
  );
  return res<<1;
}

/** 16x32 multiply, followed by a 15-bit shift right and 32-bit add.
    b must fit in 31 bits.
    Result fits in 32 bits. */
static OPUS_INLINE opus_val32 MAC16_32_Q15_armv5e(opus_val32 c, opus_val16 a,
 opus_val32 b)
{
  int res;
  __asm__(
      "#MAC16_32_Q15\n\t"
      "smlawb %0, %1, %2, %3;\n"
      : "=r"(res)
      : "r"(b<<1), "r"(a), "r"(c)
  );
  return res;
}

/** 16x16 multiply-add where the result fits in 32 bits */
static OPUS_INLINE opus_val32 MAC16_16_armv5e(opus_val32 c, opus_val16 a,
 opus_val16 b)
{
  int res;
  __asm__(
      "#MAC16_16\n\t"
      "smlabb %0, %1, %2, %3;\n"
      : "=r"(res)
      : "r"(a), "r"(b), "r"(c)
  );
  return res;
}

/** 16x16 multiplication where the result fits in 32 bits */
static OPUS_INLINE opus_val32 MULT16_16_armv5e(opus_val16 a, opus_val16 b)
{
  int res;
  __asm__(
      "#MULT16_16\n\t"
      "smulbb %0, %1, %2;\n"
      : "=r"(res)
      : "r"(a), "r"(b)
  );
  return res;
}

#endif  //USE_MSVS_ARM_INTRINCICS

#undef MULT16_32_Q16
#define MULT16_32_Q16(a, b) (MULT16_32_Q16_armv5e(a, b))
#undef MULT16_32_Q15
#define MULT16_32_Q15(a, b) (MULT16_32_Q15_armv5e(a, b))
#undef MAC16_32_Q15
#define MAC16_32_Q15(c, a, b) (MAC16_32_Q15_armv5e(c, a, b))
#undef MAC16_16
#define MAC16_16(c, a, b) (MAC16_16_armv5e(c, a, b))
#undef MULT16_16
#define MULT16_16(a, b) (MULT16_16_armv5e(a, b))

#endif
