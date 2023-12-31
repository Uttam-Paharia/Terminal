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
#include <ctype.h>
#include <grp.h>
void myls(char* command, char*pwd);
void mymv(char* command);
void myps(char* command);
char command[1000];
char *prompt;
void mycp(char *command);
void mygrep(char *command);


char *Dir()
{
    char *buf;
    buf = (char *)malloc(1000 * sizeof(char));
    getcwd(buf, 1000);
    return buf;
}
void strip_extra_spaces(char* str) {
  int i, x;
  for(i=x=0; str[i]; ++i)
    if(!isspace(str[i]) || (i > 0 && !isspace(str[i-1])))
      str[x++] = str[i];
  str[x] = '\0';
}

void trimLeadingandTrailing(char *s)
{
	int  i,j;
 
	for(i=0;s[i]==' '||s[i]=='\t';i++);
		
	for(j=0;s[i];i++)
	{
		s[j++]=s[i];
	}
	s[j]='\0';
	for(i=0;s[i]!='\0';i++)
	{
		if(s[i]!=' '&& s[i]!='\t')
			j=i;
	}
	s[j+1]='\0';
}
int main()
{

    char *user = malloc(1000 * sizeof(char)), *host = malloc(1000 * sizeof(char));
    getlogin_r(user, 1000);
    gethostname(host, 100);
    while (1)
    {

        prompt = (char *)malloc(10000 * sizeof(char));
        prompt[0] = '\0';
        strcat(prompt, "\001\e[01;31m\012");
        strcat(prompt, user);
        strcat(prompt, "@");
        strcat(prompt, host);
        strcat(prompt, "\001\e[0m\002");
        strcat(prompt, ":");
        strcat(prompt, "\001\e[01;33m\002");
        strcat(prompt, Dir());
        strcat(prompt, ":");
        strcat(prompt, "\001\e[0m\002");
        strcpy(command, readline(prompt));
        char control[2000], str[2000];
        
        if (strlen(command) > 0)
        {
            add_history(command);
        }
        strip_extra_spaces(command);
        trimLeadingandTrailing(command);

        strcpy(control,command);
        for(int i=0;i<strlen(control);i++)
        {
            control[i]=tolower(control[i]);
        }

        if (!strcmp(command, "exit") || !strcmp(command, "quit"))
        {
            break;
        }
        int k = 0;
        
        if(strlen(command)>=4 && command[0]=='m' && command[1]=='y' && command[2]=='l' && command[3]=='s'  )
        {
            myls(command,Dir());
        }
        else if ( strlen(command)>=4 &&command[0]=='m' && command[1]=='y' && command[2]=='c' && command[3]=='p' )
        {
            

            mycp(command);
        }
        else if ( strlen(command)>=4 &&command[0]=='m' && command[1]=='y' && command[2]=='m' && command[3]=='v' )
        {
            mycp(command);
        }
        else if (strlen(command)>=6 &&command[0]=='m' && command[1]=='y' && command[2]=='g' && command[3]=='r' && command[4]=='e' && command[5]=='p' )
        {
            mygrep(command);
        }
        else if ( strlen(command)>=4 &&command[0]=='m' && command[1]=='y' && command[2]=='p' && command[3]=='s' )
        {
            myps(command);
        }
        
         else if( control[0]=='h' && control[1]=='e' && control[2]=='l' && control[3]=='p')
        {
            char arr[10];
            arr[0]='\0';
            if(command[4]!='\0')//cause terminating spaces are already removed
            {
            for(int k=5;k<strlen(control);k++)
            {
               arr[k-5]=command[k];
               
               arr[k-4]='\0';
               

            }
            }
            
            if(strlen(arr)==0)
            {
                printf("functions that can be implemented are \n"
                "myps:for ps function to know more about it type help myps\n"
                "myls:for ls function to know more about it type help myls\n"
                "mycp:for cp function to know more about it type help mycp\n"
                "mymv:for mv function to know more about it type help mymv\n"
                "mygrep:for grep function to know more about it type help mygrep\n");
            }
            else if(!strcmp("mycp",arr))
            {
               printf("NAME\n"
       "cp - copy files and directories"
      " flags implemented are i(interogative), v(verbose), n(no_cobbler\n)"
      "-n\n"
             " do not overwrite an existing file (overrides a previous  -i  option\n)"
             "-i\n "
             "prompt before overwrite (overrides a previous -n option\n)"
             "-v, --verbose\n"
              "explain what is being done\n"
      "SYNOPSIS:\n"
      "cp [OPTION]... SOURCE... DIRECTORY/destination\n"
                      "give command in format of mycp -options source_file destination_file/directory or else it will not work.\n"
                      "code copies only one file to another you can't use it for multiple files.\n"
                      "also command like mycp -vni f1 f2 works but permuting it like mycp -i -v f1 f2 doesn't works .\n"
                      "if you try to copy it in home directory code will not work\n");

            }
            else if(!strcmp("myls",arr))
            {
                printf("NAME\n"
       "ls - list directory contents\n"

"SYNOPSIS"
       "ls [OPTION]... [FILE]...\n"

"DESCRIPTION"
       "List  information  about  the FILEs (the current directory by default).\n"
      " Sort entries alphabetically if none of -cftuvSUX nor --sort  is  specified.\n"
      "option implemented:\n"
      "-l     use a long listing format\n"
      "-S     sort by file size, largest first\n"
      "-t     sort by time, newest first; see --time\n"
      "-r reverse order while sorting\n"
      " -a do not ignore entries starting with .\n"
      " multiple options dont work in format -a -r etc but will work in format of -St etc.\n");
              

       

            }
            else if (!strcmp("mymv",arr))
            {
                printf("NAME\n"
       "mp - move files and directories\n"
      " flags implemented are i(interogative), v(verbose), n(no_cobbler)\n"
      "-n\n"
             " do not overwrite an existing file (overrides a previous  -i  option\n)"
             "-i \n"
             "prompt before overwrite (overrides a previous -n option)\n"
             "-v, --verbose\n"
              "explain what is being done\n"
      "SYNOPSIS:\n"
      "mv [OPTION]... SOURCE... DIRECTORY/destination\n"
                      "give command in format of mymv -options source_file destination_file/directory or else it will not work.\n"
                      "code copies only one file to another you can't use it for multiple files.\n"
                      "also command like mymv -vni f1 f2 works but permuting it like mymv -i -v f1 f2 doesn't works .\n"
                      "if you try to move it in home directory code will not work\n");
            }
            else if (!strcmp("mygrep",arr))
            {
                printf("DESCRIPTION\n"
       "grep  searches  for  PATTERNS  in  each  FILE.  PATTERNS is one or more\n"
       "patterns separated by newline characters, and  grep  prints  each  line\n"
       "that  matches a pattern.  Typically PATTERNS should be quoted when grep\n"
       "is used in a shell command.\n"

       "A FILE of '-'  stands  for  standard  input.   If  no  FILE  is  given,\n"
       "recursive  searches  examine  the  working  directory, and nonrecursive\n"
       "searches read standard input."
       "options implemented are:\n"
       "-w\n"
              "Select  only  those  lines  containing  matches  that form whole\n"
              "words.  The test is that the matching substring must  either  be\n"
              "at  the  beginning  of  the  line,  or  preceded  by  a non-word\n"
              "constituent character.  Similarly, it must be either at the  end\n"
              "of  the  line  or  followed by a non-word constituent character.\n"
              "Word-constituent  characters  are  letters,  digits,   and   the\n"
              "underscore.  This option has no effect if -x is also specified.\n"
        "-c\n"
              "Suppress  normal output; instead print a count of matching lines\n"
              "for each input file.  With the -v,  --invert-match  option  (see"
              "below), count non-matching lines.\n"
         "-n\n"
              "Prefix each line of output with the 1-based line  number  within\n"
              "its input file.\n"     
            "permutations like -nwc works but -n -c -w don't work.\n");
            }
            else if(!strcmp("myps",arr))
            {
                printf(" ps displays information about a selection of the active processes.  If\n"
       "you want a repetitive update of the selection and the displayed\n"
      " information, use top instead.\n"

       "This version of ps accepts several kinds of options:\n"

       "1   UNIX options, which may be grouped and must be preceded by a dash.\n"
       "2   BSD options, which may be grouped and must not be used with a dash.\n"
       "3   GNU long options, which are preceded by two dashes.\n"
       "options implimented :\n"
       "-a     Select all processes except both session leaders (see getsid(2))\n"
              "and processes not associated with a terminal.\n"
              "-e     Select all processes.\n"
              "-af and -ef\n"
             "only these options will be implemented no more permutations \n"); 


            }
            else
            {
                printf("Command not found!!\n");
            }
            
            
        }
        else
        system(command);
        if (control[0] == 'c' && control[1] == 'd' && control[2] == ' ')//change directory
        {
            for (int i = 3; control[i] != '\0'; i++)
            {
                str[i - 3] = control[i];
                k++;
            }
            str[k] = '\0';
            if (chdir(str) == -1)
            {
                printf("Invalid command\n");
            }            
        }
        

            
    }
}