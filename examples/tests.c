// The TLCS system was initially described here:
// https://hackmd.io/WYp7A-jPQvK8xSB1pyH7hQ
// 
// Vincenzo Iovino, 2023, Aragon ZK Research

//const char *g2Str = "1 0x24aa2b2f08f0a91260805272dc51051c6e47ad4fa403b02b4510b647ae3d1770bac0326a805bbefd48056c8c121bdb8 0x13e02b6052719f607dacd3a088274f65596bd0d09920b61ab5da61bbdc7f5049334cf11213945d57e5ac7d055d042b7e 0x0ce5d527727d6e118cc9cdc6da2e351aadfd9baa8cbdd3a76d429a695160d12c923ac9cc3baca289e193548608b82801 0x0606c4a02ea734cc32acd2b02bc28b99cb3e287e85a763af267492ab572e99ab3f370d275cec1da1aaa9075ff05f79be";
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <mcl/bn_c384_256.h>
#include "tlcs.h"
#include "pairing.h"

// we verify the signature for round 1 of the LOE unchained chain
int
main (void)
{
//const char *g1Str = "1 0x17f1d3a73197d7942695638c4fa9ac0fc3688c4f9774b905a14e3a3f171bac586c55e83ff97a1aeffb3af00adb22c6bb 0x08b3f481e3aaa0f1a09e30ed741d8ae4fcf5e095d5d00af600db18cb2c04b3edd03cc744a2888ae40caa232946c5e7e1";
//const char *g1Str="1 3685416753713387016781088315183077757961620795782546409894578378688607592378376318836054947676345821548104185464507 1339506544944476473020471379941921221584933875938349620426543736416511423956333506472724655353366534992391756441569";
  const char *g2Str =
    "1 352701069587466618187139116011060144890029952792775240219908644239793785735715026873347600343865175952761926303160 3059144344244213709971259814753781636986470325476647558659373206291635324768958432433509563104347017837885763365758 1985150602287291935568054521177171638300868978215655730859378665066344726373823718423869104263333984641494340347905 927553665492332455747201965776037880757740193453592970025027978793976877002675564980949289727957565575433344219582";
  const char *PK_UNCHAINED_LOEStr =
    "a0b862a7527fee3a731bcb59280ab6abd62d5c0b6ea03dc4ddf6612fdfc9d01f01c31542541771903475eb1ec6615f8d0df0b8b6dce385811d6dcf8cbefb8759e5e616a3dfd054c928940766d9a5b9db91e3b697e5d70a975181e007f87fca5e";
  const char *sigStr =
    "9544ddce2fdbe8688d6f5b4f98eed5d63eee3902e7e162050ac0f45905a55657714880adabe3c3096b92767d886567d0";
  mclBnGT e1, e2;
  mclBnG2 g2, PK_UNCHAINED_LOE;
  mclBnG1 sig, HashedMsg;
//char round[8]={ 0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x01}; // 1 in Big Endian
  uint64_t round = 1;
  uint64_t round_big_endian;

  const char *dst = "BLS_SIG_BLS12381G2_XMD:SHA-256_SSWU_RO_NUL_";

  round_big_endian = __builtin_bswap64 (round);
  int ret = mclBn_init (MCL_BLS12_381, MCLBN_COMPILED_TIME_VAR);
  printf ("%d\n", ret);
//ret=mclBnG1_setStr(&g1, g1Str, strlen(g1Str), 10);    
//printf("%d\n",ret);
  ret = mclBnG2_setStr (&g2, g2Str, strlen (g2Str), 10);
  printf ("%d\n", ret);
  mclBn_setETHserialization (1);
  ret =
    mclBnG2_setStr (&PK_UNCHAINED_LOE, PK_UNCHAINED_LOEStr,
		    strlen (PK_UNCHAINED_LOEStr), MCLBN_IO_SERIALIZE_HEX_STR);
  printf ("%d\n", ret);
  ret =
    mclBnG1_setStr (&sig, sigStr, strlen (sigStr),
		    MCLBN_IO_SERIALIZE_HEX_STR);
  printf ("%d\n", ret);
  mclBn_setETHserialization (0);
  mclBn_setMapToMode (MCL_MAP_TO_MODE_HASH_TO_CURVE);
  mclBnG1_setDst (dst, strlen (dst));
  {
    unsigned char buf_for_hashing[SHA256_DIGEST_LENGTH];
    static SHA256_CTX ctx;
    SHA256_Init (&ctx);
    SHA256_Update (&ctx, (unsigned char *) &round_big_endian, 8);
    SHA256_Final (buf_for_hashing, &ctx);	// buf_for_hashing=SHA256(round_big_endian);
//mclBnG1_hashAndMapTo(&HashedMsg, (void *)&msg, 8);
    mclBnG1_hashAndMapTo (&HashedMsg, (void *) buf_for_hashing, 32);	// HashedMsg=H(buf_for_hashing) where H is the Hash to Point function for BLS12 group except that the domain is the string dst
  }

  printf ("%s\n", G1_toHexString (&HashedMsg));
  mclBn_pairing (&e1, &sig, &g2);	// e1=pairing(sig,g2)
  mclBn_pairing (&e2, &HashedMsg, &PK_UNCHAINED_LOE);	// e2=pairing(HashedMsg,PK_UNCHAINED_LOE)
  ret = mclBnGT_isEqual (&e1, &e2);	// check e1==e2
  printf ("verify: %d\n", ret);
  return 0;
}
