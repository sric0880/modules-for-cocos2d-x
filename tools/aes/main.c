#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <dirent.h>
#include <unistd.h>

#include "aes.h"

using namespace std;

int main(int argv, char * args[])
{
    if(argv != 4){
        printf("Usage: aes [-e|-d] [dir] [key(maxlen:16 bytes)]\n");
        return 0;
    }
    bool isEncrypt;
    if(strcmp(args[1],"-e")==0){
        isEncrypt = true;
    }else if(strcmp(args[1],"-d")==0){
        isEncrypt = false;
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
        ifstream ifs(filename);
        if(!ifs.is_open()){
            printf("file open error!\n");
            return 0;
        }
        filebuf* pbuf = ifs.rdbuf();
        size_t size = pbuf->pubseekoff(0,ifs.end,ifs.in);
        size_t size_16 = (size/16+1)*16;//加密的内容长度必须为16的倍数
        pbuf->pubseekpos(0,ifs.in);

        unsigned char * buffer = new unsigned char[size_16];
        memset(buffer, 0, size_16);//填充0

        pbuf->sgetn((char *)buffer,size);
        ifs.close();

        aes_context aes;
        unsigned char tmp[16];
        int keysize = 128;//密钥长度128bit
        memset(tmp, 0, sizeof(tmp));
        strcpy((char *)tmp, args[3]);//密钥
        if(isEncrypt){
            aes_setkey_enc(&aes, tmp, keysize);
            aes_crypt_cbc(&aes, AES_ENCRYPT, size_16, tmp, buffer, buffer);
//            aes_crypt_ecb(&aes, AES_ENCRYPT, buffer, buffer);
            strcat(filename,".data");
        }else{
            aes_setkey_dec(&aes, tmp, keysize);
            aes_crypt_cbc(&aes, AES_DECRYPT, size_16, tmp, buffer, buffer);
//            aes_crypt_ecb(&aes, AES_DECRYPT, buffer, buffer);
            char * pch = strstr(filename,".data");
            if(pch !=NULL)
                *pch = '\0';
        }

        //output

        ofstream ofs(filename);
        ofs.write((char *)buffer,size_16);
        delete [] buffer;
        ofs.close();
    }

    
    return 0;
}