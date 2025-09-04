CC = gcc
CFLAGS = -Wall -g

TARGET_MAIN = build/main

SRCS_MAIN = main.c
OBJS_MAIN = $(SRCS_MAIN:.c=.o)

all: $(TARGET_MAIN)

$(TARGET_MAIN): $(OBJS_MAIN)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $(OBJS_MAIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET_MAIN) build
