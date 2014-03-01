/*
* Author qiong
* aes encrypt interface using in cocos2d-x game
*/

#ifndef AES_MY_H
#define AES_MY_H

#ifdef __cplusplus
extern "C" {
#endif
    
// user has to free the output
void encrypt(unsigned long length, const unsigned char *input, unsigned char *output);
// user has to do nothing
void decrypt(unsigned long length, const unsigned char *input, unsigned char *output);
    
#ifdef __cplusplus
}
#endif

#endif