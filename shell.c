#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LEN_LINE    100
#define LEN_HOSTNAME    30
#define CRT_SECURE_NO_WARNINGS
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"


int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;

    printf(ANSI_COLOR_YELLOW "***** May I help you? *****\n");
    printf(ANSI_COLOR_RED "***** Separate multiple programs with semicolon and space bar. *****\n" ANSI_COLOR_RESET);

    while (true) {
        char *s;
        int len;
        char *prognum[5] = {NULL, };
        int prognum_count=0;
        //uid_t user_id;
        //struct passwd *user_pw;
        //char hostname[LEN_HOSTNAME +1];
        //memset(hostname, 0x00, sizeof(hostname));
        //user_id = getuid();
        //user_pw = getpwuid(user_id);
        //gethostname(hostname, LEN_HOSTNAME);
        //printf("%s@%s $ ", user_pw->pw_name, hostname);

	char hostname[LEN_HOSTNAME + 1];
	memset(hostname, 0x00, sizeof(hostname));
        gethostname(hostname, LEN_HOSTNAME);
	printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_MAGENTA "@" ANSI_COLOR_CYAN "%s $ " ANSI_COLOR_RESET, getpwuid(getuid())->pw_name, hostname);

        s = fgets(command, MAX_LEN_LINE, stdin);
        len = strlen(command);
	if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
//////////////////////////////////////////////////////
/*	switch (s){
	case 'NULL':
		fprintf(stderr, "fgets failed\n");
            	exit(1);
		break;
	case 'exit':
		printf(ANSI_COLOR_YELLOW "***** Good bye *****\n" ANSI_COLOR_RESET);
           	break;
	default:
		prognum_count += 1;
		prognum[0] = command;
	}
*/
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        else if(!strcmp(s, "exit")){
            printf(ANSI_COLOR_YELLOW "***** Good bye *****\n" ANSI_COLOR_RESET);
            break;
        }
	else if(strstr(s, "; ") != NULL){
		int i = 0;
		char *p = strtok(command, "; ");
		while(p != NULL){
			prognum[i] = p;
			i++;
			prognum_count +=1 ;
			p = strtok(NULL, "; ");
		}
	}
        else{
		prognum_count += 1;
		prognum[0] = command;
	}

	//printf("주소값:%d",prognum[1]);
	//printf("주소값:%d",prognum[0]);

	for(int i=0; i < 5; i++) {
		if(prognum[i] != NULL) {
			printf("[%s]\n", prognum[i]);
		}
	}
	
	for(int i=0;i<prognum_count;i++) {//프로그램 수 만큼 반복
		args[0] = prognum[i];
		printf("%s\n", args[0]);
		pid = fork();
		if (pid < 0) {
		    fprintf(stderr, "fork failed\n");
		    exit(1);
		} 
		if (pid != 0) {   /* parent */ 
		    cpid = waitpid(pid, &status, 0);
		    if (cpid != pid) {
			fprintf(stderr, "waitpid failed\n");        
		    }
		    printf("Child process terminated\n");
		    if (WIFEXITED(status)) {
			printf("Exit status is %d\n", WEXITSTATUS(status)); 
		    }
		}
		else {   /* child */
		    ret = execve(args[0], args, NULL);
		    if (ret < 0) {
			fprintf(stderr, "execve failed\n");   
			return 1;
		    }
		}
	}
    }
    return 0;
}
