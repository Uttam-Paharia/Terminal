#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <readline/history.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
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
#define _GNU_SOURCE
char *strcasestr(const char *haystack, const char *needle);

void grep(const char *pattern, const char *filename, bool n, bool c, bool w)
{
    FILE *fp;

    char line[2000];
    int  line_num_word = 0;
    int line_num=0;

    int found = 0;
    char *ptr;
    char *token;
    int pattern_len = strlen(pattern);

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error: Cannot open file '%s'\n", filename);
        return;
    }

    while (fgets(line, 2000, fp) != NULL)
    {
        int flag = 1;
        line_num++;



        ptr = strstr(line, pattern);

        if (ptr != NULL)
        {
            if (w == 0)
            {
                 line_num_word++;
                if(n && !c)
                {
                    printf("%d:", line_num);
                }

                for (int i = 0; line[i] != '\0'; i++)
                {
                    int check = 1;
                    for (int j = 0; j < strlen(pattern); j++)
                    {
                        if (line[i + j] != pattern[j])
                        {
                            check = 0;
                            break;
                        }
                    }
                    if (check == 0)
                    {
                        if(!c)
                        printf("%c", line[i]);
                    }
                    else
                    {
                        if(!c)
                        printf("\001\e[01;31m\002");
                        int j = 0;
                        for (j = 0; j < strlen(pattern); j++)
                        {
                            if(!c)
                            printf("%c", line[i + j]);
                        }
                        if(!c)
                        printf("\001\e[0m\002");
                        i += (j - 1);
                    }
                }
            }

            else
            {
                int word_check =0;
                char dup[2000];
                strcpy(dup,line);
                

                char *tok = strtok(dup," \n\t\0");
                while(tok!=NULL)
                {
                    if(strcmp(tok,pattern)==0)
                    {
                        
                        word_check=1;
                        break;
                    }
                    

                    tok = strtok(NULL, " \n\t\0");
                }
                if(word_check)
                {
                   

                for (int i = 0; line[i] != '\0'; i++)
                {
                    int check = 1;
                    
                    for (int j = 0; j < strlen(pattern); j++)
                    {
                        if (i != 0 && line[i - 1] != ' ')
                        {
                            check = 0;
                            break;
                        }
                        if (line[i + strlen(pattern)] != '\n' && line[i + strlen(pattern)] != ' ')
                        {
                            check = 0;
                            break;
                        }
                        if (line[i + j] != pattern[j])
                        {
                            check = 0;
                            break;
                        }
                    }
                    if (check == 0)
                    {
                        if(!c)
                        printf("%c", line[i]);
                    }
                    else
                    {
                        if (flag)
                        {
                             line_num_word++;
                            if(n && !c)
                            {
                                printf("%d:", line_num);
                            }
                            flag = 0;
                        }
                        if(!c)
                        printf("\001\e[01;31m\002");
                        int j = 0;
                        for (j = 0; j < strlen(pattern); j++)
                        {
                            if(!c)
                            printf("%c", line[i + j]);
                        }
                        // printf("%s",prompt);
                        if(!c)
                        printf("\001\e[0m\002");
                        i += (j - 1);
                    }
                }
            }
            }
        }
    }
    if(c)
    {
        printf("%d", line_num_word);
    }

    fclose(fp);
}
void mygrep(char *command)
{
    bool c = false, n = false, w = false;
    int error = 0;
    char *file;
    char *pattern;
    int a = 0, b = 0, d = 0;
    if(command[6]=='\0'||command[6]=='\n')
    {
        error=1;
        return;
    }
    
    
    if (command[7] == '-')
    {
        for (int j = 8; command[j] != ' '; j++)
        {
            if (command[j] == 'w')
            {
                w = true;
            }
            else if (command[j] == 'n')
            {
                n = true;
            }
            else if (command[j] == 'c')
            {
                 c= true;
            }
            else
            {
                error = 1;
                printf("command not found!!");
                return;
            }
            a++;
        }
        for (int j = 9 + a; command[j] != ' '; j++)
        {
            pattern[b] = command[j];
            b++;
        }
        pattern[b] = '\0';
        //
        for (int j = 10 + a + b; command[j] != '\0'; j++)
        {
            file[d] = command[j];
            d++;
        }
        file[d] = '\0';
        // printf("%s",file);
    }
    else
    {
        for (int j = 7 + a; command[j] != ' '; j++)
        {
            pattern[b] = command[j];
            b++;
        }
        pattern[b] = '\0';

        for (int j = 8 + a + b; j < strlen(command); j++)
        {
            file[d] = command[j];
            d++;
        }
        file[d] = '\0';
    }
    
     if(!error)
    grep(pattern, file, n,c, w);
    else
    {
        printf("Command not found!!\n");
    }
}
