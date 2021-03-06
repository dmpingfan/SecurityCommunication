/*************************************************************************
	> File Name: SM4.c
	> Author: 
	> Mail: 
	> Created Time: 2016年10月10日 星期一 11时38分06秒
 ************************************************************************/

#include<stdio.h>
#include<stdint.h>

#define ROUND 32
#define ENCRYPT 1
#define DECRYPT 0
#define ROTATE_LEFT(x,s,n) (((x)<<(n))|((x)>>((s)-(n))))
uint8_t sboxTable[16][16]=
{
    {
        0xd6,0x90,0xe9,0xfe,0xcc,0xe1,0x3d,0xb7,0x16,0xb6,0x14,0xc2,0x28,0xfb,0x2c,0x05
    },
    {
        0x2b,0x67,0x9a,0x76,0x2a,0xbe,0x04,0xc3,0xaa,0x44,0x13,0x26,0x49,0x86,0x06,0x99
    },
    {
        0x9c,0x42,0x50,0xf4,0x91,0xef,0x98,0x7a,0x33,0x54,0x0b,0x43,0xed,0xcf,0xac,0x62 
    },
    {
        0xe4,0xb3,0x1c,0xa9,0xc9,0x08,0xe8,0x95,0x80,0xdf,0x94,0xfa,0x75,0x8f,0x3f,0xa6
    },
    {
        0x47,0x07,0xa7,0xfc,0xf3,0x73,0x17,0xba,0x83,0x59,0x3c,0x19,0xe6,0x85,0x4f,0xa8 
    },
    {
        0x68,0x6b,0x81,0xb2,0x71,0x64,0xda,0x8b,0xf8,0xeb,0x0f,0x4b,0x70,0x56,0x9d,0x35
    },
    {
        0x1e,0x24,0x0e,0x5e,0x63,0x58,0xd1,0xa2,0x25,0x22,0x7c,0x3b,0x01,0x21,0x78,0x87
    },
    {
        0xd4,0x00,0x46,0x57,0x9f,0xd3,0x27,0x52,0x4c,0x36,0x02,0xe7,0xa0,0xc4,0xc8,0x9e
    },
    {
        0xea,0xbf,0x8a,0xd2,0x40,0xc7,0x38,0xb5,0xa3,0xf7,0xf2,0xce,0xf9,0x61,0x15,0xa1
    },
    {
        0xe0,0xae,0x5d,0xa4,0x9b,0x34,0x1a,0x55,0xad,0x93,0x32,0x30,0xf5,0x8c,0xb1,0xe3
    },
    {
        0x1d,0xf6,0xe2,0x2e,0x82,0x66,0xca,0x60,0xc0,0x29,0x23,0xab,0x0d,0x53,0x4e,0x6f
    },
    {
        0xd5,0xdb,0x37,0x45,0xde,0xfd,0x8e,0x2f,0x03,0xff,0x6a,0x72,0x6d,0x6c,0x5b,0x51
    },
    {
        0x8d,0x1b,0xaf,0x92,0xbb,0xdd,0xbc,0x7f,0x11,0xd9,0x5c,0x41,0x1f,0x10,0x5a,0xd8   
    },
    {
        0x0a,0xc1,0x31,0x88,0xa5,0xcd,0x7b,0xbd,0x2d,0x74,0xd0,0x12,0xb8,0xe5,0xb4,0xb0 
    },
    {
        0x89,0x69,0x97,0x4a,0x0c,0x96,0x77,0x7e,0x65,0xb9,0xf1,0x09,0xc5,0x6e,0xc6,0x84
    },
    {
        0x18,0xf0,0x7d,0xec,0x3a,0xdc,0x4d,0x20,0x79,0xee,0x5f,0x3e,0xd7,0xcb,0x39,0x48
    }
};

void process(int,int,uint32_t*,uint64_t*,uint64_t*);
void generateSubRoundKey(uint64_t*,uint32_t*);

int main()
{
	int i;
    //key schedule
    uint64_t mainKey[2]={0x0123456789abcdef,0xfedcba9876543210};
    uint32_t subRoundKey[32];
    
    uint64_t plainText[2]={0x0123456789abcdef,0xfedcba9876543210};

    uint64_t cipherText[2];
	uint64_t plainText1[2];
	generateSubRoundKey(mainKey,subRoundKey);
	for(i=0;i<32;i++)
		printf("rk[%d]=%#x\n",i,subRoundKey[i]);
    process(ENCRYPT,ROUND,subRoundKey,plainText,cipherText);
    printf("The Cipher is : %#lx%#lx\n",cipherText[0],cipherText[1]);
 
    process(DECRYPT,ROUND,subRoundKey,cipherText,plainText1);
	printf("The Plain is : %#lx%#lx\n",plainText1[0],plainText1[1]);
	printf("new X[] = 0x%016llx0x%016llx\n",plainText1[0],plainText1[1]);
	printf("old X[] = 0x%016llx0x%016llx\n",plainText[0],plainText[1]);
    
    return 0;
}
void generateSubRoundKey(uint64_t mainKey[2],uint32_t subRoundKey[32])
{
	int round;
    uint32_t T1(uint32_t);
    uint32_t FK[4]={0xa3b1bac6,0x56aa3350,0x677d9197,0xb27022dc};
    uint32_t CK[32]=
    {
        0x00070e15,0x1c232a31,0x383f464d,0x545b6269,
        0x70777e85,0x8c939aa1,0xa8afb6bd,0xc4cbd2d9,
        0xe0e7eef5,0xfc030a11,0x181f262d,0x343b4249,
        0x50575e65,0x6c737a81,0x888f969d,0xa4abb2b9,
        0xc0c7ced5,0xdce3eaf1,0xf8ff060d,0x141b2229,
        0x30373e45,0x4c535a61,0x686f767d,0x848b9299,
        0xa0a7aeb5,0xbcc3cad1,0xd8dfe6ed,0xf4fb0209,
        0x10171e25,0x2c333a41,0x484f565d,0x646b7279
    };
    uint32_t K[4];
    K[0] = ((mainKey[0]>>32) & 0xffffffff) ^ FK[0];
    K[1] = ((mainKey[0]>>0)  & 0xffffffff) ^ FK[1];
    K[2] = ((mainKey[1]>>32) & 0xffffffff) ^ FK[2];
    K[3] = ((mainKey[1]>>0)  & 0xffffffff) ^ FK[3];

    for(round=0;round<32;round++)
    {
        subRoundKey[round]=K[0]^T1(K[1]^K[2]^K[3]^CK[round]);
        K[0]=K[1];
        K[1]=K[2];
        K[2]=K[3];
        K[3]=subRoundKey[round];
    }
}

uint32_t T1(uint32_t input)
{
    uint8_t sbox(uint8_t);
    uint32_t midState;
    midState =  (sbox((input>>24) & 0xff) << 24)|
                (sbox((input>>16) & 0xff) << 16)|
                (sbox((input>>8)  & 0xff) << 8) |
                (sbox((input>>0)  & 0xff) << 0);
    return  midState^
            ROTATE_LEFT(midState,32,13)^
            ROTATE_LEFT(midState,32,23);
}
void process(int state,
             int round,
             uint32_t subRoundKey[32],
             uint64_t plainText[2],
             uint64_t cipherText[2]
            )
{
    uint32_t T(uint32_t);
	uint32_t X[4];
	uint32_t midState;

    //initialize the state
    X[0]=(plainText[0]>>32) & 0xffffffff;
    X[1]=plainText[0] & 0xffffffff;
    X[2]=(plainText[1]>>32) & 0xffffffff;
    X[3]=plainText[1] & 0xffffffff;

    //process
    for(round=0;round<32;round++)
    {
        int j= state==ENCRYPT?round:(31-round);
        midState=X[0]^T(X[1]^X[2]^X[3]^subRoundKey[j]);
        X[0]=X[1];
        X[1]=X[2];
        X[2]=X[3];
        X[3]=midState;
        printf("X[%d] = %#x\n",round,midState);
    }
    cipherText[0]= ((uint64_t)X[3]<<32) | X[2];
    cipherText[1]= ((uint64_t)X[1]<<32) | X[0];
}

uint32_t T(uint32_t input)
{
    uint8_t sbox(uint8_t);
    uint32_t midState;
    midState =  (sbox((input>>24) & 0xff) << 24)|
                (sbox((input>>16) & 0xff) << 16)|
                (sbox((input>>8)  & 0xff) << 8) |
                (sbox((input>>0)  & 0xff) << 0);
    return  midState^
            ROTATE_LEFT(midState,32,2)^
            ROTATE_LEFT(midState,32,10)^
            ROTATE_LEFT(midState,32,18)^
            ROTATE_LEFT(midState,32,24);
            
}

uint8_t sbox(uint8_t input)
{
    return sboxTable[(input>>4) & 0xf][input & 0xf];
}
