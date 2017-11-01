#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include "hhlib.h"
#include "log.h"

static int log_fd;

void sig_int(int signal)
{
    switch(signal)
    {
        // only handle SIGINT
        case SIGINT:
        {
            printf("Process exit due to user exit (ctrl + c)\n");
            log_write(log_fd, 1, "Caught SIGINT, killing children and returning.");
            int status;
            // send SIGTERM to every child process with same group ID
            kill(0, SIGTERM);
            // give time to exit gracefully
            sleep(1);

            // this is buggy, only gets one child's status, how to fix?
            pid_t p;
            p = waitpid(-1, &status, WNOHANG);
            if(!WIFEXITED(status))
            {
                // if the child(s) didn't exit gracefully, send SIGKILL
                log_write(log_fd, 1, "Every child process did not exit gracefully. Trying SIGKILL.");
                printf("p: %d\n", p);
                while((p = waitpid(-1, NULL, WNOHANG)) != -1)
                {
                    kill(p, SIGKILL);
                }
            }
            log_write(log_fd, 1, "Every child terminated. Exiting.");
            exit(0);
        }
        // this is because this pid will also get the SIGTERM
        default:
            break;
    }
}


int main(int argc, char** argv)
{
    // remove old log and create a new one

    remove("cleaner.log");

    log_fd = open("cleaner.log", O_RDWR | O_APPEND | O_CREAT | O_NONBLOCK, S_IRWXU);

    log_write(log_fd, 1, "Main program init.");

    struct sigaction sa;

    sigemptyset(&sa.sa_mask);

    sa.sa_handler = &sig_int;

    // the sig_int only handles these 2 signals
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // chech that the program was opened appropriately
    if(argc == 1)
    {
        printf("usage: %s filename1 filename 2 ...\n", argv[0]);
        log_write(log_fd, 1, "The main program was run without any parameters. Exiting.");
        return -1;
    }

    pid_t pid;

    // fork childs, exec one program with each child
    for(int i = 1; i < argc; i++)
    {
        pid = fork();
        if(pid == -1)
        {
            perror("Fork failed!\n");
            return -1;
        }
        // if child, exec another program (cleaner)
        if(pid == 0)
        {
            log_write(log_fd, 2, "Opening a new cleaner instance with filename: ", argv[i]);

            char* args[] = {"./cleaner", argv[i], NULL};

            execvp(args[0], args);
            
            return 0;
        }
    }
    //sleep(20);
    // Wait for children to return before exiting the parent program
    for(int i = 1; i < argc; i++)
    {
        if(pid != 0)
        {
            if(waitpid(-1, NULL, 0) <= 0)
            {
                perror("waitpid failed");
            }
        }
    }
	log_write(log_fd, 1, "Every cleaner returned. Exiting main program.");

	return 0;
}