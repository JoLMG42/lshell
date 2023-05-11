# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lguillau <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/14 14:27:36 by lguillau          #+#    #+#              #
#    Updated: 2023/05/11 16:57:23 by jtaravel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES	=	srcs/parsing/add_sapce_dol.c\
		srcs/parsing/add_spaces2.c \
		srcs/parsing/add_spaces.c \
		srcs/builtins/builtins.c \
		srcs/builtins/cd2.c \
		srcs/builtins/cd.c \
		srcs/parsing/check_syntax_func.c \
		srcs/builtins/echo_2.c \
		srcs/builtins/echo.c \
		srcs/builtins/env.c \
		srcs/exec/exec_2.c \
		srcs/exec/exec_bonux.c \
		srcs/exec/exec.c \
		srcs/exec/exec_et_2.c \
		srcs/exec/exec_et.c \
		srcs/exec/exec_no_next.c \
		srcs/exec/exec_ou_2.c \
		srcs/exec/exec_ou.c \
		srcs/exec/exec_pp_2.c \
		srcs/exec/exec_pp.c \
		srcs/exec/execute_one.c \
		srcs/exec/exec_utils2.c \
		srcs/exec/exec_utils.c \
		srcs/builtins/exit_2.c \
		srcs/builtins/exit.c \
		srcs/builtins/export.c \
		srcs/builtins/export_no_arg.c \
		srcs/builtins/export_plus_equal.c \
		srcs/builtins/export_utils.c \
		srcs/exec/first_execute.c \
		srcs/utils/free.c \
		srcs/utils/ft_atoi.c \
		srcs/utils/ft_itoa.c \
		srcs/lst/ft_lst.c \
		srcs/lst/ft_lsttree.c \
		srcs/exec/heredoc_2.c \
		srcs/exec/heredoc.c \
		srcs/exec/last_execute.c \
		srcs/lst/lst_env_2.c \
		srcs/lst/lst_env_3.c \
		srcs/lst/lst_env.c \
		srcs/main.c \
		srcs/exec/middle_execute.c \
		srcs/parsing/ouverture_redir.c \
		srcs/parsing/ouverture_redir_first_cmd.c \
		srcs/parsing/parse_arg.c \
		srcs/parsing/parse_arg_firstcmd.c \
		srcs/parsing/parse_cmd.c \
		srcs/parsing/parse_first_cmd.c \
		srcs/parsing/parsing_main_1.c \
		srcs/parsing/parsing_main_2.c \
		srcs/builtins/pwd.c \
		srcs/parsing/recalculcmd.c \
		srcs/parsing/set_arg.c \
		srcs/parsing/set_arg_firstcmd.c \
		srcs/parsing/set_brace_lvl.c \
		srcs/parsing/set_brace_lvl_firstcmd.c \
		srcs/utils/strdup.c \
		srcs/utils/string_tools2.c \
		srcs/utils/string_tools3.c \
		srcs/utils/string_tools4.c \
		srcs/utils/string_tools.c \
		srcs/utils/strjoin.c \
		srcs/utils/supersplit2.c \
		srcs/utils/supersplit.c \
		srcs/parsing/syntax.c \
		srcs/builtins/unset.c \
		srcs/exec/wait.c \
		srcs/parsing/wildcards.c \
		srcs/parsing/wildcards_2.c \
		srcs/parsing/wildcards_3.c \
		srcs/parsing/wildcards_4.c \
		expander.c \


S_PATH	=	srcs/
O_PATH	=	
I_PATH	=	includes/

SRCS	=	${addprefix ${S_PATH}, ${FILES}}
OBJS	=	${addprefix ${O_PATH}, ${FILES:.c=.o}}

NAME	=	minishell

CC	=	clang

RM	=	rm -rf

CFLAGS	=	-Wall -Wextra -Werror -I./includes -g3

${O_PATH}%.o:	${S_PATH}%.c
		@mkdir -p ${dir $@}
		@${CC} ${CFLAGS} -c $< -o $@ -I ${I_PATH}
		@echo "${CYAN}Compiling${S} ${IGREY}$<${S} ${YELLOW}➡️  ${S}${SBLUE}$@${S}"

${NAME}:	${OBJS}
		@${CC} ${OBJS} ${CFLAGS} -o ${NAME} -lreadline -I ${I_PATH}
		@echo "${PURPLE}\n███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗\n████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║\n██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║\n██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║\n██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n${S}"
		@echo ""
		@echo "${PURPLE}Building${S} ${IGREY}$@${S} 🔨"
		@echo ""
		@echo "\033[3;92mCompilation is completed !${S} 🎉"

all:		${NAME}

clean:
		@${RM} ${OBJS} #${O_PATH}
		@echo "${SRED}Removing${S} ${IGREY}${O_PATH}${S} 🗑️"

fclean:		clean
		@${RM} ${NAME}
		@echo "${SRED}Removing${S} ${IGREY}${NAME}${S} 🗑️"

space:
		@echo ""

re:		fclean all

.PHONY:		all clean fclean re bonus

S	=	\033[0m
BOLD	=	\033[1m
ITALIC	=	\033[3m
UNDER	=	\033[4m
TEST	=	\033[5m
REV	=	\033[7m

# Colors
GREY	=	\033[30m
RED	=	\033[31m
GREEN	=	\033[32m
YELLOW	=	\033[33m
BLUE	=	\033[34m
PURPLE	=	\033[35m
CYAN	=	\033[36m
WHITE	=	\033[37m

SGREY	=	\033[90m
SRED	=	\033[91m
SGREEN	=	\033[92m
SYELLOW	=	\033[93m
SBLUE	=	\033[94m
SPURPLE	=	\033[95m
SCYAN	=	\033[96m
SWHITE	=	\033[97m

# Colored backgrounds
IGREY	=	\033[40m
IRED	=	\033[41m
IGREEN	=	\033[42m
IYELLOW	=	\033[43m
IBLUE	=	\033[44m
IPURPLE	=	\033[45m
ICYAN	=	\033[46m
IWHITE	=	\033[47m
