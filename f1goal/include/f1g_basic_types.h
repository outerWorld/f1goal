
#ifndef __F1G_BASIC_TYPES_H
#define __F1G_BASIC_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

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

typedef struct _value_s {
	i32_t	val_size;
	i32_t	val_len;
	i8_p	val;
}value_t, *value_p;

//
#define IP_STR_LEN		16
#define FL_NAME_SIZE	128
#define HOST_SIZE		128
#define HOST_PATH_SIZE	256

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
