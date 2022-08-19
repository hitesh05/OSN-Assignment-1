#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


// Printing the permission(s)
void print(int who, int what, int flag, int n)
{
    char user[3][10] = {"User", "Group", "Others"};
    char permission[3][10] = {"read", "write", "execute"};
    char fl[2][10] = {"Yes", "No"};
    char name[3][10] = {"directory", "oldfile", "newfile"};

    char buffer[60];
    sprintf(buffer, "%s has %s permission on %s: %s\n", user[who], permission[what], name[n], fl[flag]);
    int len = strlen(buffer);
    buffer[len] = '\0';
    write(1, buffer, len);
    fflush(stdout);
}

// Printing if the files are reversed or not
void print2(int ans)
{
    char a[2][15] = {"reversed", "Not reversed"};
    char buffer[60];
    sprintf(buffer, "Files are: %s\n", a[ans]);
    int len = strlen(buffer);
    buffer[len] = '\0';
    write(1, buffer, len);
    fflush(stdout);
}

int main(int argc, char **argv)
{
    if (argc < 4) // number of arguments
    {
        perror("Incorrect input!");
        return 0;
    }

    const char *oldfile = argv[1];
    const char *newfile = argv[2];
    const char *directory = argv[3];

    // creating directory and naming new file:
    struct stat dir, file1, file2;
    char buffer[60];


    // Checking permissions for directory
    if (!stat(directory, &dir) && S_ISDIR(dir.st_mode))
    {
        sprintf(buffer, "Directory is created: Yes\n\n");
        int len = strlen(buffer);
        buffer[len] = '\0';
        write(1, buffer, len);
        fflush(stdout);

        // USER //
        if (dir.st_mode & S_IRUSR)
        {
            print(0, 0, 0, 0);
        }
        else
        {
            print(0, 0, 1, 0);
        }

        if (dir.st_mode & S_IWUSR)
        {
            print(0, 1, 0, 0);
        }
        else
        {
            print(0, 1, 1, 0);
        }

        if (dir.st_mode & S_IXUSR)
        {
            print(0, 2, 0, 0);
        }
        else
        {
            print(0, 2, 1, 0);
        }

        // GROUP //

        if (dir.st_mode & S_IRGRP)
        {
            print(1, 0, 0, 0);
        }
        else
        {
            print(1, 0, 1, 0);
        }

        if (dir.st_mode & S_IWGRP)
        {
            print(1, 1, 0, 0);
        }
        else
        {
            print(1, 1, 1, 0);
        }

        if (dir.st_mode & S_IXGRP)
        {
            print(1, 2, 0, 0);
        }
        else
        {
            print(1, 2, 1, 0);
        }

        // OTHERS //

        if (dir.st_mode & S_IROTH)
        {
            print(2, 0, 0, 0);
        }
        else
        {
            print(2, 0, 1, 0);
        }

        if (dir.st_mode & S_IWOTH)
        {
            print(2, 1, 0, 0);
        }
        else
        {
            print(2, 1, 1, 0);
        }

        if (dir.st_mode & S_IXOTH)
        {
            print(2, 2, 0, 0);
        }
        else
        {
            print(2, 2, 1, 0);
        }
    }
    else
    {
        sprintf(buffer, "Directory is created: No\n\n");
        int len = strlen(buffer);
        buffer[len] = '\0';
        write(1, buffer, len);
        fflush(stdout);
        perror("Directory not found");
        // exit(1); //if directory doesn't exist, exit
    }

    // Checking permissions for oldfile
    if (!stat(oldfile, &file1))
    {
        // USER //
        if (file1.st_mode & S_IRUSR)
        {
            print(0, 0, 0, 1);
        }
        else
        {
            print(0, 0, 1, 1);
        }

        if (file1.st_mode & S_IWUSR)
        {
            print(0, 1, 0, 1);
        }
        else
        {
            print(0, 1, 1, 1);
        }

        if (file1.st_mode & S_IXUSR)
        {
            print(0, 2, 0, 1);
        }
        else
        {
            print(0, 2, 1, 1);
        }

        // GROUP //

        if (file1.st_mode & S_IRGRP)
        {
            print(1, 0, 0, 1);
        }
        else
        {
            print(1, 0, 1, 1);
        }

        if (file1.st_mode & S_IWGRP)
        {
            print(1, 1, 0, 1);
        }
        else
        {
            print(1, 1, 1, 1);
        }

        if (file1.st_mode & S_IXGRP)
        {
            print(1, 2, 0, 1);
        }
        else
        {
            print(1, 2, 1, 1);
        }

        // OTHERS //

        if (file1.st_mode & S_IROTH)
        {
            print(2, 0, 0, 1);
        }
        else
        {
            print(2, 0, 1, 1);
        }

        if (file1.st_mode & S_IWOTH)
        {
            print(2, 1, 0, 1);
        }
        else
        {
            print(2, 1, 1, 1);
        }

        if (file1.st_mode & S_IXOTH)
        {
            print(2, 2, 0, 1);
        }
        else
        {
            print(2, 2, 1, 1);
        }
    }
    else
    {
        perror("output_file_1 not found");
        write(1, "\n", 1);
    }

    // Checking permissions for newfile
    if (!stat(newfile, &file2))
    {
        // USER //
        if (file2.st_mode & S_IRUSR)
        {
            print(0, 0, 0, 2);
        }
        else
        {
            print(0, 0, 1, 2);
        }

        if (file2.st_mode & S_IWUSR)
        {
            print(0, 1, 0, 2);
        }
        else
        {
            print(0, 1, 1, 2);
        }

        if (file2.st_mode & S_IXUSR)
        {
            print(0, 2, 0, 2);
        }
        else
        {
            print(0, 2, 1, 2);
        }

        // GROUP //

        if (file2.st_mode & S_IRGRP)
        {
            print(1, 0, 0, 2);
        }
        else
        {
            print(1, 0, 1, 2);
        }

        if (file2.st_mode & S_IWGRP)
        {
            print(1, 1, 0, 2);
        }
        else
        {
            print(1, 1, 1, 2);
        }

        if (file2.st_mode & S_IXGRP)
        {
            print(1, 2, 0, 2);
        }
        else
        {
            print(1, 2, 1, 2);
        }

        // OTHERS //

        if (file2.st_mode & S_IROTH)
        {
            print(2, 0, 0, 2);
        }
        else
        {
            print(2, 0, 1, 2);
        }

        if (file2.st_mode & S_IWOTH)
        {
            print(2, 1, 0, 2);
        }
        else
        {
            print(2, 1, 1, 2);
        }

        if (file2.st_mode & S_IXOTH)
        {
            print(2, 2, 0, 2);
        }
        else
        {
            print(2, 2, 1, 2);
        }
    }
    else
    {
        perror("output_file_2 not found");
        write(1, "\n", 1);
    }

    const int num = 1000; // number of chunks

    // getting filename after parsing the path provided
    const char delim[2] = "/";


    // parse name of oldfile from path provided
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

    // parse name of newfile from path provided
    char *filepath2;
    filepath2 = (char *)calloc(500, sizeof(char));
    strcat(filepath2, argv[2]);

    char *t, t2[500];
    char *next_file;
    strcpy(t2, filepath2);
    t = strtok(t2, delim);
    if (t == NULL)
    {
        next_file = argv[2];
    }
    while (t != NULL)
    {
        next_file = t;
        t = strtok(NULL, delim);
    }

    if (strcmp(prev_file, filepath) != 0)
    {
        filepath[strlen(filepath) - strlen(prev_file)] = '\0';
    }

    chdir(filepath);

    // try opening oldfile
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
        perror("error in size of oldfile");
        return 0;
    }

    if (strcmp(next_file, filepath2) != 0)
    {
        filepath2[strlen(filepath2) - strlen(newfile)] = '\0';
    }

    chdir(filepath2);

    // try opening new file
    int new_fd = open(next_file, O_RDONLY);
    if (new_fd < 0)
    {
        perror("error opening new file\n");
        return 0;
    }

    off_t file_len2 = lseek(new_fd, -1, SEEK_END);
    if (file_len2 < 0)
    {
        perror("error in size of newfile");
        return 0;
    }

    // if files are not of same length, they can't have been reversed
    if (file_len != file_len2)
    {
        print2(1);
        return 0;
    }

    // similar logic as previous 2 questions to check for reversal
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

    // set one pointer to EOF and one at the start
    lseek(new_fd, 0, SEEK_SET);
    lseek(prev_fd, ptr, SEEK_SET);

    int flag = 1;

    while (flag)
    {
        read(prev_fd, read_str, str_len);
        read(new_fd, write_str, str_len);

        int index = str_len - 1;
        long long int i = 0;

        for (; i < str_len; i++)
        {
            if (write_str[i] != read_str[index]) // break if characters do not match
            {
                print2(1);
                return 0;
            }
            index--;
        }

        long long check = lseek(prev_fd, 0, SEEK_CUR);
        check = check - chunk_size - i;
        if (check < 0)
        {
            break;
        }

        lseek(prev_fd, -i - chunk_size, SEEK_CUR);
        str_len = chunk_size;
    }

    // files are reversed
    print2(0);

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