#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>

#include "aes.h"

int main(int argv, char * args[])
{
    if(argv != 4){
        printf("Usage: aes [-e|-d] [dir] [key(maxlen:16 bytes)]\n");
        return 0;
    }
    int isEncrypt;
    if(strcmp(args[1],"-e")==0){
        isEncrypt = 1;
    }else if(strcmp(args[1],"-d")==0){
        isEncrypt = 0;
    }else{
        printf("Usage: aes [-e|-d] [dir] [key(maxlen:16 bytes)]\n");
        return 0;
    }

    DIR * dir;
    if((dir = opendir(args[2])) ==NULL){
        printf("open dir %s failed!\n",args[1]);
        return 0;
    }
    chdir(args[2]);
    struct dirent* dir_ent;
    while((dir_ent = readdir(dir))!=NULL){
        if(strcmp(dir_ent->d_name,".")==0
            ||strcmp(dir_ent->d_name,"..")==0)
            continue;

        char filename[50];
        strcpy(filename,dir_ent->d_name);
        FILE * pIn;
        pIn = fopen(filename, "r");
        if (!pIn) {
            printf("file open error!\n");
            return 0;
        }
        fseek (pIn , 0 , SEEK_END);
        long size = ftell (pIn);
        rewind (pIn);
        size_t size_16 = ceil((float)size/16)*16;//加密的内容长度必须为16的倍数
        printf("size:%d, size_16: %d\n", size, size_16);

        unsigned char * buffer = (unsigned char*) malloc(size_16);
        memset(buffer, 0, size_16);//填充0

        size_t result = fread (buffer,1,size,pIn);
        if (result!=size) {
            printf("memory error\n");
            return 0;
        }
        fclose(pIn);

        aes_context aes;
        unsigned char tmp[16];
        int keysize = 128;//密钥长度128bit
        memset(tmp, 0, sizeof(tmp));
        strcpy((char *)tmp, args[3]);//密钥
        if(isEncrypt){
            aes_setkey_enc(&aes, tmp, keysize);
            aes_crypt_cbc(&aes, AES_ENCRYPT, size_16, tmp, buffer, buffer);
//            aes_crypt_ecb(&aes, AES_ENCRYPT, buffer, buffer);
            // strcat(filename,".data");
        }else{
            aes_setkey_dec(&aes, tmp, keysize);
            aes_crypt_cbc(&aes, AES_DECRYPT, size_16, tmp, buffer, buffer);
//            aes_crypt_ecb(&aes, AES_DECRYPT, buffer, buffer);
            // char * pch = strstr(filename,".data");
            // if(pch !=NULL)
            //     *pch = '\0';
        }

        //output
        
        FILE * pFile;
        pFile = fopen (filename , "w");
        if (pFile) {
            fwrite((char *)buffer,sizeof(char),size_16,pFile);
        }
        free(buffer);
        fclose(pFile);
    }

    
    return 0;
}