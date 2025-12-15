EXEC = bin/game

CXX = g++
EXT = cpp
CXXFLAGS = -Wall -Wextra -std=c++17 -g -Iinclude $(shell sdl2-config --cflags)

LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net -lSDL2_gfx

SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.$(EXT))
OBJS = $(patsubst $(SRC_DIR)/%.$(EXT), $(OBJ_DIR)/%.o, $(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS)
	@echo "Création de l'exécutable..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Succès ! Exécutable disponible ici : $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(EXT)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Nettoyage des fichiers de build..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean