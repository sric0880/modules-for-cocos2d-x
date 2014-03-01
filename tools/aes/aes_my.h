/*
* Author qiong
* aes encrypt interface using in cocos2d-x game
*/

#ifndef AES_MY_H
#define AES_MY_H

#ifdef __cplusplus
extern "C" {
#endif

int encrypt_save(size_t length, const unsigned char *input, const char* fullpath);

void decrypt(size_t length, const unsigned char *input, unsigned char *output);
    
#ifdef __cplusplus
}
#endif

#endif