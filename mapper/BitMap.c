#include <BitMap.h>
#include <stdio.h>
#include <flog.h>
/*
 *
 *	BitMap dumper
 *
 */
void dump_mapping(U_1024_BYTE * mapper,int size){
	int Bcount,bcount,c;
	printf("Mapping \n 0 1 2 3 4 5 6 7\n");
	c=0;
        for (Bcount=0;Bcount<=size;Bcount++) {
		for (bcount=0;bcount<8;bcount++) {
			//bit=GET_BIT((mapper),(Bcount+bcount));
			printf(" %d",(unsigned char )GET_BIT((mapper),c));
			c++;
		}
	printf("\n");
        }

}

/*
 *
 * Activate Bit numbered from 0 to max.
 *
 */
void set_bit( U_1024_BYTE * ptr, unsigned int bitnumber) {
	unsigned char *byte_ptr=NULL;
	byte_ptr=(unsigned char*) (&ptr->byte[GET_BYTES_INDEX(bitnumber)]);
	*byte_ptr=(*byte_ptr)|ACTIVATE(bitnumber);
	flog(LOG_DEBUG4,"Activating bit %llu\n",bitnumber);
}

/*
 *
 * Dectivate Bit numbered from 0 to max.
 *
 */
//void unset_bit( U_1024_BYTE * ptr, unsigned int bitnumber) {
void unset_bit( U_1024_BYTE * ptr, unsigned int bitnumber) {
	unsigned  char *byte_ptr=NULL;
	byte_ptr=(unsigned char*) (&ptr->byte[GET_BYTES_INDEX(bitnumber)]);
	(*byte_ptr)^=ACTIVATE(bitnumber);
	flog(LOG_DEBUG4,"Activating bit %llu\n",bitnumber);
}

