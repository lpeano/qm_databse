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
void set_bit( U_1024_BYTE * ptr, unsigned long bitnumber) {
	unsigned char *byte_ptr=NULL;
	byte_ptr=((unsigned char*) (ptr))+(bitnumber/8);
	flog(LOG_DEBUG4,"Range is %20p --> %20p",ptr,((char*)ptr+1024*64));
	flog(LOG_DEBUG4,"Activating bit %llu on byte number %lu on bit number %lu at byte pointer %20p:%d original pointer %20p",bitnumber,byte_ptr-(unsigned char*)ptr ,bitnumber%0x8L,byte_ptr,bitnumber%8,ptr);
	flog(LOG_DEBUG4,"Activated prima  %d",(*byte_ptr));
	*byte_ptr|=ACTIVATE(bitnumber);
	flog(LOG_DEBUG4,"Activated bit %llu %d",bitnumber,(*byte_ptr));
	flog(LOG_DEBUG4,"Dumping bitmap after bit number (%lu) [%d %d %d %d %d %d %d %d]",(unsigned long) bitnumber%8,(unsigned char)((BYTE*) (byte_ptr))->BIT0,(unsigned char)((BYTE*) byte_ptr)->BIT1,(unsigned char)((BYTE*) byte_ptr)->BIT2,(unsigned char)((BYTE*) byte_ptr)->BIT3,(unsigned char)((BYTE*) byte_ptr)->BIT4,(unsigned char)((BYTE*) byte_ptr)->BIT5,(unsigned char)((BYTE*) byte_ptr)->BIT6,(unsigned char)((BYTE*) byte_ptr)->BIT7);
}

/*
 *
 * Dectivate Bit numbered from 0 to max.
 *
 */
//void unset_bit( U_1024_BYTE * ptr, unsigned int bitnumber) {
void unset_bit( U_1024_BYTE * ptr, unsigned long bitnumber) {
	unsigned  char *byte_ptr=NULL;
	byte_ptr=((unsigned char*) (ptr))+(bitnumber/8);
	flog(LOG_DEBUG4,"Range is %20p --> %20p",ptr,((char*)ptr+1024*64));
	flog(LOG_DEBUG4,"Deactivated prima  %d pointer is %20p",(*byte_ptr),byte_ptr);
	(*byte_ptr)&=DEACTIVATE(bitnumber);
	flog(LOG_DEBUG4,"Deactivated bit %llu %d",bitnumber,(*byte_ptr));
	flog(LOG_DEBUG4,"Dumping bitmap after bit number %lu %d %d %d %d %d %d %d %d",(unsigned long) bitnumber%8,(unsigned char)((BYTE*) (byte_ptr))->BIT0,(unsigned char)((BYTE*) byte_ptr)->BIT1,(unsigned char)((BYTE*) byte_ptr)->BIT2,(unsigned char)((BYTE*) byte_ptr)->BIT3,(unsigned char)((BYTE*) byte_ptr)->BIT4,(unsigned char)((BYTE*) byte_ptr)->BIT5,(unsigned char)((BYTE*) byte_ptr)->BIT6,(unsigned char)((BYTE*) byte_ptr)->BIT7);
}

unsigned char get_bit(U_1024_BYTE * ptr, unsigned long bitnumber) {
	//unsigned char *byte_ptr=((unsigned char ) ((0x01U)<<(b%8)));
	unsigned char *byte_ptr=((unsigned char *) ptr + (bitnumber/8));
	unsigned char b_value=(unsigned char) (*byte_ptr)>>(bitnumber%8)&(0x01);
	flog(LOG_DEBUG4,"Getting byte %20p for bit %lu subbyte bit is %lu value => %u",byte_ptr,bitnumber,(bitnumber%8),b_value);
	return(b_value); 
}
