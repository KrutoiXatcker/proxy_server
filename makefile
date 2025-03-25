# Компилятор и флаги
CC = g++ -Wall -g

# Имя целевого исполняемого файла
TARGET = proxy_server

# Директории
PREF_SRC = ./src/
PREF_OBJ = ./obj/
PREF_BIN = ./bin/
PREF_INC = ./include/

# Поиск исходных файлов и заголовков
SRC = $(wildcard $(PREF_SRC)*.cpp)
OBJ = $(patsubst $(PREF_SRC)%.cpp,$(PREF_OBJ)%.o,$(SRC))
DEPS = $(wildcard $(PREF_INC)*.h)

# Флаги компилятора
CFLAGS = -I$(PREF_INC)

# Основная цель
$(PREF_BIN)$(TARGET): $(OBJ)
	@mkdir -p $(PREF_BIN)
	$(CC) $(OBJ) -o $@

# Правило для объектных файлов
$(PREF_OBJ)%.o: $(PREF_SRC)%.cpp $(DEPS)
	@mkdir -p $(PREF_OBJ)
	$(CC) -c $< $(CFLAGS) -o $@

# Очистка
clean:
	rm -rf $(PREF_OBJ)*.o $(PREF_BIN)$(TARGET)

# Пересборка
rebuild: clean $(PREF_BIN)$(TARGET)

.PHONY: clean rebuild