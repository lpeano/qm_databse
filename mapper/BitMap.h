#ifndef _BIT_MAPPER_STRUCTS
#define _BIT_MAPPER_STRUCTS
/*
 *
 * Byte bitmap filter
 *
 */
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


/*
 *
 * 64 byte bit mapper
 *
 */

typedef struct u_64_byte {
	BYTE byte[64];
} U_64_BYTE;

#ifndef _BIT_MAPPER_MACROS
#define _BIT_MAPPER_MACROS

/*
 * 
 * 	Get_Bitmap_Value
 *
 */
#define ACTIVATE(__N) 	(unsigned char ) ((0x01)<<(__N%8))
#define GET_BYTES_INDEX(__x) (__x<8)?0: (__x/0x08) 
#define GET_BIT(__ptr,__n) (unsigned char ) *((unsigned char*) (&(__ptr->byte[GET_BYTES_INDEX(__n)])))>>(__n%8)&(0x01)
//#define SET_BIT(_prt,__n) (_ptr->byte[GET_BYTES_INDEX(__n)])+=(ACTIVATE(__n%8))


#endif

#endif

extern void dump_mapping(U_64_BYTE * ,int );
extern void set_bit( U_64_BYTE * , unsigned int);


