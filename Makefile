# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ntassin <ntassin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/27 13:09:58 by ntassin           #+#    #+#             #
#    Updated: 2026/04/27 13:09:59 by ntassin          ###   ########.fr       #
#                                                                              #
# **************************************************************************** #

NAME        = fdf
BONUS_NAME  = fdf_bonus

# ─── Compilateur ─────────────────────────────────────────────────────────────

CC          = cc
CFLAGS      = -Wall -Wextra -Werror -Wno-cast-function-type \
              -D BUFFER_SIZE=16384

# ─── Répertoires ─────────────────────────────────────────────────────────────

SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = include
MLX_DIR     = minilibx-linux
LFT_DIR     = libft
GNL_DIR     = GNL
BONUS_DIR   = $(SRC_DIR)/bonus
BONUS_INC   = $(INC_DIR)/bonus
OBJ_BONUS   = obj_bonus

# ─── Bibliothèques ───────────────────────────────────────────────────────────

MLX_LIB     = $(MLX_DIR)/libmlx.a
LFT_LIB     = $(LFT_DIR)/libft.a

INC         = -I$(INC_DIR) -I$(MLX_DIR) -I$(LFT_DIR) -I$(GNL_DIR)
LIBS        = -L$(MLX_DIR) -lmlx -L$(LFT_DIR) -lft -lX11 -lXext -lm

# ─── Sources ─────────────────────────────────────────────────────────────────

FDF_SRCS    = main.c parse.c parse2.c projection.c draw.c draw2.c events.c utils.c
GNL_SRCS    = get_next_line.c get_next_line_utils.c
BONUS_SRCS  = main_bonus.c parse_bonus.c parse2_bonus.c projection_bonus.c \
              draw_bonus.c draw2_bonus.c draw3_bonus.c events_bonus.c \
              mouse_bonus.c keys_bonus.c utils_bonus.c color_bonus.c \
              screenshot_bonus.c perlin_bonus.c fbm_bonus.c biome_bonus.c \
              mapgen_write_bonus.c mapgen_bonus.c

OBJS        = $(addprefix $(OBJ_DIR)/, $(FDF_SRCS:.c=.o)) \
              $(addprefix $(OBJ_DIR)/, $(GNL_SRCS:.c=.o))
BONUS_OBJS  = $(addprefix $(OBJ_BONUS)/, $(BONUS_SRCS:.c=.o)) \
              $(addprefix $(OBJ_BONUS)/, $(GNL_SRCS:.c=.o))

# ─── Couleurs ────────────────────────────────────────────────────────────────

GREEN       = \033[0;32m
YELLOW      = \033[0;33m
CYAN        = \033[0;36m
RESET       = \033[0m
BOLD        = \033[1m

# ─── Règles ──────────────────────────────────────────────────────────────────

all: banner $(MLX_LIB) $(LFT_LIB) $(NAME)

bonus: banner $(MLX_LIB) $(LFT_LIB) $(BONUS_NAME)

banner:
	@printf "\n"
	@python3 -c "\
import random; \
C = ['\x1b[91m', '\x1b[93m', '\x1b[92m', '\x1b[96m', '\x1b[95m']; \
L = [ \
  '  ███████╗██████╗ ███████╗', \
  '  ██╔════╝██╔══██╗██╔════╝', \
  '  █████╗  ██║  ██║█████╗  ', \
  '  ██╔══╝  ██║  ██║██╔══╝  ', \
  '  ██║     ██████╔╝██║     ', \
  '  ╚═╝     ╚═════╝ ╚═╝     ', \
]; \
[print(''.join('\x1b[1m' + random.choice(C) + c + '\x1b[0m' for c in l)) for l in L] \
" 2>/dev/null || printf "$(BOLD)$(CYAN)  === FDF ===\n$(RESET)"
	@printf "\n"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@printf "$(BOLD)$(GREEN)[ OK ]  $(NAME) compilé avec succès\n$(RESET)"

$(BONUS_NAME): $(BONUS_OBJS)
	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBS) -o $(BONUS_NAME)
	@printf "$(BOLD)$(GREEN)[ OK ]  $(BONUS_NAME) compilé avec succès\n$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
	@printf "  $(CYAN)CC$(RESET)    $<\n"

$(OBJ_DIR)/%.o: $(GNL_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
	@printf "  $(CYAN)CC$(RESET)    $<\n"

$(OBJ_BONUS)/%.o: $(BONUS_DIR)/%.c
	@mkdir -p $(OBJ_BONUS)
	@$(CC) $(CFLAGS) -I$(BONUS_INC) -I$(MLX_DIR) -I$(LFT_DIR) -I$(GNL_DIR) -c $< -o $@
	@printf "  $(CYAN)CC$(RESET)    $<\n"

$(OBJ_BONUS)/%.o: $(GNL_DIR)/%.c
	@mkdir -p $(OBJ_BONUS)
	@$(CC) $(CFLAGS) -I$(BONUS_INC) -I$(MLX_DIR) -I$(LFT_DIR) -I$(GNL_DIR) -c $< -o $@
	@printf "  $(CYAN)CC$(RESET)    $<\n"

$(LFT_LIB):
	@printf "$(YELLOW)-->   Compilation libft...\n$(RESET)"
	@$(MAKE) -C $(LFT_DIR) --no-print-directory > /dev/null
	@printf "$(GREEN)[ OK ]  libft prête\n$(RESET)"

$(MLX_LIB):
	@printf "$(YELLOW)-->   Compilation MiniLibX...\n$(RESET)"
	@$(MAKE) -C $(MLX_DIR) --no-print-directory > /dev/null 2>&1
	@printf "$(GREEN)[ OK ]  MiniLibX prête\n$(RESET)"

clean:
	@rm -rf $(OBJ_DIR) $(OBJ_BONUS)
	@$(MAKE) -C $(LFT_DIR) clean --no-print-directory > /dev/null
	@$(MAKE) -C $(MLX_DIR) clean --no-print-directory > /dev/null
	@printf "$(YELLOW)[ RM ]  objets supprimés\n$(RESET)"

fclean: clean
	@rm -f $(NAME) $(BONUS_NAME)
	@rm -f $(MLX_LIB)
	@$(MAKE) -C $(LFT_DIR) fclean --no-print-directory > /dev/null
	@printf "$(YELLOW)[ RM ]  $(NAME) + $(BONUS_NAME) + libs supprimés\n$(RESET)"

re: fclean all

.PHONY: all bonus banner clean fclean re
