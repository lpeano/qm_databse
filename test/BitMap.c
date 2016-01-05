#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <BitMap.h>
typedef struct {
	unsigned long slide1; 
	unsigned long slide2;
}Bitmap;	
/*
typedef struct bit {
	unsigned char BIT0:1;
	unsigned char BIT1:1;
	unsigned char BIT2:1;
	unsigned char BIT3:1;
	unsigned char BIT4:1;
	unsigned char BIT5:1;
	unsigned char BIT6:1;
	unsigned char BIT7:1;
}BYTE;

typedef struct u_64_byte {
	BYTE byte[64];
} U_64_BYTE;

void dump_mapping(U_64_BYTE * mapper,int size){
	int Bcount,bcount;
	unsigned char *bita,bit;
	printf("Mapping \n 0 1 2 3 4 5 6 7\n");
        for (Bcount=0;Bcount<=size;Bcount++) {
		bita=(unsigned char *) (&mapper->byte[Bcount]);
		bit=*bita;
		for (bcount=7;bcount>=0;bcount--) {
			printf(" %d ",((*bita)>>bcount)&0x01);
		}
	printf(" %20.0f%20.0fKB%20.0fMB%20.0fGB%20.0fTB%20.0fPB\n",(exp2((Bcount+1)*8)-1),(exp2((Bcount+1)*8)-1)/1024,(exp2((Bcount+1)*8)-1)/1024/1024,(exp2((Bcount+1)*8)-1)/1024/1024/1024,(exp2((Bcount+1)*8)-1)/1024/1024/1024/1024,(exp2((Bcount+1)*8)-1)/1024/1024/1024/1024/1024);
	//printf(" %d %d %d %d %d %d %d %d\n",mapper->byte[Bcount].BIT0,mapper->byte[Bcount].BIT1,mapper->byte[Bcount].BIT2,mapper->byte[Bcount].BIT3,mapper->byte[Bcount].BIT4,mapper->byte[Bcount].BIT5,mapper->byte[Bcount].BIT6,mapper->byte[Bcount].BIT7);
        }

}*/
int main(int c, char *argv[]) {
	Bitmap bmprova;
	unsigned int bit_activation=0;
	U_64_BYTE *mapper=(U_64_BYTE *)NULL;
	
	bmprova.slide1=atol(argv[1]);
	bit_activation=atoi(argv[2]);
	bmprova.slide2=(unsigned long)0;
	printf("DATA %lu\n",(unsigned long) bmprova.slide1);
	mapper= (U_64_BYTE * ) &bmprova;

	printf("Mapping\n");
	printf("Size of BYTE :%d\n",(int)sizeof(BYTE));
	printf("Size of U_64_BYTE :%d\n",(int)sizeof(U_64_BYTE));
	dump_mapping(mapper,sizeof(bmprova.slide1));
	set_bit(mapper,bit_activation);
	getchar();
	dump_mapping(mapper,sizeof(bmprova.slide1));
	printf("%lu\n",(unsigned long) bmprova.slide1);
	printf("Long Long size :%llu %llu %llu %llu\n",(unsigned long long)sizeof(long),(unsigned long long)sizeof(long),(unsigned long long)exp2(sizeof(long)*8),(unsigned long long)exp2(sizeof(long long)*8));
	printf("Byte %llu\n",(unsigned long long)exp2(8));
	printf("Byte %llu\n",(unsigned long long)exp2(8*2));
	printf("Byte %llu\n",(unsigned long long)exp2(8*4));
	printf("Byte %llu\n",(unsigned long long)exp2(8*8));
	

	return(0);
}
