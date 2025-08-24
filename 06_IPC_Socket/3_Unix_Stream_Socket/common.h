#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_MESSAGE_LEN 4096
#define CHAR_SE '#'
#define CHAR_END_OF_STRING '\0'
#define MAX_TOKENS 10

struct data_string {
  int count;
  char tokens[MAX_TOKENS][BUFFER_SIZE];
};

void handle_message(struct data_string *total_message, char message[], char cache_data[]);
int add_separate_char_to_message(char message[]);