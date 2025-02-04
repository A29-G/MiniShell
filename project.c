#include"main.h"
void execute_external_commands(char *input_string);
char *external[200];
char prompt[100]="minishell$";
int pid=-1;
int status;
FILE *fptr;

void my_handler()
{
    if (pid == -1) 
    {
        printf("\n%s",prompt);
        fflush(stdout);
    }
}

int main()
{
    char input_string[100];
    system("clear");
    scan_input(prompt,input_string);
}

char *get_command(char *input_string)
{
    int i=0;
    while(input_string[i]==' ')
    {
        for(int j=i;input_string[j]!='\0';j++)
        {
            input_string[j]=input_string[j+1];
        }
    }

    char *atr=strchr(input_string,' ');
        
    int len=atr-input_string;
    //create the array dynamically
    char *buffer=malloc((len+1)*sizeof(char));
    if(buffer==NULL)
    {
        perror("Failed to create Dynamic array\n");
        exit(0);
    }
    
    strncpy(buffer, input_string,len);   //store first word from the input_string to new array
    buffer[len]='\0';
    return buffer;                      // return the address of the newly created array
}

void scan_input(char *prompt,char *input_string)
{
    extract_external_commands(external);
    signal(SIGINT,my_handler);
    signal(SIGTSTP,my_handler);
    while(1)
    {
        printf("%s", prompt);

        __fpurge(stdin);
        scanf("%[^\n]",input_string);
        if(strncmp( input_string,"PS1=",4)==0)     //validate the i/p PS1 variable or not
        {                                           //=>valid : change the prompt
            if(input_string[4]!=' ')
            {
                strcpy(prompt,input_string+4);
            }
        }
        // else
        // {
        //     printf("command not found\n");
        // }

         char *cmd=get_command(input_string);

         int ret=check_command_type(cmd);
         if(ret==EXTERNAL)
        {
            int status;
            pid=fork(); //create the child process
            if(pid > 0)    //parent
            {
                waitpid(pid,&status,WUNTRACED);    //==>waitpid(child_pid,&status,WUNTRACED);
                pid = -1;
            }
            else if(pid == 0)                     //child
            {
                signal(SIGINT,SIG_DFL);
                signal(SIGTSTP,SIG_DFL);

                execute_external_commands(input_string);       //==>execute_external_command(input_string);
                exit(0);   //exit(0);
            }
        }

        if(ret==BUILTIN)
        {
            execute_internal_commands(input_string);
        }
    }
}

int check_command_type(char *command)
{
	char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL
                        };

    for(int i=0;builtins[i]!=NULL;i++)           //check the command is present int the built in array or not 
    {
        if(strcmp(builtins[i],command)==0)
        {
            return BUILTIN;                      //if it is present ==> return BULTIN
        }
    }
    //if it is not present

    //check the command is present in the external array or not
   for (int i = 0; external[i] != NULL; i++) 
   {
        if (strcmp(external[i], command) == 0) 
        {
            return EXTERNAL; // Return EXTERNAL if found
        }
    }
    return  NO_COMMAND;     //return NO_COMMAND if it is not both internal and external                   
}

void extract_external_commands(char **external_commands)
{
    for(int i=0;i<200;i++)
    {
        external_commands[i]=malloc((30)*sizeof(int));
        if(external_commands[i]==NULL)
        {
            perror("Failed to allocate the memory\n");
            exit(0);
        }
    }

      fptr=fopen("program.txt","r");
    if(fptr==NULL)
    {
        perror("Failed to open the file\n");
        exit(0);
    }
    char buffer[100];
    int i=0;
    while(i<200 && fgets(buffer, sizeof(buffer), fptr) != NULL)
    {
        int len=strlen(buffer);
        buffer[len-2]='\0';
        strcpy(external_commands[i++],buffer);
    }
    fclose(fptr);
}

void execute_external_commands(char *input_string)
{
    char *argv[100];
    for(int i=0;i<100;i++)
    {
        argv[i]=malloc((30)*sizeof(char));
        if(argv[i]==NULL)
        {
            perror("Failed to allocate the memory\n");
            exit(0);
        }
    }           
    
    int i=0,k1=0,j=0;
    while(input_string[i]!='\0')
    {                                  //convert the input_string into an array of pointer
        if(input_string[i]==' ')
        {
            if(i>j)
            {
                strncpy(argv[k1],input_string+j, i-j);
                argv[k1][i-j]='\0';
                k1++;
            }
            j=i+1;
        }
        i++;
    }

    if(i>j)
    {
        strncpy(argv[k1],input_string+j, i-j);
        argv[k1][i-j]='\0';
        k1++;
    }
    argv[k1]=NULL;

    int *buffer = malloc((k1)*sizeof(int));
    int k=0,flag=0;
    for(int i=0;i<k1;i++)
    {
        if((i==0) && strcmp(argv[i],"|")!=0)
            buffer[k++]=i;
        if(strcmp(argv[i],"|")==0)      //check the array of pointer have  pipe or not
        {
            flag=1;
            argv[i]=NULL;
            if(argv[i+1]!=NULL)
                buffer[k++]=i+1;
        }
    }

    if(flag == 1)                        //if the pipe present
    {                                    //==>n_pipe();
        int fd[2];
        int ret;
        for(int i=0;i<k;i++)
        {
            if(i!=k-1)
            {
                pipe(fd);
            }

            ret=fork();             //creates child process
            if(i!=k-1)
            {
                if(ret>0)
                {
                    close(fd[1]);
                    dup2(fd[0],0);
                    close(fd[0]);
                }
            }

            if(ret == 0)
            {
                if(i!=k-1)
                {
                    close(fd[0]);
                    dup2(fd[1],1);
                }
                execvp(argv[buffer[i]],argv+buffer[i]); //call exec system call
                exit(0);
            }
        }

        for (int i = 0; i < k; i++) 
        {
            wait(&status);
        }
    }
    else                    //if the pipe is not present 
    {
        execvp(argv[0],argv);               //exec() system call to execute the command
    }
}

void execute_internal_commands(char *input_string)
{

    if(strcmp(input_string,"exit")==0)  //checks exit command or not
    {
        exit(0);
    }

    if(strcmp(input_string,"pwd")==0)   //checks pwd command or not
    {
        char buf[200];
        getcwd(buf,200);
        printf("%s\n",buf);
    }
    
    if (strncmp(input_string, "cd", 2) == 0) //checks cd command or not
    {
        char *buffer;
        char *token = strtok(input_string, " ");
        while (token != NULL) 
        {
            buffer=token;
            token = strtok(NULL, " ");
        }
        chdir(buffer); 
    }

    if(strncmp(input_string,"echo ",4)==0)  //checks echo command or not
    {
        char *st;
        if(strcmp(input_string,"echo $$")==0)
            printf("%u\n",getpid());            //print the pid of process

        if(strcmp(input_string,"echo $?")==0)
            printf("%d\n",WEXITSTATUS(status))  ;    //print the status of process

        if((strcmp(input_string,"echo $SHELL")==0))
            printf("%s\n",getenv("SHELL"));
    }
}