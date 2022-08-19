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
    // 4 arguments needed
    if (argc < 4)
    {
        perror("Incorrect input!");
        return 0;
    }

    int num = 1000;

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

    // creating directory with required permissions and naming new file:
    struct stat check_dir;
    if (stat("Assignment", &check_dir) == -1)
    {
        mkdir("Assignment", 0700); // check permissions
    }

    char new_file[500] = "Assignment/2_";
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

    // setting max number of chunks acc to file size
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

    char *start_char = argv[2];
    char *end_char = argv[3];

    long long int start = 0;
    long long int end = 0;
    long long pow = 1;

    // convert character input to integer
    for (int i = strlen(argv[2]) - 1; i >= 0; i--)
    {
        start += pow * (argv[2][i] - '0');
        pow *= 10;
    }
    pow = 1;
    for (int i = strlen(argv[3]) - 1; i >= 0; i--)
    {
        end += pow * (argv[3][i] - '0');
        pow *= 10;
    }
    // lseek, read, write
    long long chunk_size = start / num;
    if (!chunk_size)
    {
        chunk_size = start;
    }

    /*  
        similar logic as Q1. 
        Divide the file into 3 parts :
        0-> start; start-> end; end->EOF
        reverse files in the first and 3rd part
        Keep it same for the second part.
        Set pointers accordingly
    */

   // Note: errors in Work Done portion


   // First part
    char *read_str, *write_str;

    read_str = (char *)calloc(chunk_size, sizeof(char));
    write_str = (char *)calloc(chunk_size, sizeof(char));

    char *print_str;
    double work_done = 0;
    print_str = (char *)calloc(50, sizeof(char));
    start--;

    long long ptr = (start / chunk_size) * (chunk_size);
    if (ptr == start)
    {
        ptr = ptr - chunk_size;
    }

    long long str_len = start - ptr + 1;

    lseek(new_fd, 0, SEEK_SET);
    lseek(prev_fd, ptr, SEEK_SET);

    int flag = 1;

    long w2 = 0;
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
        w2 += str_len;

        work_done = 100.0 * w2 / start;
        sprintf(print_str, "Work done: %.2f %%\r", work_done);
        write(1, print_str, 50);
        fflush(stdout);

        lseek(prev_fd, -i - chunk_size, SEEK_CUR);
        str_len = chunk_size;
    }

    // Second part
    long long mid = end - start;
    chunk_size = mid / num;
    if (!chunk_size)
    {
        chunk_size = mid;
    }
    long long words_read = 0;
    read_str = (char *)calloc(chunk_size + 1, sizeof(char));
    write_str = (char *)calloc(chunk_size + 1, sizeof(char));

    ptr = start + 1;

    str_len = mid - (chunk_size * (mid / num));
    lseek(new_fd, 0, SEEK_CUR);
    lseek(prev_fd, ptr, SEEK_SET);

    flag = 1;

    while (flag)
    {
        read(prev_fd, read_str, str_len);

        int index = str_len - 1;
        long long int i = 0;

        write(new_fd, read_str, str_len);

        long long check = lseek(prev_fd, 0, SEEK_CUR);
        check = check - chunk_size - i;
        words_read += str_len;
        if (words_read == mid)
        {
            break;
        }

        work_done = 100.0 * w2 / file_len;
        sprintf(print_str, "Work done: %.2f %%\r", work_done);
        write(1, print_str, 50);
        fflush(stdout);

        lseek(prev_fd, -i - chunk_size, SEEK_CUR);
        str_len = chunk_size;
    }

    // Last part

    long long end2 = end;
    end = file_len - end - 1;
    chunk_size = end / num;
    if (!chunk_size)
    {
        chunk_size = end;
    }

    ptr = end2 + ((end / chunk_size) * (chunk_size)) + 1;
    read_str = (char *)calloc(chunk_size + 1, sizeof(char));
    write_str = (char *)calloc(chunk_size + 1, sizeof(char));

    str_len = file_len - ptr + 1;

    lseek(new_fd, end2 + 1, SEEK_SET);
    lseek(prev_fd, ptr, SEEK_SET);

    flag = 1;

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
        w2+=str_len;

        long long check = lseek(prev_fd, 0, SEEK_CUR);
        check = check - chunk_size - i;
        if (check < end2)
        {
            break;
        }

        work_done = 100.0 * w2 / file_len;
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