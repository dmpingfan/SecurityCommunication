#include "HelloWord.h"
#include "aes.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>
void main()
{
	int rv=0,i;
	unsigned char ch;
	char sch;
	unsigned int ich;
	char sbuf[128]={0};
	char tbuf[128]={0};
	char rbuf[128]={0};
	unsigned char ubuf[128]={0};
	printf("Hello World!!\n");
	printf("shuru scanf rbuf\n");
	scanf_s("%s",&rbuf,128);
	getchar();
	sprintf_s((char *)ubuf,128,"%s",rbuf);
	sprintf_s(tbuf,128,"%s",(char *)ubuf);
	for (i=0;i<255;i++)
	{
		ich=i+1;
		ch = (unsigned char) ich;
		sprintf_s(sbuf,128,"0x%02x",ich);
		sprintf_s(tbuf,128,"0x%02x",ch);
		if (strcmp(tbuf,sbuf))
		{
			getchar();
		}
		printf("%d: num=0x%02x; char=0x%02x\n",ich,ich,ch);
	}
	//rv=aes_cbc_test();
	getchar();
}