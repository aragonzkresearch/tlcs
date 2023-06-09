// The TLCS system was initially described here:
// https://hackmd.io/WYp7A-jPQvK8xSB1pyH7hQ
// 
// Vincenzo Iovino, 2023, Aragon ZK Research
#ifndef _PAIRING_H_
#define _PAIRING_H_ 1
//#include <mcl/bn_c384_256.h>
#include "global_bufs.h"
typedef mclBnG1 G1;
typedef mclBnG2 G2;
typedef mclBnGT GT;
typedef mclBnFr Zp;

extern G2 G2Generator;
int pairing_init (void);
void GT_copy (GT * a, GT * b);
inline size_t
Zp_serialize (unsigned char *buf, size_t maxBufSize, const Zp * x)
{
  size_t length = mclBnFr_serialize (buf, maxBufSize, x);
  buf[length] = '\0';
  return length;
}

inline size_t
G1_serialize (unsigned char *buf, size_t maxBufSize, const G1 * g)
{
  size_t length = mclBnG1_serialize (buf, maxBufSize, g);
  buf[length] = '\0';
  return length;
}

#define Zp_deserialize(x,buf,maxBufSize) (mclBnFr_deserialize(x,buf,maxBufSize))
#define G1_deserialize(g,buf,maxBufSize) (mclBnG1_deserialize(g,buf,maxBufSize))
#define G1_setStr(g1,g1Str,len_g1Str,base) (mclBnG1_setStr(g1,g1Str,len_g1Str,base))
#define G2_setStr(g2,g2Str,len_g2Str,base) (mclBnG2_setStr(g2,g2Str,len_g2Str,base))
#define Zp_setRand(x) (mclBnFr_setByCSPRNG(x))
#define Zp_add(z,x,y) (mclBnFr_add(z,x,y))
#define Zp_sub(z,x,y) (mclBnFr_sub(z,x,y))
#define Zp_mul(z,x,y) (mclBnFr_mul(z,x,y))
#define Zp_inverse(z,x) (mclBnFr_inv(z,x))
#define G1_mul(h, g1, x) (mclBnG1_mul(h, g1, x))
#define G2_mul(h, g2, x) (mclBnG2_mul(h, g2, x))
#define G1_add(h, u, v) (mclBnG1_add(h, u, v))
#define GT_pow(e, gt, x) (mclBnGT_pow(e, gt, x))
#define G1_isEqual(h, u) (mclBnG1_isEqual(h, u))
#define G2_isEqual(h, u) (mclBnG2_isEqual(h, u))
#define Zp_isEqual(h, u) (mclBnFr_isEqual(h, u))
#define GT_isEqual(e1, e2) (mclBnGT_isEqual(e1, e2))
#define G1_hashAndMapTo(g1,buf,bufSize) (mclBnG1_hashAndMapTo(g1, buf, bufSize))
#define pairing(e,g1,g2) (mclBn_pairing(e,g1,g2))
#define GT_serialize(buf,len,e) (mclBnGT_serialize(buf,len,e))
#define GT_deserialize(e,buf,len) (mclBnGT_deserialize(e,buf,len))
#define G2_serialize(buf,len,g2) (mclBnG2_serialize(buf,len,g2))
#define G2_deserialize(g2,buf,len) (mclBnG2_deserialize(g2,buf,len))
char *G1_toHexString (const G1 * g);
char *G2_toHexString (const G2 * g);
size_t GT_toHexString (char *buf, const GT * e);
char *Zp_toHexString (const Zp * x);
int G2_fromHexString (G2 * g, const char *s);
int Zp_fromHexString (Zp * x, const char *s);
inline void
Zp_copy (Zp * a, Zp * b)
{
  Zp_serialize (buf_for_serializing, sizeof (buf_for_serializing), b);
  Zp_deserialize (a, buf_for_serializing, sizeof (buf_for_serializing));
}
#endif
