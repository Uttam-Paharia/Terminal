#include <stdio.h>
#include <readline/history.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdbool.h>
bool copy(const char *src_filename, const char *dest_filename, bool i, bool nc, bool v, bool mv)
{
    
    FILE *src_file = fopen(src_filename, "rb");
    if (src_file == NULL)
    {
        fprintf(stderr, "Error: cannot open source file '%s'\n", src_filename);
        return false;
    }

    FILE *dest_file = fopen(dest_filename, "wb");
    if(dest_file == NULL)
    {
        printf("destination file is NULL so cant copy here give some valid directory");
        return false;
    }
    if (dest_file != NULL && nc)
    {
        printf("Can't overwrite the file\n");
        fclose(src_file);
        return false;
    }
    else
    {

        char c;
        bool u = false;
        if (dest_file != NULL && !i)
        {
            u = true;
        }
        if (dest_file != NULL && i)
        {
            printf("Do you want to overwrite the file?\n if yes type 'y' or 'Y' else type 'n' or 'N' ");
            scanf("%c", &c);
            if (c == 'y' || c == 'Y')
            {
                u = true;
            }
            else if (c == 'n' || c == 'N')
            {
                u = false;
                fclose(dest_file);
                fclose(src_file);
                return false;
            }
            else
            {
                printf("invalid command is given\n");
                fclose(src_file);
                fclose(dest_file);
                return false;
            }
        }
        if (u)
        {
            char buffer[2];
            while (fread(buffer, sizeof(char), 1, src_file) > 0)
            {
                fwrite(buffer, sizeof(char), 1, dest_file);
            }
            struct stat source_stat;
    if (stat(src_filename, &source_stat) == -1) {
        perror("stat");
        
    }
    
    
    // Copy the permissions to the destination file
    if (chmod(dest_filename, source_stat.st_mode) == -1) {
        perror("chmod");
        
    }
        }
    }

    fclose(src_file);
    fclose(dest_file);

    if (v && !mv)
    {
        printf("'%s' -> '%s'\n", src_filename, dest_filename);
    }
    if (mv)
    {
        if (remove(src_filename) == 0)
        {
            if(v)
            printf("'%s' moved to '%s'\n", src_filename, dest_filename);
            return true;
        }
        else
        {
            fprintf(stderr, "Error: unable to move file '%s' to '%s'\n", src_filename, dest_filename);
            return false;
        }
    }

    return true;
}

void mycp(char *command)
{
    bool i = false, nc = false, v = false;
    char *src, *dest;
    int a = 0, b = 0, c = 0;
    int error = 0;
    bool mv = false;
    if (command[2] == 'm')
    {
        mv = true;
    }
    char *tok = strtok(command, " ");
    char token[100][2000];
    int o = 0;
    while (tok != NULL)
    {
        int k=0;
        for (int j = 0; j < strlen(tok); j++)
        {
            token[o][j] = tok[j];

            k = j;
        }
        
        token[o][k+1] = '\0';
        
        o++;
        tok = strtok(NULL, " ");
    }
    token[o][0] = '\0';
    if (o != 3)
    {
        printf("Kindly give exactly two file names\n First is source file , second is destination file\n");
        return;
    }

    if (token[1][0] == '-')
    {
        for (int j = 1; j < strlen(token[1]); j++)
        {
            if (token[1][j] == 'v')
            {
                v = true;
            }
            else if (token[1][j] == 'n')
            {
                nc = true;
                i=false;
            }
            else if (token[1][j] == 'i')
            {
                i = true;
                nc=false;
            }
            else
            {
                error = 1;
            }
        }
    }
        if (error)
        {
            printf("Command not found!!\n");
        }
        else
        {
            if (token[1][0] == '-')
            {

                struct stat file;
                stat(token[3], &file);

                if ((S_ISDIR(file.st_mode)))
                {
                    strcat(token[3], "/");
                    strcat(token[3], token[2]);
                    copy(token[2], token[3], i, nc, v, mv);
                }
                else
                {
                    copy(token[2], token[3], i, nc, v, mv);
                }
            }

            if (token[1][0] != '-')
            {
                struct stat file;
                stat(token[2], &file);

                if ((S_ISDIR(file.st_mode)))
                {
                    strcat(token[2], "/");
                    strcat(token[2], token[1]);
                    copy(token[1], token[2], i, nc, v, mv);
                }
                else
                {
                    
                    copy(token[1], token[2], i, nc, v, mv);
                }
            }
        }
    
}
