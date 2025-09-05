NAME := miniaudio.so

OBJ_DIR := ./obj/
INCLUDE_DIRS := ./includes/ ../includes/
CLASSE_HEADERS := 

INCLUDE_DIRS := $(addprefix -I, $(INCLUDE_DIRS))
CLASSE_HEADERS := $(addprefix -I./src/, $(CLASSE_HEADERS))

MAKE := @make --no-print-directory

SOURCE_DIR := ./src/

OBJ_DIR := ./obj/

OBJECTS = $(SOURCES:.cpp=.o)

CPP_FILES :=	main \

CPP_FILES := $(addsuffix .cpp, $(CPP_FILES))

SOURCES := $(addprefix $(SOURCE_DIR), $(CPP_FILES))

OBJECTS := $(addprefix $(OBJ_DIR), $(CPP_FILES:.cpp=.o))
DEPS := $(addprefix $(OBJ_DIR), $(CPP_FILES:.cpp=.d))

CFLAGS = -MP -MMD -g -Wall -Wextra -Werror -fPIC

all: miniaudio $(NAME)

miniaudio:
	@if [ -f "includes/miniaudio.h" ]; then \
		echo "\033[32;1;4mminiaudio Found\033[0m"; \
	else \
		echo "\033[31;1;4mminiaudio Not Found\033[0m"; \
		echo "\033[33;1mDownloading miniaudio\033[0m"; \
		curl https://raw.githubusercontent.com/mackron/miniaudio/master/miniaudio.h > includes/miniaudio.h;\
	fi

$(OBJECTS): $(OBJ_DIR)%.o : $(SOURCE_DIR)%.cpp
	@echo $<
	@c++ $(CFLAGS) $(CLASSE_HEADERS) $(INCLUDE_DIRS) -c $< -o $@

$(OBJ_DIR):
	@(cd $(SOURCE_DIR) && find . -type d -exec mkdir -p -- $(shell pwd)/$(OBJ_DIR){} \;)

$(NAME): $(OBJ_DIR) $(OBJECTS)
	@c++ $(OBJECTS) -o $(NAME) -shared
	@echo "\033[0;32mCompiled $(NAME)\033[0m"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[0;32mCleaned objects\033[0m"

fclean:
	$(MAKE) clean
	@rm -f $(NAME)
	@echo "\033[0;32mCleaned $(NAME)\033[0m"

re: fclean all

.PHONY: all clean fclean dclean re miniaudio 

-include $(DEPS)
