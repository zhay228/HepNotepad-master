
#ifndef DES_H
#define DES_H

#include   <stdio.h>
#include   <string.h>
#include <stdlib.h>
#include <string>
#define KEY "ECINC~16"
#define KEY_OUT "OUTECINC~16"

using namespace std;

/*
* 程序描述:      加/解密公用函数
*            decrypt / encrypt routine
*
*/
char * HexToStr(const char *lpInData, int iInSize);
int HexToInt(const char *lpInData);
char * StrToHex(const char *lpInData, int iInSize);
int DECRYPT( unsigned char *key, unsigned char *s,unsigned char *d, unsigned short len );
int ENCRYPT( unsigned char *key, unsigned char *s,unsigned char *d,unsigned short len );
char*  Encrypt(string content);
char*  Decrypt(string content);
#endif