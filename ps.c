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

int allnumbers(char *name)
{
    int k = 1;
    for (int i = 0; i < strlen(name); i++)
    {
        if (!isdigit(name[i]))
        {
            k = 0;
        }
    }
    return k;
}

struct data
{
    int pid;
    int parentid;
    int sid;
    char cmd[1000];
    unsigned long long int rss;
    long int sz;
    int uid;
    char username[256];
    int psr;
    char tty_name[256];
    long long int utime;
    long long int stime;
    time_t STIME;

    char formed_utime[256];
    char formed_STIME[256];
    int c;
};

int myps(char *command)
{
    int af = 0, ef = 0, a = 0,e=0, f=0,error=0;

    char *duplicatecommand;
    duplicatecommand=(char*)malloc(1000*sizeof(char));
    strcpy(duplicatecommand,command);
    char *token=strtok(duplicatecommand," \n\t\0");
    while(token!=NULL)
    {
        if(!strcmp("-af",token))
        {
            af=1;
            ef = 0; a = 0;e=0;
            f=1;
        }
        else if(!strcmp("-ef",token))
        {
            ef=1;
            af=0;a=0;e=0;f=1;
        }
        else if(!strcmp("-a",token))
        {
            a=1;e=0;af=0;ef=0;
            f=0;
        }
        else if(!strcmp("-e",token))
        {
            e=1;
            a=0;af=0;ef=0;
            f=0;
        }
        else if(!strcmp("myps",token))
        {
            
        }
        
        else
        {
            printf("Command not found !!:(");
            error=1;
        }

        token=strtok(NULL," \n\t\0");
    }
    if(!error)
    {
    char *cmdarray[100];
    int processid = getpid();
    int parentid = getppid();
    char current_tty[256];
    memset(current_tty ,'\0' , 256);
    char store[256];
    memset(store , '\0' , 256);
    sprintf(store, "/proc/%d/fd/0" , processid);
    int fd = open(store , O_RDONLY , 0);
    strcpy(current_tty , ttyname(fd));
    close(fd);
    memset(store, '\0', 256);
    strcpy(store, current_tty);
    memset(current_tty , '\0' , 256);
    for (int i = 5; i < strlen(store); i++)
    {
        current_tty[i - 5] = store[i];
    }
    
    int k = 0;
    cmdarray[k] = strtok(command, " ");
    while (cmdarray[k] != NULL)
        cmdarray[++k] = strtok(NULL, " ");

    DIR *dir;
    struct dirent *dir_entry;

    dir = opendir("/proc");

    if (dir == NULL)
    {
        printf("Error opening /proc directory\n");
        return 0;
    }
    if (!f)
     printf("%5s\t\t%10s\t\t%9s\t\tCMD\n" ,"PID" , "TTY" , "TIME");

    if(f)
    printf("%-15s\t%5s\t%5s\t%s %5s %-5s\t%9s\t%s\n" , "UID" , "PID" ,"PPID" , "C" , "STIME" , "TTY" , "TIME" , "CMD");
    while ((dir_entry = readdir(dir)) != NULL)
    {

        if (allnumbers(dir_entry->d_name))
        {
            struct data features;
            char address[1000];
            sprintf(address, "/proc/%s/stat", dir_entry->d_name);
            FILE *file = fopen(address, "r");
            if(file==NULL)
            continue;
            fscanf(file, "%d %s %*c %d %*d %d %*d %*d %*u %*d %*d %*d %*d %lld %lld %*d %*d %*d %*d %*d %*d %ld %*d %*u %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d %d",
                   &features.pid, features.cmd, &features.parentid, &features.sid, &features.utime, &features.stime, &features.STIME, &features.psr, &features.c);
            features.c = features.stime / sysconf(_SC_CLK_TCK);
            fclose(file);
            sprintf(address, "/proc/%s/statm", dir_entry->d_name);
            if((file = fopen(address, "r"))==NULL)
            continue;
            fscanf(file, "%ld %llu", &features.sz, &features.rss);
            features.rss *= 4;
            fclose(file);
            sprintf(address, "/proc/%s/status", dir_entry->d_name);
            file = fopen(address, "r");
            if(file==NULL)
            continue;

            char line[256];
            while (fgets(line, sizeof(line), file))
            {
                if (sscanf(line, "Uid:\t%d", &features.uid) == 1)
                {
                    break;
                }
            }
            fclose(file);
            struct passwd *pw = malloc(sizeof(struct passwd));
            pw = getpwuid(features.uid);
            strcpy(features.username, pw->pw_name);
            sprintf(address, "/proc/%s/fd/0", dir_entry->d_name);
            int fd = open(address, O_RDONLY, 0);
            if (ttyname(fd) != NULL)
                strcpy(features.tty_name, ttyname(fd));
            else
                strcpy(features.tty_name, "?");
            close(fd);

            time_t uptime;
            if((file = fopen("/proc/uptime", "r"))==NULL)
            continue;
            fscanf(file, "%ld", &uptime);
            fclose(file);
            features.STIME = (uptime - (features.STIME) / sysconf(_SC_CLK_TCK));

            long long int PROG = (features.utime + features.stime) / sysconf(_SC_CLK_TCK);
            snprintf(features.formed_utime, 256, "%.02lld:%.02lld:%.02lld", (PROG / 3600) % 3600, (PROG / 60) % 60, PROG % 60);
            char new[1000];
            memset(new, '\0', 256);
            strcpy(new, features.cmd);
            for (int i = 1; i < strlen(new) - 1; i++)
            {
                features.cmd[i - 1] = new[i];
                features.cmd[i] = '\0';
            }
            if (features.tty_name[0] != '?')
            {
                memset(new, '\0', 256);
                strcpy(new, features.tty_name);
                memset(features.tty_name , '\0' , 256);
                for (int i = 5; i < strlen(new); i++)
                {
                    features.tty_name[i - 5] = new[i];
                }
            }

            time_t now = time(0);
            features.STIME = now - features.STIME;
            struct tm *loop = localtime(&features.STIME);
            strftime(features.formed_STIME, 256, "%H:%M", loop);
            if(e)
             printf("%4d\t\t%10s\t\t%9s\t\t%s\n", features.pid, features.tty_name, features.formed_utime, features.cmd);

            if(!af && !ef && !e && !a)
            { if(!strcmp(features.tty_name , current_tty))
            printf("%4d\t\t%10s\t\t%9s\t\t%s\n", features.pid, features.tty_name, features.formed_utime, features.cmd);
            }

            if(a)
          {  if(strcmp(features.tty_name, "?") && features.sid != features.pid && features.pid != parentid)
            {
                printf("%5d\t\t%10s\t\t%9s\t\t%s\n", features.pid, features.tty_name, features.formed_utime, features.cmd);
            }
        }


            if(af)
           { if(strcmp(features.tty_name, "?") && features.sid != features.pid && features.pid != parentid)
            {
                printf("%-15s\t%5d\t%5d\t%d %5s %-5s\t%9s\t%s\n" , features.username , features.pid ,features.parentid , features.c , features.formed_STIME , features.tty_name , features.formed_utime, features.cmd);
            }
            }

            if(ef)
            {
                printf("%-15s\t%5d\t%5d\t%d %5s %-5s\t%9s\t%s\n" , features.username , features.pid ,features.parentid , features.c , features.formed_STIME , features.tty_name , features.formed_utime, features.cmd);

            }

        }
    }
 }
}