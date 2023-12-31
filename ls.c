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
struct meta_data
{
    char name[200];
    struct stat filestat;
};
void print_perms(mode_t perms)
{
    printf((S_ISDIR(perms)) ? "d" : "-");

    printf((perms & S_IRUSR) ? "r" : "-");
    printf((perms & S_IWUSR) ? "w" : "-");
    printf((perms & S_IXUSR) ? "x" : "-");
    printf((perms & S_IRGRP) ? "r" : "-");
    printf((perms & S_IWGRP) ? "w" : "-");
    printf((perms & S_IXGRP) ? "x" : "-");
    printf((perms & S_IROTH) ? "r" : "-");
    printf((perms & S_IWOTH) ? "w" : "-");
    printf((perms & S_IXOTH) ? "x" : "-");
}

int cmp(const void *a, const void *b)

{
    char *k = (((struct meta_data *)a)->name);
    char *j = ((struct meta_data *)b)->name;
    return strcmp(k, j);
}
int cmpsize(const void *a, const void *b)
{
    int k = (((struct meta_data *)a)->filestat).st_size;
    int j = (((struct meta_data *)b)->filestat).st_size;
    return j - k;
}
int cmptime(const void *a, const void *b)
{
    int k = (((struct meta_data *)a)->filestat).st_mtime;
    int j = (((struct meta_data *)b)->filestat).st_mtime;
    return j - k;
}
char *formatdate(char *str, time_t val)
{
    strftime(str, 36, "%d %b %H:%M:%S", localtime(&val));
    return str;
}

void myls(char *command, char *pwd)
{
    char ls[2000];
    int y = 0;
    int error = 0;
    for (int i = 4; command[i] != '\0'; i++)
    {
        ls[y] = command[i];
        y++;
    }
    ls[y] = '\0';

    int t = 0, l = 0, S = 0, flag = 0, r = 0,a=0;
    struct meta_data arr[1000];
    struct dirent *de;
    int filecount = 0;
    DIR *dr = opendir(".");
    if (dr == NULL)
    {
        printf("could not open current directory\n");
    }
    while ((de = readdir(dr)) != NULL)
    {
        stat(de->d_name, &arr[filecount].filestat);
        strcpy(arr[filecount].name, de->d_name);
        filecount++;
    }
    if (ls[0] != ' ' && command[4] != '\0')
    {
        printf("Command not found!!\n");
        error = 1;
    }
    else
    {
        for (int i = 0; i < y; i++)
        {
            if (ls[i] == ' ')
            {
                flag = 0;
                continue;
            }
            if (ls[i] != 't' && ls[i] != 'S' && ls[i] != 'l' && ls[i] != 'r' && ls[i] !='a'&& ls[i] != '-')
            {

                printf("Command not found!!\n");
                error = 1;
                break;
            }
            if (ls[i] == 't' || ls[i] == 'S' || ls[i] == 'l' || ls[i] == 'r'|| ls[i]=='a')
            {
                if (ls[i - 1] == ' ')
                {
                    printf("command not found!!\n");
                    error = 1;
                    break;
                }
            }
            if (flag)
            {
                if (ls[i] == 't')
                {
                    t = 1;
                    
                    S=0;
                }

                if (ls[i] == 'l')
                {
                    l = 1;
                }
                if (ls[i] == 'S')
                {
                    S = 1;
                    
                    t=0;
                }
                if (ls[i] == 'r')
                {
                    r = 1;
                    
                    
                }
                if (ls[i] == 'a')
                {
                    a = 1;
                }

                if (ls[i - 1] == ' ')
                {
                    printf("Command not found");
                    error = 1;
                    break;
                }
            }
            else if (ls[i] == '-')
            {
                if (ls[i + 1] == '-')

                {
                    printf("Command not found!!!");
                    error = 1;
                    break;
                }
                flag = 1;
            }
        }
    }

    
    if (!error )
    {
        
        
            qsort(arr, filecount, sizeof(arr[0]), cmp);
        
        if (t == 1)
        {
            qsort(arr, filecount, sizeof(arr[0]), cmptime);
        }
        if (S == 1)
        {
            qsort(arr, filecount, sizeof(arr[0]), cmpsize);
        }
        if (r == 0 && l!=1)
        {

            for (int i = 0; i < filecount; i++)
            {
                if(arr[i].name[0]=='.' && a!=1)
                {
                    continue;
                }
                printf("%s\n", arr[i].name);
            }
        }
        if (r == 1 && l!=1)
        {
            qsort(arr, filecount, sizeof(arr[0]), cmp);
            for (int i = filecount - 1; i >= 0; i--)
            {
                if(arr[i].name[0]=='.' && a!=1)
                {
                    continue;
                }
                printf("%s\n", arr[i].name);
            }
        }

        closedir(dr);
    }
    if (l == 1 && error == 0)
    {
        if(r!=1)
        {
        for (int i = 0; i < filecount; i++)
        {
            if(arr[i].name[0]=='.' && a!=1)
                {
                    continue;
                }
            print_perms(arr[i].filestat.st_mode);
            printf("   %ld ", arr[i].filestat.st_nlink);
            printf("   %s ", (getpwuid(arr[i].filestat.st_uid))->pw_name);
            printf("   %s ", (getgrgid(arr[i].filestat.st_gid))->gr_name);
            printf("   %-10ld ", arr[i].filestat.st_size);
            char date[100];
            printf("   %s ", formatdate(date, arr[i].filestat.st_mtime));
             printf("    %s\n", arr[i].name);
        }
        }
        if(r==1)
        {
            for (int i = filecount-1; i>=0; i--)
        {
            if(arr[i].name[0]=='.'&& a!=1)
                {
                    continue;
                }
            print_perms(arr[i].filestat.st_mode);
            printf("   %ld ", arr[i].filestat.st_nlink);
            printf("   %s ", (getpwuid(arr[i].filestat.st_uid))->pw_name);
            printf("   %s ", (getgrgid(arr[i].filestat.st_gid))->gr_name);
            printf("   %-10ld ", arr[i].filestat.st_size);
            char date[100];
            printf("   %s", formatdate(date, arr[i].filestat.st_mtime));
            printf("    %s\n", arr[i].name);
        }

        }
    }
}