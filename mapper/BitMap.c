#include <BitMap.h>
#include <stdio.h>
/*
 *
 *	BitMap dumper
 *
 */
void dump_mapping(U_64_BYTE * mapper,int size){
	int Bcount,bcount,c;
	printf("Mapping \n 0 1 2 3 4 5 6 7\n");
	c=0;
        for (Bcount=0;Bcount<=size;Bcount++) {
		for (bcount=7;bcount>=0;bcount--) {
			//bit=GET_BIT((mapper),(Bcount+bcount));
			printf(" %d",(unsigned char )GET_BIT((mapper),c));
			c++;
		}
	printf("\n");
        }

}

