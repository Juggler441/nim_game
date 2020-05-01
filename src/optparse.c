#include <stdio.h> // fopen, feof, fgets, fclose
#include <ctype.h> // isdigit
#include <stdlib.h> // atoi
#define OPT_FILE "options.txt"
#define MAX_OPT_LEN 64

int strip(char *line) { /*
	elimina todos los caracteres que no sean digitos
	retorna la nueva longitud de line */
	char *line2 = line;
	char *start = line;
	do {
		if (isdigit(*line)) {
			*line2 = *line;
			line2++;
		}
	} while (*++line);
	*line2 = '\0';
	//line = start;
	return line2 - start;
}

int assign_all(int *val, int val_len) { /*
	retorna 0 en caso de error */
	FILE *f;
	char opt[MAX_OPT_LEN];
	if (! (f = fopen(OPT_FILE, "rt")))
		return 0;
	while (val_len --> 0) {
		if (feof(f) || ! fgets(opt, MAX_OPT_LEN, f)) {
			fclose(f);
			return 0;
		}
		if(! *opt) return 0;
		strip(opt);
		*val++ = atoi(opt);
	}
	fclose(f);
	return 1;
}
