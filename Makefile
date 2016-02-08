CC=		gcc

RM=		rm -rf

NAME=		my_nm

NAME2=		my_objdump

DIR=		./srcs

DIR_NM=		$(DIR)/nm

DIR_OBJDUMP=	$(DIR)/objdump

SRCS_NM=	$(DIR_NM)/main.c \
		$(DIR_NM)/nm64.c \
		$(DIR_NM)/sort64.c \
		$(DIR_NM)/print64.c \
		$(DIR_NM)/swap64.c

SRCS_OBJDUMP=	$(DIR_OBJDUMP)/main.c \
		$(DIR_OBJDUMP)/start_obj.c \
		$(DIR_OBJDUMP)/print.c

OBJS_NM=	$(SRCS_NM:.c=.o)

OBJS_OBJDUMP=	$(SRCS_OBJDUMP:.c=.o)

CFLAGS+=	-Wall -W -Wextra -Werror -I ./includes

%.o:		%.c
		@printf "[\033[0;32mCompiled\033[0m] % 35s\n" $< 
		@$(CC) -c -o $@ $< $(CFLAGS)

all:		$(NAME) $(NAME2)

$(NAME):	$(OBJS_NM)
		@$(CC) $(OBJS_NM) -o $(NAME) $(CFLAGS)
		@printf "[\033[0;34mAssembled\033[0m] % 34s\n" $(NAME) | tr ' ' '.' 

$(NAME2):	$(OBJS_OBJDUMP)
		@$(CC) $(OBJS_OBJDUMP) -o $(NAME2) $(CFLAGS)
		@printf "[\033[0;34mAssembled\033[0m] % 34s\n" $(NAME2) | tr ' ' '.'

nm:		$(NAME)

objdump:	$(NAME2)

clean:
		@$(RM) $(OBJS_NM)
		@printf "[\033[0;31mDeleted\033[0m] % 36s\n" $(OBJS_NM)
		@$(RM) $(OBJS_OBJDUMP)
		@printf "[\033[0;31mDeleted\033[0m] % 36s\n" $(OBJS_OBJDUMP)

fclean:		clean
		@$(RM) $(NAME)
		@printf "[\033[0;35mRemoved\033[0m] % 36s\n" $(NAME) | tr ' ' '.'
		@$(RM) $(NAME2)
		@printf "[\033[0;35mRemoved\033[0m] % 36s\n" $(NAME2) | tr ' ' '.'


re:		fclean all

.PHONY:		all clean fclean re
