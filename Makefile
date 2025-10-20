# =========================
# COMPILADOR
# =========================
CC = gcc

# =========================
# DETECTAR SISTEMA
# =========================
UNAME_S := $(shell uname -s)

# =========================
# DIRETÓRIOS
# =========================
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

# =========================
# FLAGS DE COMPILAÇÃO
# =========================
CFLAGS = -Wall -Wextra -std=c99 \
	-Iinclude \
	-Iinclude/SDL2 \
	-Isrc

# =========================
# TARGET E LDFLAGS
# =========================
ifeq ($(UNAME_S),Linux)
    TARGET = $(BIN_DIR)/engine
    LDFLAGS = -lSDL2 -lSDL2_ttf
else ifeq ($(UNAME_S),Darwin)
    TARGET = $(BIN_DIR)/engine
    CFLAGS += -I/usr/local/include/SDL2 -I/opt/homebrew/include/SDL2
    LDFLAGS = -L/usr/local/lib -L/opt/homebrew/lib -lSDL2 -lSDL2_ttf
else
    TARGET = $(BIN_DIR)/engine.exe
    LDFLAGS = -Llib -lSDL2 -lSDL2_ttf
endif

# =========================
# FONTES DO PROJETO
# =========================
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/logs/log.c \
	   $(SRC_DIR)/initSDL.c \
	   $(SRC_DIR)/interface/ui.c \
	   $(SRC_DIR)/interface/button/button.c \
	   $(SRC_DIR)/interface/text/text.c 

OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# =========================
# REGRAS PRINCIPAIS
# =========================
all: download_dlls copy_dlls $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

# =========================
# DOWNLOAD DE DLLs (Windows)
# =========================
download_dlls: | $(LIB_DIR)
	@echo "🔽 Baixando SDL2 DLLs..."
	@if [ ! -f "$(LIB_DIR)/SDL2.dll" ]; then \
		curl -L https://github.com/libsdl-org/SDL/releases/download/release-2.28.5/SDL2-2.28.5-win32-x64.zip -o sdl2.zip; \
		unzip -j sdl2.zip "*.dll" -d $(LIB_DIR)/; \
		rm sdl2.zip; \
	fi
	@if [ ! -f "$(LIB_DIR)/SDL2_ttf.dll" ]; then \
		curl -L https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.2/SDL2_ttf-2.20.2-win32-x64.zip -o sdl2_ttf.zip; \
		unzip -j sdl2_ttf.zip "*.dll" -d $(LIB_DIR)/; \
		rm sdl2_ttf.zip; \
	fi
	@echo "✅ DLLs baixadas"

copy_dlls: | $(BIN_DIR)
	@echo "📋 Copiando DLLs..."
	@for dll in $(LIB_DIR)/*.dll; do \
		cp -f $$dll $(BIN_DIR)/; \
	done 2>/dev/null || true
	@echo "✅ DLLs copiadas"

# =========================
# LIMPEZA
# =========================
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR) engine.log 2>/dev/null || true
	@echo "🧹 Limpeza concluída"

clean-all: clean
	@rm -rf $(LIB_DIR) 2>/dev/null || true

# =========================
# BUILD E RUN
# =========================
compile: $(TARGET)

run: $(TARGET)
ifeq ($(UNAME_S),Linux)
	@./$(TARGET)
else ifeq ($(UNAME_S),Darwin)
	@./$(TARGET)
else
	@.\\$(TARGET)
endif

rebuild: clean all run

# =========================
# PHONY
# =========================
.PHONY: all clean clean-all run rebuild copy_dlls download_dlls compile