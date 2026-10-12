#include "pch.h"
#include "System/Security/Cryptography/SHA256.h"
#include "System/ArgumentNullException.h"
#include "System/Text/StringBuilder.h"
#include <cstring>
#include <iomanip>
#include <sstream>

namespace DotNetDupe {
    namespace System {
        namespace Security {
            namespace Cryptography {

                #define SHA2_SHFR(x, n)    (x >> n)
                #define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
                #define SHA2_CH(x, y, z)  (((x) & (y)) ^ (~(x) & (z)))
                #define SHA2_MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
                #define SHA256_F1(x) (SHA2_ROTR(x,  2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
                #define SHA256_F2(x) (SHA2_ROTR(x,  6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
                #define SHA256_F3(x) (SHA2_ROTR(x,  7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x,  3))
                #define SHA256_F4(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))

                #define SHA2_UNPACK32(x, str) {             \
                    *((str) + 3) = (unsigned char)((x));     \
                    *((str) + 2) = (unsigned char)((x) >> 8); \
                    *((str) + 1) = (unsigned char)((x) >> 16);\
                    *((str) + 0) = (unsigned char)((x) >> 24);\
                }

                #define SHA2_PACK32(str, x) {               \
                    *(x) = ((unsigned int)*((str) + 3))     \
                         | ((unsigned int)*((str) + 2) << 8) \
                         | ((unsigned int)*((str) + 1) << 16)\
                         | ((unsigned int)*((str) + 0) << 24);\
                }

                static const unsigned int sha256_k_consts[64] = {
                    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
                    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
                    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
                    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
                    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
                    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
                    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
                    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
                    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
                };

                class PureSha256 {
                public:
                    void Init() {
                        h[0] = 0x6a09e667; h[1] = 0xbb67ae85; h[2] = 0x3c6ef372; h[3] = 0xa54ff53a;
                        h[4] = 0x510e527f; h[5] = 0x9b05688c; h[6] = 0x1f83d9ab; h[7] = 0x5be0cd19;
                        len = 0; tot_len = 0;
                    }
                    void Transform(const unsigned char* msg, unsigned int nb) {
                        unsigned int w[64], wv[8], t1, t2;
                        for (unsigned int i = 0; i < nb; i++) {
                            const unsigned char* sub = msg + (i << 6);
                            for (unsigned int j = 0; j < 16; j++) SHA2_PACK32(&sub[j << 2], &w[j]);
                            for (unsigned int j = 16; j < 64; j++) w[j] = SHA256_F4(w[j-2]) + w[j-7] + SHA256_F3(w[j-15]) + w[j-16];
                            for (unsigned int j = 0; j < 8; j++) wv[j] = h[j];
                            for (unsigned int j = 0; j < 64; j++) {
                                t1 = wv[7] + SHA256_F2(wv[4]) + SHA2_CH(wv[4], wv[5], wv[6]) + sha256_k_consts[j] + w[j];
                                t2 = SHA256_F1(wv[0]) + SHA2_MAJ(wv[0], wv[1], wv[2]);
                                wv[7] = wv[6]; wv[6] = wv[5]; wv[5] = wv[4]; wv[4] = wv[3] + t1;
                                wv[3] = wv[2]; wv[2] = wv[1]; wv[1] = wv[0]; wv[0] = t1 + t2;
                            }
                            for (unsigned int j = 0; j < 8; j++) h[j] += wv[j];
                        }
                    }
                    void Update(const unsigned char* msg, unsigned int mlen) {
                        unsigned int rem = 64 - len;
                        unsigned int cpy = mlen < rem ? mlen : rem;
                        std::memcpy(&block[len], msg, cpy);
                        if (len + mlen < 64) { len += mlen; return; }
                        Transform(block, 1);
                        unsigned int nb = (mlen - cpy) >> 6;
                        Transform(msg + cpy, nb);
                        cpy += (nb << 6); len = mlen - cpy;
                        std::memcpy(block, msg + cpy, len);
                        tot_len += (nb + 1) << 6;
                    }
                    void Final(unsigned char* digest) {
                        unsigned int nb = (64 - 9 < len) ? 2 : 1;
                        tot_len += len;
                        std::memset(&block[len], 0, 64 - len);
                        block[len] = 0x80;
                        if (nb == 2) { Transform(block, 1); std::memset(block, 0, 64); }
                        SHA2_UNPACK32(tot_len << 3, &block[60]);
                        Transform(block, 1);
                        for (int i = 0; i < 8; i++) SHA2_UNPACK32(h[i], &digest[i << 2]);
                    }
                private:
                    unsigned int tot_len, len;
                    unsigned char block[64];
                    unsigned int h[8];
                };

                SHA256::SHA256() {}

                SmartPointer<SHA256> SHA256::Create() {
                    return SmartPointer<SHA256>::NewShared();
                }

                Array<byte> SHA256::ComputeHash(const Array<byte>& buffer) {
                    PureSha256 sha; sha.Init();
                    if (buffer.GetLength() > 0) {
                        sha.Update(reinterpret_cast<const unsigned char*>(buffer.GetData()), static_cast<unsigned int>(buffer.GetLength()));
                    }
                    unsigned char digest[32];
                    sha.Final(digest);
                    Array<byte> result(32);
                    for (int i = 0; i < 32; i++) result[i] = digest[i];
                    return result;
                }

                Array<byte> SHA256::ComputeHash(IO::Stream* pStream) {
                    if (!pStream) throw ArgumentNullException("Stream cannot be null.");
                    PureSha256 sha; sha.Init();
                    char szChunk[4096];
                    int nBytesRead = 0;
                    while ((nBytesRead = pStream->Read(szChunk, 0, 4096)) > 0) {
                        sha.Update(reinterpret_cast<const unsigned char*>(szChunk), static_cast<unsigned int>(nBytesRead));
                    }
                    unsigned char digest[32];
                    sha.Final(digest);
                    Array<byte> result(32);
                    for (int i = 0; i < 32; i++) result[i] = digest[i];
                    return result;
                }

                Array<byte> SHA256::HashData(const Array<byte>& buffer) {
                    SHA256 hasher;
                    return hasher.ComputeHash(buffer);
                }

                Array<byte> SHA256::HashData(IO::Stream* pStream) {
                    SHA256 hasher;
                    return hasher.ComputeHash(pStream);
                }

                String SHA256::ToHexString(const Array<byte>& arrHash) {
                    static const char szHexDigits[] = "0123456789abcdef";
                    std::string strResult;
                    strResult.reserve(arrHash.GetLength() * 2);
                    for (int i = 0; i < arrHash.GetLength(); i++) {
                        byte b = arrHash[i];
                        strResult.push_back(szHexDigits[(b >> 4) & 0x0F]);
                        strResult.push_back(szHexDigits[b & 0x0F]);
                    }
                    return String(strResult.c_str());
                }

            }
        }
    }
}
