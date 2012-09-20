
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "f1g_basic_funcs.h"

void hex_print(FILE* fd, u8_p data, u32_t data_len)
{
	u32_t i,j,line;

	line = data_len / 16;
	if (data_len%16 != 0) {
		line += 1;
	}

	for (i=0; i<line; i++) {
		fprintf(fd, "%08x    ", i*16);
		for (j=i*16; j<data_len && j<(i+1)*16;j++) {
			fprintf(fd, "%02x ", (unsigned char)(data[j]));
		}
		for (; j<(i+1)*16; j++) {
			fprintf(fd, "   ");
		}
		fprintf(fd, "    ");

		fprintf(fd, "|");
		for (j=i*16; j<data_len && j<(i+1)*16;j++) {
			if (data[j]<=0x20 || data[j]>=0x7F) {
				fprintf(fd, ".");
			} else {
				fprintf(fd, "%c", data[j]);
			}
		}
		for (; j<(i+1)*16; j++) {
			fprintf(fd, " ");
		}
		fprintf(fd, "|");
		fprintf(fd, "\n");
	}
}
