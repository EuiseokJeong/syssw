#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_LEN_LINE    10
#define LEN_HOSTNAME    30

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;
    char *p;


    while (true) {
        char *s;
        int len;

        char hostname[LEN_HOSTNAME + 1];
        memset(hostname, 0X00, sizeof(hostname));
        gethostname(hostname, LEN_HOSTNAME);
        printf("%s@%s$", getpwuid(getuid())->pw_name, hostname);

        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }

        if(strcmp(args[0], "exit\n")==0){
                return 0;
        }

        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        printf("[%s]\n", command);
        p=strtok(command, ";");
        while(p){
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        }
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status));
            }
        }
        else {  /* child */
            args[0]=p;
             ret = execve(args[0], args, NULL);
            printf("%d", ret);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");
                return 1;
        }
        }
        p=strtok(NULL, ";");
        }
    }
    return 0;
 }
