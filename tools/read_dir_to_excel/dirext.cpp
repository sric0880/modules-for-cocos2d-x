#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <fstream>
#include <set>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wchar.h>
#include <stdbool.h>
#include <errno.h>

#define CPP_BRIDGE_XLS

#include <xlslib.h>
using namespace xlslib_core;
using namespace xlslib_strings;

int enc_utf8_to_unicode_one(unsigned char *ch, int *unicode);

int main(int argv, char * args[]){
	if(argv!=3){
		printf("usage: radioext dir_path excel_file\n");
		return 0;
	}
	DIR * dir;
	if((dir = opendir(args[1])) ==NULL){
		printf("open dir %s failed!\n",args[1]);
		return 0;
	}
	chdir(args[1]);
	workbook *wb;
	   worksheet* ws;
	    wb = xlsNewWorkbook();
	    ws = xlsWorkbookSheet(wb, "sheet1");
	    xlsWorksheetLabel(ws, 0, 0, "file_name", NULL);
	    xlsWorksheetLabel(ws, 0, 1, "size(byte)", NULL);
	struct dirent* dir_ent;
	int row = 1;
	while((dir_ent = readdir(dir))!=NULL){
		if(strcmp(dir_ent->d_name,".")==0
			||strcmp(dir_ent->d_name,"..")==0)
			continue;
		struct stat s_model;
		stat(dir_ent->d_name,&s_model);
        dir_ent->d_name[strlen(dir_ent->d_name)-4] = '\0';
		printf("read file: %s,\tsize: %li\n", dir_ent->d_name,s_model.st_size);

		int wc[50];
		//memset(wc,'0',50*sizeof(int));
		int err = enc_utf8_to_unicode_one((unsigned char *)dir_ent->d_name, wc);
		// if(err == 0){
		// 	printf("convert from utf8 to unicode failed!\n");
		// 	continue;
		// }
		
		xlsWorksheetLabelW(ws, row, 0, (wchar_t *)wc, NULL);
	    xlsWorksheetNumberInt(ws, row, 1, s_model.st_size, NULL);
	    ++row;
	}
	char aa[10] = "./";
	strcat(aa,args[2]);
	int err = xlsWorkbookDump(wb, aa);
	    xlsDeleteWorkbook(wb);
	    if (err != 0)
	    {
	        fprintf(stderr, "Dump xls file failed: I/O failure %d.\n", err); 
	        return -1;  
	    }
	closedir(dir);
}

/*****************************************************************************
 * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *    ch      指向输入缓冲区, 以UTF-8编码
 *    unicode        指向输出缓冲区, 其保存的数据即是Unicode编码值,
 *                类型为unsigned long .
 *
 * 返回值:
 *    成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
 *
 * 注意:
 *     1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *        字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *        在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 ****************************************************************************/
int enc_utf8_to_unicode_one(unsigned char *ch, int *unicode)
{
    unsigned char *p = NULL;
    int * puni = NULL;

    if((p = ch) && (puni = unicode))
    {
    	while(*p!='\0'){
    		int e = 0, n = 0;
    		if(strcmp((char*)p,".m4a")==0){
    			*puni = 0;
    			return 0;
    		}
    			
    		if(*p >= 0xfc)
	        {
	            /*6:<11111100>*/
	            e = (p[0] & 0x01) << 30;
	            e |= (p[1] & 0x3f) << 24;
	            e |= (p[2] & 0x3f) << 18;
	            e |= (p[3] & 0x3f) << 12;
	            e |= (p[4] & 0x3f) << 6;
	            e |= (p[5] & 0x3f);
	            n = 6;
	        }
	        else if(*p >= 0xf8) 
	        {
	            /*5:<11111000>*/
	            e = (p[0] & 0x03) << 24;
	            e |= (p[1] & 0x3f) << 18;
	            e |= (p[2] & 0x3f) << 12;
	            e |= (p[3] & 0x3f) << 6;
	            e |= (p[4] & 0x3f);
	            n = 5;
	        }
	        else if(*p >= 0xf0)
	        {
	            /*4:<11110000>*/
	            e = (p[0] & 0x07) << 18;
	            e |= (p[1] & 0x3f) << 12;
	            e |= (p[2] & 0x3f) << 6;
	            e |= (p[3] & 0x3f);
	            n = 4;
	        }
	        else if(*p >= 0xe0)
	        {
	            /*3:<11100000>*/
	            e = (p[0] & 0x0f) << 12;
	            e |= (p[1] & 0x3f) << 6;
	            e |= (p[2] & 0x3f);
	            n = 3;
	        }
	        else if(*p >= 0xc0) 
	        {
	            /*2:<11000000>*/
	            e = (p[0] & 0x1f) << 6;
	            e |= (p[1] & 0x3f);
	            n = 2;
	        }
	        else 
	        {
	            e = p[0];
	            n = 1;
	        }
	        if(n == 0){
	        	*puni = 0;
	        	return 0;
	        }
	        	
	        p+=n;
	        *puni = e;
	        ++puni;
    	}
        *puni = 0;
    }
}
