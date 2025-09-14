CC = gcc
CFLAGS = -Wall -g

TARGET_CONSUMER = build/posix_shm_consumer
TARGET_PRODUCER = build/posix_shm_producer

SRCS_PRODUCER = posix_shm_producer.c
OBJS_PRODUCER = $(SRCS_PRODUCER:.c=.o)

SRCS_CONSUMER = posix_shm_consumer.c
OBJS_CONSUMER = $(SRCS_CONSUMER:.c=.o)

all: $(TARGET_CONSUMER) $(TARGET_PRODUCER)

$(TARGET_CONSUMER): $(OBJS_CONSUMER)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $(OBJS_CONSUMER)

$(TARGET_PRODUCER): $(OBJS_PRODUCER)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $(OBJS_PRODUCER)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET_CONSUMER) $(TARGET_PRODUCER) build