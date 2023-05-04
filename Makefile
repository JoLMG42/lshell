# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lguillau <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/14 14:27:36 by lguillau          #+#    #+#              #
#    Updated: 2023/05/04 18:58:37 by jtaravel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FILES	=	add_sapce_dol.c\
			add_spaces2.c \
			add_spaces.c \
			builtins.c \
			cd2.c \
			cd.c \
			check_syntax_func.c \
			echo_2.c \
			echo.c \
			env.c \
			exec_2.c \
			exec_bonux.c \
			exec.c \
			exec_et_2.c \
			exec_et.c \
			exec_no_next.c \
			exec_ou_2.c \
			exec_ou.c \
			exec_pp_2.c \
			exec_pp.c \
			execute_one.c \
			exec_utils2.c \
			exec_utils.c \
			exit_2.c \
			exit.c \
			export.c \
			export_no_arg.c \
			export_plus_equal.c \
			export_utils.c \
			first_execute.c \
			free.c \
			ft_itoa.c \
			ft_lst.c \
			ft_lsttree.c \
			heredoc_2.c \
			heredoc.c \
			last_execute.c \
			lst_env_2.c \
			lst_env_3.c \
			lst_env.c \
			main.c \
			middle_execute.c \
			ouverture_redir.c \
			ouverture_redir_first_cmd.c \
			parse_arg.c \
			parse_arg_firstcmd.c \
			parse_cmd.c \
			parse_first_cmd.c \
			parsing_main_1.c \
			parsing_main_2.c \
			pwd.c \
			recalculcmd.c \
			set_arg.c \
			set_arg_firstcmd.c \
			set_brace_lvl.c \
			set_brace_lvl_firstcmd.c \
			strdup.c \
			string_tools2.c \
			string_tools3.c \
			string_tools4.c \
			string_tools.c \
			strjoin.c \
			supersplit2.c \
			supersplit.c \
			syntax.c \
			unset.c \
			wait.c \
			wildcards.c \
			wildcards_2.c \
			wildcards_3.c \
			wildcards_4.c \




S_PATH	=	srcs/
O_PATH	=	./
I_PATH	=	includes/

SRCS	=	${addprefix ${S_PATH}, ${FILES}}
OBJS	=	${addprefix ${O_PATH}, ${FILES:.c=.o}}

NAME	=	minishell

CC	=	clang

RM	=	rm -rf

CFLAGS	=	-Wall -Wextra -Werror -g3

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
		@${RM} ${O_PATH}
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
