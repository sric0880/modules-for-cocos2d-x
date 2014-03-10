#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "aes.h"
#include "aes_my.h"

static const char* key = "123456789012345";

int encrypt_save(size_t length, const unsigned char *input, const char* fullpath)
{
	size_t size_16 = ceil((float)length/16)*16;
	unsigned char * buffer = (unsigned char *)malloc(size_16);
    memset(buffer, 0, size_16);//填充0
    memcpy(buffer, input, length);
    aes_context aes;
    unsigned char tmp[16];
    memset(tmp, 0, sizeof(tmp));
    strcpy((char *)tmp, key);//密钥
    aes_setkey_enc(&aes, tmp, 128);
    aes_crypt_cbc(&aes, AES_ENCRYPT, size_16, tmp, buffer, buffer);
    FILE * pf;
    pf=fopen(fullpath,"wb");
    if(pf == NULL){
    	printf("%s encrypt and save error!\n", fullpath);
    	return 0;
    }
    fwrite(buffer,sizeof(unsigned char),size_16,pf);
    fclose(pf);
    free(buffer);
    return 1;
}

void decrypt(size_t length, const unsigned char *input, unsigned char *output)
{
    assert(length%16==0);
    aes_context aes;
    unsigned char tmp[16];
    memset(tmp, 0, sizeof(tmp));
    strcpy((char *)tmp, key);//密钥
    aes_setkey_dec(&aes, tmp, 128);
    aes_crypt_cbc(&aes, AES_DECRYPT, length, tmp, input, output);
}