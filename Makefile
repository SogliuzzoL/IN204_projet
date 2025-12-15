EXEC = bin/game

CXX = g++
EXT = cpp
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Iinclude $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net -lSDL2_gfx

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

SRCS = $(shell find $(SRC_DIR) -name "*.$(EXT)")
OBJS = $(patsubst $(SRC_DIR)/%.$(EXT), $(OBJ_DIR)/%.o, $(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS)
	@echo "Édition des liens..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Succès ! Exécutable : $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXT)
	@echo "Compilation de $<"
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Nettoyage..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean