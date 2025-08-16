# Makefile to build avr_sh target (C)

CC      = clang
CFLAGS  = -std=c11 -g -Wall -Wextra -Werror -pedantic-errors -DDEBUG_PRINTS_ENABLED

# generate header dependencies
CFLAGS += -MMD -MP

TARGET  = avr_sh
SRCS    = avr_sh.c avr_sh_builtins.c avr_sh_helper.c
OBJS    = $(SRCS:.c=.o)
DEPS    = avr_sh_headers.h avr_sh_helper.h avr_sh_builtins.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) $(OBJS:.o=.d)

.PHONY: all clean

# include auto-generated header dependency files
-include $(OBJS:.o=.d)

# $@ - Target
# $^ - All prereqs
# $< - First prereq