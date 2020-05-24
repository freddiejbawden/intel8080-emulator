# CC = clang

EXE = intel8080
EXETEST = 8080testrunner

SRC_DIR = src
OBJ_DIR = obj
MAINS_DIR = main
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS += -Wall
CPPFLAGS += -Iinclude
LDFLAGS=-lSDL2 -lsdl2_mixer

.PHONY: all clean debug

all: $(EXE) rom/invaders
emu: $(EXE)


test: $(EXETEST)

$(EXE): $(OBJ) $(OBJ_DIR)/main.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(EXETEST): $(OBJ) $(OBJ_DIR)/test.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@


$(OBJ_DIR)/main.o: $(SRC_DIR)/$(MAINS_DIR)/main.c
	$(CC) $(DEBUG) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/test.o: $(SRC_DIR)/$(MAINS_DIR)/test.c
	$(CC) $(DEBUG) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(DEBUG) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# debug: DEBUG = -DDEBUG
debug: DEBUG = -g -O0

debug: all

rom/invaders:
	bash ./createRom.sh


clean:
	$(RM) $(OBJ)
