CC = gcc
CFLAGS = -Wall -g

TARGET_WORKER = build/task_worker
TARGET_DISPATCHER = build/task_dispatcher

SRCS_DISPATCHER = task_dispatcher.c
OBJS_DISPATCHER = $(SRCS_DISPATCHER:.c=.o)

SRCS_WORKER = task_worker.c
OBJS_WORKER = $(SRCS_WORKER:.c=.o)

all: $(TARGET_WORKER) $(TARGET_DISPATCHER)

$(TARGET_WORKER): $(OBJS_WORKER)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $(OBJS_WORKER)

$(TARGET_DISPATCHER): $(OBJS_DISPATCHER)
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $(OBJS_DISPATCHER)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET_WORKER) $(TARGET_DISPATCHER) build
