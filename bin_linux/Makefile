SRC_DIR = ../src/
INCLUDE_DIR = ../src/include/
CC = gcc
#CFLAGS = -g -D LOG_MOVES -D USE_STDERR -Wall -I$(INCLUDE_DIR)
CFLAGS = -Wall -O2 -I$(INCLUDE_DIR)

play_nim: $(SRC_DIR)gui_main.c game_logic.o helpers.o pos.o sticks.o optparse.o AI.o update_board.o
	$(CC) $(CFLAGS) $(SRC_DIR)gui_main.c game_logic.o helpers.o pos.o sticks.o optparse.o AI.o update_board.o `pkg-config --cflags --libs allegro_dialog-5 allegro_primitives-5 allegro_main-5 allegro-5` -o $@

restart:
	touch $(SRC_DIR)*
	touch $(INCLUDE_DIR)*

game_logic.o: $(SRC_DIR)game_logic.c $(INCLUDE_DIR)game_logic.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)game_logic.c

helpers.o: $(SRC_DIR)helpers.c $(INCLUDE_DIR)helpers.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)helpers.c

pos.o: $(SRC_DIR)pos.c $(INCLUDE_DIR)pos.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)pos.c

sticks.o: $(SRC_DIR)sticks.c $(INCLUDE_DIR)sticks.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)sticks.c

optparse.o: $(SRC_DIR)optparse.c $(INCLUDE_DIR)optparse.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)optparse.c

AI.o: $(SRC_DIR)AI.c $(INCLUDE_DIR)AI.h game_logic.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)AI.c

update_board.o: $(SRC_DIR)update_board.c $(INCLUDE_DIR)update_board.h game_logic.o sticks.o
	$(CC) $(CFLAGS) -c $(SRC_DIR)update_board.c
