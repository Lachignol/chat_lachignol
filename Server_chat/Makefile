NAME    = Serveur
SRCDIR  = src
OBJDIR  = obj
INCDIR  = includes

SRCS    = $(SRCDIR)/main.c \
          $(SRCDIR)/Monitor.c \
          $(SRCDIR)/Server.c \
          $(SRCDIR)/Sanitize.c \
          $(SRCDIR)/addUser.c \
          $(SRCDIR)/Broadcast.c

OBJS    = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -I$(INCDIR)

all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

