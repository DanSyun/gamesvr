#pragma once

#include "singleton.hpp"
#include "type_def.h"
#include "aes.h"
#include "evp.h"

class AESCipher: public Singleton<AESCipher>
{
public:
    bool Decrypt(const uint8* encrypt, const uint8* key, uint8* out)
    {
        uint8 iv[EVP_MAX_IV_LENGTH] = {0};
        int mlen = 0;
        int flen = 0;
        int ret = 0;

        EVP_CIPHER_CTX* ctx;
        ctx = EVP_CIPHER_CTX_new();

        ret = EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, iv);
        if(ret != 1)
            return false;
        
        EVP_CIPHER_CTX_set_padding(ctx, 0);
        ret = EVP_DecryptUpdate(ctx, out, &mlen, encrypt, AES_BLOCK_SIZE*3);
        if(ret != 1)
            return false;

        ret = EVP_DecryptFinal_ex(ctx, out+mlen, &flen);
        if(ret != 1)
            return false;

        EVP_CIPHER_CTX_free(ctx);
        return true;
    }
};

