#include "aes_my.h"

#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "aes.h"

static const char* key = "123456789012345";

void encrypt(unsigned long length, const unsigned char *input, unsigned char *output)
{
	size_t size_16 = (length/16+1)*16;
	unsigned char * buffer = (unsigned char *)malloc(size_16);
    memset(buffer, 0, size_16);//填充0
    memcpy(buffer, input, length);
    aes_context aes;
    unsigned char tmp[16];
    memset(tmp, 0, sizeof(tmp));
    strcpy((char *)tmp, key);//密钥
    aes_setkey_enc(&aes, tmp, 128);
    aes_crypt_cbc(&aes, AES_ENCRYPT, size_16, tmp, buffer, buffer);
    output = buffer;
    // free(buffer); // user has to free the output
}

void decrypt(unsigned long length, const unsigned char *input, unsigned char *output)
{
    assert(length%16==0);
    aes_context aes;
    unsigned char tmp[16];
    memset(tmp, 0, sizeof(tmp));
    strcpy((char *)tmp, key);//密钥
    aes_setkey_dec(&aes, tmp, 128);
    aes_crypt_cbc(&aes, AES_DECRYPT, length, tmp, input, output);
}