#include <stdio.h> // printf, fprintf, snprintf
#include <stdlib.h> // malloc, free
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <all.h>

#define true 1
#define false 0
#define OPT_NUM 20
#define LOSER_MSG_LEN 20

int inverted;
res padded_res;

#ifdef LOG_MOVES
	void print_board(int *board, int board_len) {
		while (board_len --> 0)
			printf("%d, ", *board++);
		printf("\n");
	}
#endif

static void draw_line(float x1, float y1, float x2, float y2,
					  ALLEGRO_COLOR color, float thickness) {
	if (inverted) {
		y1 = padded_res.y - y1;
		y2 = padded_res.y - y2;
	}
	al_draw_line(x1, y1, x2, y2, color, thickness);
}

static void draw_piramid(const res *pos, int pos_len, ALLEGRO_COLOR back_color,
						 ALLEGRO_COLOR front_color, float line_thick, int p_y) {
	int i;
	float x, y;
	//al_reset_clipping_rectangle();
	al_clear_to_color(back_color);
	for (i = 0; i < pos_len; i++) {
		x = (float) pos[i].x;
		y = (float) pos[i].y;
		draw_line(x, y, x, y - p_y, front_color, line_thick);
	}
	//al_flip_display();
}

static int draw_all_dead(const stick *sticks, int sticks_len, const res *pos,
						 int margin, int p_y, ALLEGRO_COLOR front_color, float line_thick) { /*
	retorna false en caso de error (stick *sticks esta roto) */
	int x, y, i;
	res tachadura[2];
	for (i = 0; i < sticks_len; i++) {
		if (sticks[i].alive)
			continue;
		x = i;
		while (sticks[i].connect_to != 0) {
			i++;
			if (i >= sticks_len)
				return false;
		}
		y = i;
		index2tachadura(pos, tachadura, x, y, margin, p_y);
		draw_line((float) tachadura[0].x, (float) tachadura[0].y, (float)
		tachadura[1].x, (float) tachadura[1].y, front_color, line_thick);
	}
	//al_flip_display();
	return true;
}

static int is_ai_turn(int turn, int n_ai, int ai_turn) {
	if (n_ai) { // si es mayor que cero
		if (turn >= ai_turn && turn < ai_turn + n_ai)
			return true;
	}
	return false;
}

int main(int argc, char **argv) {
	int n_sticks, *board, board_len, index[2] = {0, 0};
	int p_y, base, pico, pisos, margin, num_players, turn, n_ai, ai_turn;
	int fps, redraw = true, loop = true, opt_val[OPT_NUM], i = 0, perfect_ai;
	float line_thick;
	unsigned char front_color_r, front_color_g, front_color_b;
	unsigned char back_color_r, back_color_g, back_color_b;
	char loser_msg[LOSER_MSG_LEN];
	stick *sticks;
	res *pos;
	res mouse_up, mouse_down;
	ALLEGRO_DISPLAY *scr;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_COLOR front_color, back_color;
	ALLEGRO_EVENT ev;
	
	if ((! assign_all(opt_val, OPT_NUM)) || (! (opt_val[15] >= opt_val[18])) ||
		(opt_val[19] && opt_val[15] != 2)) {
			// (! (num_players >= ai_turn))
		// (perfect_ai && num_players != 2)
		#ifdef USE_STDERR
		fprintf(stderr, "bad options file, using defaults\n");
		#endif
		
		// inicializar variables para inicializar allegro
		padded_res.x = 640;
		padded_res.y = 480; // 480p
		fps = 2; // only to restart the full graphics
		
		// inicializar variables para la gui
		front_color_r = 238;
		front_color_g = 237;
		front_color_b = 240;
		back_color_r = 0;
		back_color_g = 36;
		back_color_b = 86; // blue powershell
		margin = 10;
		line_thick = 2;
		p_y = 30;
		
		// inicializar las variables para el juego
		base = 8;
		pico = true;
		inverted = false;
		num_players = 3;
		turn = 0; // el numero del primer turno
		n_ai = 2; // numero de jugadores AI
		ai_turn = 1; // el turno de la primera AI
		perfect_ai = 0; // usar el argoritmo de juego perfecto
	} else {
		// inicializar variables para inicializar allegro
		padded_res.x = opt_val[i++];
		padded_res.y = opt_val[i++];
		fps = opt_val[i++];
		
		// inicializar variables para la gui
		front_color_r = opt_val[i++];
		front_color_g = opt_val[i++];
		front_color_b = opt_val[i++];
		back_color_r = opt_val[i++];
		back_color_g = opt_val[i++];
		back_color_b = opt_val[i++];
		margin = opt_val[i++];
		line_thick = opt_val[i++];
		p_y = opt_val[i++];
		
		// inicializar las variables para el juego
		base = opt_val[i++];
		pico = opt_val[i++];
		inverted = opt_val[i++];
		num_players = opt_val[i++];
		turn = opt_val[i++];
		n_ai = opt_val[i++];
		ai_turn = opt_val[i++];
		perfect_ai = opt_val[i++];
	}
	
	// inicializar allegro
	if (! al_init())
		return -1;
	if (! al_init_primitives_addon())
		return -1;
	if (! al_install_mouse())
		return -1;
	if (! (timer = al_create_timer((double) 1.0 / (double) fps)))
		return -1;
	if (! (scr = al_create_display(padded_res.x, padded_res.y))) {
		al_destroy_timer(timer);
		return -1;
	}
	if (! (event_queue = al_create_event_queue())) {
		al_destroy_display(scr);
		al_destroy_timer(timer);
		return -1;
	}
	al_register_event_source(event_queue, al_get_display_event_source(scr));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	
	back_color = al_map_rgb(back_color_r, back_color_g, back_color_b);
	front_color = al_map_rgb(front_color_r,front_color_g, front_color_b);
	
	// inicializar el juego
	n_sticks = number_of_sticks(base, pico);
	pisos = base2pisos(base, pico);
	
	if (! (board = (int *) malloc(max_len_board(base, n_sticks) * sizeof *board)))
		return -1;
	if (! (sticks = (stick *) malloc(n_sticks * sizeof *sticks)))
		return -1;
	if (! (pos = (res *) malloc(n_sticks * sizeof *pos)))
		return -1;
	board_len = init_board(board, base, n_sticks, pico);
	init_sticks(sticks, n_sticks, base);
	if (! init_pos(pos, n_sticks, padded_res, base, pisos, p_y, pico))
		return -1;
	
	// game loop
	al_start_timer(timer);
	draw_piramid(pos, n_sticks, back_color, front_color, line_thick, p_y);
	al_flip_display();
	
	while (loop) {
		al_wait_for_event(event_queue, &ev);
		
		if (ev.type == ALLEGRO_EVENT_TIMER)
			redraw = true;
		
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			break;
		
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			if (inverted) {
				mouse_down.x = ev.mouse.x;
				mouse_down.y = padded_res.y - ev.mouse.y;
			} else {
				mouse_down.x = ev.mouse.x;
				mouse_down.y = ev.mouse.y;
			}
			redraw = false;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			if (0 < ev.mouse.x && ev.mouse.x < padded_res.x &&
				0 < ev.mouse.y && ev.mouse.y < padded_res.y) {
				if (inverted) {
					mouse_up.x = ev.mouse.x;
					mouse_up.y = padded_res.y - ev.mouse.y;
				} else {
					mouse_up.x = ev.mouse.x;
					mouse_up.y = ev.mouse.y;
				}
			} else continue;
			do {
				if (is_ai_turn(turn, n_ai, ai_turn)) {
					if (! chboard2index(AI_move(board, board_len, turn,
					num_players, perfect_ai), index, sticks, n_sticks)) {
						#ifdef USE_STDERR
						fprintf(stderr, "chboard2index() failed\n");
						#endif
						loop = false;
						break;
					}
				} else {
					if (! index2kill(pos, n_sticks, index,
					mouse_down, mouse_up, base, p_y)) {
						// fprintf(stderr, "index2kill() failed\n");
						break;
					}
				}
				// define LOG_MOVES en gcc para que tambien aparezca en AI.c
				#ifdef LOG_MOVES
					printf("\nplayer %d", turn);
					if (is_ai_turn(turn, n_ai, ai_turn))
						printf(" (AI)");
					printf("\n");
					// printf("before update_board()\n");
					// print_board(board, board_len);
					printf("index[0] = %d\n"
						   "index[1] = %d\n", index[0], index[1]);
				#endif
				if (! error_check(sticks, index[0], index[1])) {
					// fprintf(stderr, "error_check() failed\n");
					break;
				}
				kill(sticks, index[0], index[1]);
				if (! (board_len = update_board(board,
				board_len, sticks, index[0], index[1]))) {
					#ifdef USE_STDERR
					fprintf(stderr, "update_board() failed\n");
					#endif
					loop = false;
					break;
				}
				#ifdef LOG_MOVES
					// printf("after update_board()\n");
					print_board(board, board_len);
				#endif
				if (win(board, board_len)) {
					snprintf(loser_msg,  LOSER_MSG_LEN, "player %d lost!\n",
					loser(board, board_len, turn, num_players));
					al_show_native_message_box(NULL, "nim game", "game over",
					loser_msg, NULL, ALLEGRO_MESSAGEBOX_WARN);
					loop = false;
					break;
				}
				
				turn = next_turn(turn, num_players);
			} while (is_ai_turn(turn, n_ai, ai_turn));
			redraw = true;
		}
		
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			draw_piramid(pos, n_sticks, back_color, front_color, line_thick, p_y);
			if (! draw_all_dead(sticks, n_sticks, pos, margin,
								p_y, front_color, line_thick)) {
				#ifdef USE_STDERR
				fprintf(stderr, "draw_all_dead() failed\n");
				#endif
				break;
			}
			al_flip_display();
		}
	}
	
	// salir
	al_destroy_timer(timer);
	al_destroy_display(scr);
	al_destroy_event_queue(event_queue);
	free(board);
	free(sticks);
	free(pos);
	return 0;
}
