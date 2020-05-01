#ifndef POS_H
#define POS_H

typedef struct {
	int x;
	int y;
} res;

//int delta_x(int res_x, int i);
#define delta_x(res_x, i) ((res_x) / ((i) + 1))
int delta_y(int res_y, int p, int p_y);
int padding(res *res, int x, int y);
int init_pos(res *pos, int pos_len, res res, int base,
			 int pisos, int p_y, int pico);
int index2kill(const res *pos, int pos_len, int *index,
			   res from, res to, int base, int p_y);
void index2tachadura(const res *pos, res *tachadura,
					 int x, int y, int margin, int p_y);

#endif
