
#ifndef __F1G_BASIC_TYPES_H
#define __F1G_BASIC_TYPES_H

enum {
	F1G_OK = 0x00,
	F1G_ERR
};

enum {
	F1G_SUCCESS = 0x00,
	F1G_FAIL
};

enum {
	F1G_U8 = 0x00,
	F1G_I8,
	F1G_U16,
	F1G_I16,
	F1G_U32,
	F1G_I32,
	F1G_U64,
	F1G_I64,
	F1G_STRING
};

typedef unsigned char 			u8_t;
typedef char 					i8_t;
typedef unsigned char * 		u8_p;
typedef char * 					i8_p;
typedef unsigned short 			u16_t;
typedef short 					i16_t;
typedef unsigned short * 		u16_p;
typedef short * 				i16_p;
typedef unsigned int 			u32_t;
typedef int 					i32_t;
typedef unsigned int * 			u32_p;
typedef int * 					i32_p;
typedef unsigned long long 		u64_t;
typedef long long 				i64_t;
typedef unsigned long long * 	u64_p;
typedef long long * 			i64_p;
typedef char * 					string_t;

#endif
