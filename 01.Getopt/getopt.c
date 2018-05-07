// -s  Процесс становится лидером группы. Подсказка: смотриии setpgid(2).
// -u  Печатает значение ulimit
// -Unew_ulimit  Изменяет значение ulimit. Подсказка: смотри atol(3C) на странице руководства strtol(3C)
// -c  Печатает размер в байтах core-файла, который может быть создан.
// -Csize  Изменяет размер core-файла

#include <stdio.h>     // printf()
#include <unistd.h>    // getuid(), geteuid()
#include <string.h>    // strcmp()
#include <sys/types.h> // uid_t
#include <stdlib.h>    // getenv()
#include <ulimit.h>    // ulimit()

int execute_command(char *command);

int main(int argc, char *argv[], char *envp[])
{
    unsigned i;
    for (i = 1; i < argc; i++)
    {
        int execute_status = execute_command(argv[i]);
        if (execute_status == -1)
        {
            printf("Syscall Error: %s\n", argv[i]);
        }
        if (execute_status == 1)
        {
            printf("Format error: '%s'", argv[i]);
        }
    }
    return EXIT_SUCCESS;
}

int execute_command(char *command)
{

    if ((strlen(command) < 2) && (command[0] != '-'))
    {
        return 1;
    }

    if (command[1] == 'i')
    {
        printf("user_id=[%d], user_eid=[%d]\n", getuid(), geteuid());
        return 0;
    }
    if (command[1] == 'p')
    {
        printf("pid=[%d], parent_pid=[%d], group_id=[%d]\n",
               getpid(), getppid(), getgid());
        return 0;
    }
    if (command[1] == 'u')
    {
        printf("%li\n", ulimit(UL_GETFSIZE, 0));
        return 0;
    }

    if (command[1] == 'v')
    {
        unsigned i = 1;
        char *s = *__environ;

        for (; s; i++)
        {
            printf("%s\n", s);
            s = *(__environ + i);
        }

        return 0;
    }

    if (command[1] == 'V')
    {
        if (strlen(command) <= 2)
        {
            return 1;
        }
        else
        {
            putenv(command + 2);
        }
        return 0;
    }

    if (command[1] == 'd')
    {
        char *pwd = getenv("PWD");
        if (pwd == NULL)
        {
            printf("PWD not set\n");
            return -1;
        }
        else
        {
            printf("%s\n", pwd);
        }
        return 0;
    }
    return 1;
}