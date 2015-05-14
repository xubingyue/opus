/***********************************************************************
Copyright (C) 2013 Xiph.Org Foundation and contributors.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
- Neither the name of Internet Society, IETF or IETF Trust, nor the
names of specific contributors, may be used to endorse or promote
products derived from this software without specific prior written
permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/

#ifndef SILK_MACROS_ARMv4_H
#define SILK_MACROS_ARMv4_H

#ifdef USE_MSVS_ARM_INTRINCICS
/* (a32 * (opus_int32)((opus_int16)(b32))) >> 16 output have to be 32bit int */
#define silk_SMULWB_armv4(a, b)    (_arm_smull((a),(opus_int32)((opus_int16)(b))) >> 16)
/* (a32 * (b32 >> 16)) >> 16 */
#define silk_SMULWT_armv4(a, b)     ((opus_int32)(_arm_smull((a), (b)&~0xFFFF)))
/* (a32 * b32) >> 16 */
#define silk_SMULWW_armv4(a, b)     silk_SMULWB_armv4((a), (b))
#define silk_SMLAWW_armv4(a, b, c)  (silk_SMULWW_armv4((b), (c)) + (a))
#else
/* (a32 * (opus_int32)((opus_int16)(b32))) >> 16 output have to be 32bit int */
static OPUS_INLINE opus_int32 silk_SMULWB_armv4(opus_int32 a, opus_int16 b)
{
  unsigned rd_lo;
  int rd_hi;
  __asm__(
      "#silk_SMULWB\n\t"
      "smull %0, %1, %2, %3\n\t"
      : "=&r"(rd_lo), "=&r"(rd_hi)
      : "%r"(a), "r"(b<<16)
  );
  return rd_hi;
}

/* (a32 * (b32 >> 16)) >> 16 */
static OPUS_INLINE opus_int32 silk_SMULWT_armv4(opus_int32 a, opus_int32 b)
{
  unsigned rd_lo;
  int rd_hi;
  __asm__(
      "#silk_SMULWT\n\t"
      "smull %0, %1, %2, %3\n\t"
      : "=&r"(rd_lo), "=&r"(rd_hi)
      : "%r"(a), "r"(b&~0xFFFF)
  );
  return rd_hi;
}

/* (a32 * b32) >> 16 */
static OPUS_INLINE opus_int32 silk_SMULWW_armv4(opus_int32 a, opus_int32 b)
{
  unsigned rd_lo;
  int rd_hi;
  __asm__(
    "#silk_SMULWW\n\t"
    "smull %0, %1, %2, %3\n\t"
    : "=&r"(rd_lo), "=&r"(rd_hi)
    : "%r"(a), "r"(b)
  );
  return (rd_hi<<16)+(rd_lo>>16);
}

static OPUS_INLINE opus_int32 silk_SMLAWW_armv4(opus_int32 a, opus_int32 b,
 opus_int32 c)
{
  unsigned rd_lo;
  int rd_hi;
  __asm__(
    "#silk_SMLAWW\n\t"
    "smull %0, %1, %2, %3\n\t"
    : "=&r"(rd_lo), "=&r"(rd_hi)
    : "%r"(b), "r"(c)
  );
  return a+(rd_hi<<16)+(rd_lo>>16);
}
#endif //USE_MSVS_ARM_INTRINCICS

#undef silk_SMULWB 
#define silk_SMULWB(a, b) (silk_SMULWB_armv4(a, b))
/* a32 + (b32 * (opus_int32)((opus_int16)(c32))) >> 16 output have to be 32bit int */
#undef silk_SMLAWB
#define silk_SMLAWB(a, b, c) ((a) + silk_SMULWB(b, c))
#undef silk_SMULWT
#define silk_SMULWT(a, b) (silk_SMULWT_armv4(a, b))
///* a32 + (b32 * (c32 >> 16)) >> 16 */
#undef silk_SMLAWT
#define silk_SMLAWT(a, b, c) ((a) + silk_SMULWT(b, c))
//ERROR - FREEZE -> silk_SMULWB probably doesn't work
/*#undef silk_SMULWW
#define silk_SMULWW(a, b) (silk_SMULWW_armv4(a, b))
#undef silk_SMLAWW
#define silk_SMLAWW(a, b, c) (silk_SMLAWW_armv4(a, b, c))*/


#endif /* SILK_MACROS_ARMv4_H */
