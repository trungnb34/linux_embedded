#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./filestat <file_path>\n");
        return -1;
    }
    struct stat sb;
    char *filePath = argv[1];
    printf("path : %s\n", filePath);
    if (lstat(filePath, &sb) == -1) {
        perror("lstat error \n");
        return -1;
    }
    
    if (S_ISREG(sb.st_mode)) {
        printf("%s is a regular file.\n", filePath);
    } else if (S_ISDIR(sb.st_mode)) {
        printf("%s is a directory.\n", filePath);
    } else if (S_ISLNK(sb.st_mode)) {
        printf("%s is a symbolic link.\n", filePath);
    } else {
        printf("%s is another type of file.\n", filePath);
    }
    printf("Size of file %s : %d \n", filePath, sb.st_size);

    time_t lastModified = sb.st_mtime;
    printf("Last time modified : %d\n", lastModified);

    struct tm *dt = localtime(&lastModified);

    if (dt == NULL) {
        perror("localtime");
        return 1;
    }

    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", dt);
    printf("Date time : %s\n", buffer);

    return 0;
}