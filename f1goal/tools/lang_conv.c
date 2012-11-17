
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "f1g_basic_funcs.h"
#include "f1g_lang_code.h"

int main(int argc, char *argv[])
{
	int fd;
	char * pb, *pe;
	char * rd_buf = NULL;
	struct stat st;
	u16_t unicode = 0x0000;
	u32_t utf8 = 0x00;
	i32_t utf8_len = 0;
	i32_t line_len = 0;
	
	if (argc < 2) {
		return 1;
	}

	fd = open(argv[1], O_RDONLY, 0);
	if (fd < 0) {
		return 0;
	}
	
	if (0 != fstat(fd, &st)) {
		close(fd);
		return 1;
	}
	if (st.st_size <= 0) {
		close(fd);
		return 1;
	}
	
	rd_buf = (char*)malloc(st.st_size+1);
	if (NULL == rd_buf) {
		close(fd);
		return 1;
	}

	if (read(fd, rd_buf, st.st_size) != st.st_size) {
		close(fd);
		return 1;
	}
	rd_buf[st.st_size] = '\0';

	pb = strtok(rd_buf, "\n");
	while (NULL != pb) {
		line_len = strlen(pb);
		char *pf;
		pe = strtok(pb, "\t");
		pf = strtok(NULL, "\t");
		unicode = strtoul(pf, NULL, 16);
		utf8_len = sizeof(utf8);
		unicode_to_utf8(&unicode, 1, (u8_p)&utf8, &utf8_len);
		//fprintf(stdout, "%s %s 0x%08x[%d]\n", pe, pf, utf8,	utf8_len);
		fprintf(stdout, "%s %s 0x%08x\n", pe, pf, utf8);
		pb = pb + line_len + 1;
		pb = strtok(pb, "\n");
	}

	if (rd_buf) {
		free(rd_buf);
		rd_buf = NULL;
	}

	if (fd > 0) {
		close(fd);
	}

	return 0;
}

