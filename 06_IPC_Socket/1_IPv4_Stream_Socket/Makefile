CC = gcc
CFLAGS = -Wall -g

TARGET_SERVER = build/server
TARGET_CLIENT = build/client

SRCS_SERVER = server.c common.c
OBJS_SERVER = $(SRCS_SERVER:.c=.o)

SRCS_CLIENT = client.c common.c
OBJS_CLIENT = $(SRCS_CLIENT:.c=.o)

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(OBJS_SERVER)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $(OBJS_SERVER)

$(TARGET_CLIENT): $(OBJS_CLIENT)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $(OBJS_CLIENT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET_SERVER) $(TARGET_CLIENT) build
