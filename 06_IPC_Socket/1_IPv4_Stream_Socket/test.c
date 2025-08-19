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

// Init the new data_string
int init_data(struct data_string *data) {
    data->count = 0;
    for (int i = 0; i < MAX_TOKENS; i++) {
        data->tokens[i][0] = CHAR_END_OF_STRING;
    }
    return EXIT_SUCCESS;
}

struct data_string split_string_by_char (char str[], char separator) {
    // create string separator 
    char delimiter[] = {separator};
    struct data_string data;
    data.count = 0;
    // check # in str if not then return current string  
    char *p = strchr(str, separator);
    if (p == NULL) {
        data.count = 1;
        strncpy(data.tokens[data.count - 1], str, BUFFER_SIZE - 1);
        return data;
    }
    char *token = strtok(str, delimiter);
    while (token != NULL && data.count < MAX_TOKENS) {
        // copy the token to tokens list
        strncpy(data.tokens[data.count], token, BUFFER_SIZE - 1);
        data.tokens[data.count][BUFFER_SIZE - 1] = CHAR_END_OF_STRING;
        data.count ++;
        // get next string that separate by #
        token = strtok(NULL, delimiter);
    }
    return data;
}

int append_str(char dst[], char src[]) {
    int new_size = strlen(dst) + strlen(src) + 1;
    // the data string of dst must less than MAX_MESSAGE_LEN
    if (new_size > MAX_MESSAGE_LEN) {
        return EXIT_FAILURE;
    }
    strcat(dst, src);
    return EXIT_SUCCESS;
}

void handle_message(struct data_string *total_message, char message[], char cache_data[]) {
    // check the postion of CHAR_SE in message data 
    char *p = strchr(message, CHAR_SE);
    if (strlen(cache_data) > 0 && p) {
        append_str(total_message->tokens[0], cache_data); // copy cache_data to first item
        cache_data[0] = CHAR_END_OF_STRING; // reset cache_data to NULL
    }
    char temp[BUFFER_SIZE];
    strcpy(temp, message);
    struct data_string data = split_string_by_char(temp, CHAR_SE);
    
    if (message[strlen(message) - 1] != CHAR_SE) {
        if (append_str(cache_data, data.tokens[data.count - 1]) == EXIT_FAILURE) {
            cache_data[0] = CHAR_END_OF_STRING;
        }
        data.count --;
    }
    for (int i = 0; i < data.count; i++) {
        if (append_str(total_message->tokens[i], data.tokens[i]) == EXIT_FAILURE) {
            total_message->tokens[i][0] = CHAR_END_OF_STRING;
        }
        total_message->count ++;
    }
}

int main() {
    char messages[3][288] = {
        "abcdddaadfsfa#",
        "defasdfsdfdefasd12fadfsf#",
        "1234-=----=-90#"
    };

    char cache_data[BUFFER_SIZE];
    cache_data[0] = CHAR_END_OF_STRING;
    struct data_string total_message;
    init_data(&total_message);
    
    
    for (int i = 0; i < 3; i++) {
        handle_message(&total_message, messages[i], cache_data);
        if (total_message.count != 0) {
            // In kết quả
            printf("Tổng số phần tử: %d\n", total_message.count);
            for (int i = 0; i < total_message.count; i++) {
                printf("Token[%d] = %s\n", i, total_message.tokens[i]);
            }
            init_data(&total_message);
            printf("Clear messages\n");
        }
    }
    
    

    return 0;
}
