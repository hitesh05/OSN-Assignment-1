#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc < 2) // number of arguments
    {
        perror("Incorrect input!");
        return 0;
    }

    int num = 1000; // number of chunks

    // getting filename after parsing the path provided
    const char delim[2] = "/";

    char *filepath;
    filepath = (char *)calloc(500, sizeof(char));
    strcat(filepath, argv[1]);

    /* get the first token */
    char *token, token2[500];
    char *prev_file;
    strcpy(token2, filepath);
    token = strtok(token2, delim);
    if (token == NULL)
    {
        prev_file = argv[1];
    }
    /* walk through other tokens */
    while (token != NULL)
    {
        prev_file = token;
        token = strtok(NULL, delim);
    }

    // creating directory and naming new file:
    struct stat check_dir;
    if (stat("Assignment", &check_dir) == -1)
    {
        mkdir("Assignment", 0777); // check permissions
    }

    char new_file[500] = "Assignment/1_";
    char fname[500] = "new_file";
    strcpy(fname, prev_file);
    strcat(new_file, fname); // new_file = new file path

    int new_fd = open(new_file, O_RDWR | O_CREAT, S_IRWXU);
    if (new_fd < 0)
    {
        perror("error opening new file\n");
        return 0;
    }

    if (strcmp(prev_file, filepath) != 0)
    {
        filepath[strlen(filepath) - strlen(prev_file)] = '\0';
    }

    chdir(filepath);

    int prev_fd = open(prev_file, O_RDONLY);

    int check = -prev_fd;
    if (check > 0)
    {
        perror("error encountered opening input file");
        return 0;
    }

    off_t file_len = lseek(prev_fd, -1, SEEK_END);
    if (file_len < 0)
    {
        perror("error in input size");
        return 0;
    }
    if(file_len <= 1000000){  // 1 MB
        num = 1000;
    }
    else if(file_len<= 10000000){ // 10 MB
        num = 10000;
    }
    else if(file_len <= 100000000){ // 100 MB
        num = 100000;
    }
    else{ // 1 GB or more
        num = 1000000;
    }

    if (file_len < 0)
    {
        perror("error in input size");
        return 0;
    }

    // lseek, read, write
    long long chunk_size = file_len / num;
    if (!chunk_size)
    {
        chunk_size = file_len;
    }

    char *read_str, *write_str;

    read_str = (char *)calloc(chunk_size, sizeof(char));
    write_str = (char *)calloc(chunk_size, sizeof(char));

    char *print_str;
    double work_done = 0;
    print_str = (char *)calloc(50, sizeof(char));

    long long ptr = (file_len / chunk_size) * (chunk_size);
    if (ptr == file_len)
    {
        ptr = ptr - chunk_size;
    }

    long long str_len = file_len - ptr + 1;

    lseek(new_fd, 0, SEEK_SET);
    lseek(prev_fd, ptr, SEEK_SET);

    int flag = 1;

    while (flag)
    {
        read(prev_fd, read_str, str_len);

        int index = str_len - 1;
        long long int i = 0;
        for (; i < str_len; i++)
        {
            write_str[i] = read_str[index];
            index--;
        }
        write_str[i] = '\0';

        write(new_fd, write_str, i);

        long long check = lseek(prev_fd, 0, SEEK_CUR);
        check = check - chunk_size - i;
        if (check < 0)
        {
            break;
        }

        work_done = 100.0 * i / file_len;
        sprintf(print_str, "Work done: %.2f %%\r", work_done);
        write(1, print_str, 50);
        fflush(stdout);

        lseek(prev_fd, -i - chunk_size, SEEK_CUR);
        str_len = chunk_size;
    }

    sprintf(print_str, "Work done: 100.00 %\n", work_done);
    write(1, print_str, 50);
    fflush(stdout);

    char string_arr[10];
    int out;

    if (close(new_fd) < 0)
    {
        out = sprintf(string_arr, "c1\n");
        write(1, string_arr, out);
        return 1;
    }

    if (close(prev_fd) < 0)
    {
        out = sprintf(string_arr, "c2\n");
        write(1, string_arr, out);
        return 1;
    }

    return 0;
}