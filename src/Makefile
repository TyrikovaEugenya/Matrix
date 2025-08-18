# Название проекта
PROJECT = matrix
# Название теста
TEST = test
# gcc compilator
CC = gcc
# ARFLAGS
ARFLAGS = ar rc
# FLAGS for testing
FLAGS = -std=c11
# Ranlib
RANLIB = ranlib
# CFLAGS
CFLAGS=$(FLAGS) -c
# Флаги для lcov
GCOV_FLAGS =-fprofile-arcs -ftest-coverage
# Дополнительные библиотеки для линковки
LIBS = -lcheck $(LIBS_ADDITIONAL)
# Дополнительные библиотеки для линковки c отчетом о покрытии
LIBS_GCOV = $(LIBS) -lgcov
# Дополнительные библиотеки не для Mac, по умолчанию оставляем пустым, чтобы не портить сборку на Mac
LIBS_ADDITIONAL = 
# Команда для открытия файла отчета о покрытии по умполчанию
CMD_OPEN = open
# Пути до используемых папок
# Директория для объектных файлов
OBJ_DIR = object_dir
OBJECTE = object
OBJJ = obj
# Директория для gcov файлов
GCOV_OBJ_DIR = obj/gcov
# Директория для тестовых файлов
TESTS_OBJ_DIR = obj/tests
# Директория для тестов
TEST_DIR = tests
# Директория для вывода репорта
REPORT_DIR = report
# Цель и название исполняемого файла для тестов
TEST_EXE = matrix_test
# Цель и название исполняемого файла для отчета о покрытии
GCOV_EXE = gcov_report
# Имя статичной библиотеки со всеми модулями проекта
MODULES_STATIC_LIBRARY = $(PROJECT).a
# Имя статичной библиотеки для создания покрытия
MODULES_STATIC_LIBRARY_GCOV = matrix_gcov.a
# clearing object files
OBJ_CLEAR = ./*.o 
# checking for clang
CLANG_CHECK = *.h *.c ./$(TEST_DIR)/*.c ./$(TEST_DIR)/*.h
# clean clang
CLANG_CLEAN = ./.clang-format ./$(TEST_DIR)/.clang-format
# Определяем ОС
ifeq ($(OS), Windows_NT)
    detected_OS := Windows
else
    detected_OS := $(shell uname -s)
endif

ifeq ($(detected_OS), Linux)
	detected_Linux := $(shell cat /etc/issue | sed -n '1p' | awk '{print $$1}')
	# Значение по умолчанию для любых версий Linux
	LIBS_ADDITIONAL = -lm -pthread -lsubunit -lrt -L. -lcheck `pkg-config --cflags --libs check`
	ifeq ($(detected_Linux), Arch)
	LIBS_ADDITIONAL = -lm
	CMD_OPEN = xdg-open
	endif

	ifeq ($(detected_Linux), Ubuntu)
	LIBS_ADDITIONAL = -lm -pthread -lsubunit -lrt -lcheck `pkg-config --cflags --libs check`
	CMD_OPEN = xdg-open
	endif
	
	ifeq ($(detected_Linux), Debian)
	LIBS_ADDITIONAL = -lm -pthread -lsubunit -lrt
	CMD_OPEN = xdg-open
	endif
endif

# Заполняем переменные со списком директорий, исходных и исполняемых файлов
# DIRS = $(shell find -type d)
DIRS = ./ ./tests
C_TYPE = matrix.c
H_TYPE = matrix.h
ALL_O = $(C_TYPE:%.c=%.o)

# Находим необходимые директории и типы для компиляции
TEST_C_TYPE = $(notdir $(shell find $(TEST_DIR) -maxdepth 1 -name "*.c"))
TEST_O_TYPE = $(addprefix $(TESTS_OBJ_DIR)/, $(notdir $(TEST_C_TYPE:%.c=%.o)))

# Finding all modules to create gcov library
ALL_MODULES_GCOV_O = $(addprefix $(GCOV_OBJ_DIR)/, $(notdir $(C_TYPE:.c=.o)))

# Указываем список каталогов, в которых make выполняет поиск
vpath %.c $(DIRS) : $(TEST_DIR)
vpath %.o $(OBJ_DIR)

# Далее идут все цели
all: matrix.a
# matrix_test
test: $(OBJ_DIR) $(TESTS_OBJ_DIR) $(MODULES_STATIC_LIBRARY) $(TEST_O_TYPE)
	@$(CC) $(TEST_O_TYPE) $(MODULES_STATIC_LIBRARY) $(LIBS) -o $(TEST_EXE)
	@./$(TEST_EXE)
# gcov_report
gcov_report: test $(GCOV_OBJ_DIR) $(REPORT_DIR) $(MODULES_STATIC_LIBRARY_GCOV) $(ALL_H_FILES)
	@$(CC) $(TEST_O_TYPE) $(MODULES_STATIC_LIBRARY_GCOV) $(LIBS_GCOV) -o ./$(GCOV_EXE)
	@./$(GCOV_EXE)
	@gcov -o $(GCOV_OBJ_DIR) $(C_TYPE)
	@lcov -t "matrix_test" --output-file rep.info --capture --directory .
	@genhtml -q --output-directory report rep.info
	@$(CMD_OPEN) ./report/index.html

# matrix.a
matrix.a: $(OBJ_DIR) $(TESTS_OBJ_DIR) $(ALL_O) $(ALL_H_FILES)
	@$(ARFLAGS) $(MODULES_STATIC_LIBRARY) $(addprefix $(OBJ_DIR)/, $(ALL_O))
	@$(RANLIB) $(MODULES_STATIC_LIBRARY)
# matrix_gcov.a
$(MODULES_STATIC_LIBRARY_GCOV): $(GCOV_OBJ_DIR) $(TESTS_OBJ_DIR) $(ALL_MODULES_GCOV_O) $(ALL_H_FILES)
	@$(ARFLAGS) $(MODULES_STATIC_LIBRARY_GCOV) $(ALL_MODULES_GCOV_O)
	@$(RANLIB) $(MODULES_STATIC_LIBRARY_GCOV)
# running classic objects file
%.o: %.c $(ALL_H_FILES) $(TEST_H_TYPE)
	@$(CC) $(CFLAGS) -o $(addprefix $(OBJ_DIR)/, $@) $<
# running object files in obj/gcov
$(GCOV_OBJ_DIR)/%.o : %.c $(ALL_H_FILES) $(TEST_H_TYPE)
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $@ $<
# running object files in obj/tests
$(TESTS_OBJ_DIR)/%.o : %.c $(ALL_H_FILES) $(TEST_H_TYPE)
	@$(CC) $(CFLAGS) -o $@ $<
# report
$(REPORT_DIR):
	@mkdir -p $(REPORT_DIR)
# object_dir
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
# obj/gcov
$(GCOV_OBJ_DIR):
	@mkdir -p $(GCOV_OBJ_DIR)
# obj/tests
$(TESTS_OBJ_DIR):
	@mkdir -p $(TESTS_OBJ_DIR)
# Очистка директории
clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(REPORT_DIR)
	rm -f *.a *.o
	rm -f *.c.gcov *.gcno *.gcov *.gcda *.info
	rm -f ./$(GCOV_EXE) ./$(TEST_EXE) 
	rm -rf $(OBJECTE) 
	rm -rf $(OBJJ)
	rm -rf ./$(TEST)
# Перестройка проекта
rebuild: clean all
# Проверить стиль во всех директориях
check:
	cp ../materials/linters/.clang-format . 
	cp ../materials/linters/.clang-format ./$(TEST_DIR) 
	clang-format -style=Google -n *.c
	clang-format -style=Google -n *.h
	clang-format -style=Google -n ./$(TEST_DIR)/*.c
	rm $(CLANG_CLEAN)
# Исправить стиль во всех директориях
clang:
	cp ./../materials/linters/.clang-format ./
	cp ./../materials/linters/.clang-format ./$(TEST_DIR)
	clang-format -style=Google -i ./*.c
	clang-format -style=Google -i ./*.h
	clang-format -style=Google -i ./$(TEST_DIR)/*.c
	rm .clang-format
	rm ./$(TEST_DIR)/.clang-format
# Проверить на утечки на Линукс
leaks_check:
	leaks -atExit -- ./$(TEST_EXE) | grep leaks:
# Линтер
cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(CLANG_CHECK)
# Проверка на утечки
valgrind_check:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all -s ./$(TEST_EXE)

.PHONY: all clean rebuild cppcheck leaks valgrind
