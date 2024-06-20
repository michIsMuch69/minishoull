/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */ 
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:46:39 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/20 20:12:44 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "expand.h"
#include <stdio.h>

int exec_handler(t_data *data, int tab_size)
{
    printf("exec_handler: tab_size = %d\n", tab_size);
    // Check for built-in commands first
    if (exec_builtin(data, tab_size) == 0) {
        printf("Built-in command executed.\n");
        return 0;  // Built-in executed, no need to proceed
    }

    // If there are multiple commands, handle them appropriately
    if (tab_size > 1) {
        printf("Executing multiple commands.\n");
        return exec_multiple_commands(data, tab_size);
    }

    // Otherwise, execute a single command
    printf("Executing single command.\n");
    return exec_single_command(data);
}

int exec_single_command(t_data *data)
{
    pid_t pid;
    char *cmd_path;

    printf("exec_single_command: Executing single command.\n");

    cmd_path = format_exec_path(data, 0);
    if (!cmd_path) {
        perror("Command not found");
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        free(cmd_path);
        return 1;
    }
    if (pid == 0) {
        printf("Child process: Executing command: %s\n", cmd_path);
        if (redir_input(data, 0, 0) == -1) {
            exit(1);
        }
        printf("Child process: Input redirection done.\n");
        if (redir_output(data, 0, 1, NULL) == -1) {
            exit(1);
        }
        printf("Child process: Output redirection done.\n");
        execve(cmd_path, data[0].args.tab, data[0].env.tab);
        perror("execve failed");
        exit(1);
    }
    waitpid(pid, NULL, 0);
    free(cmd_path);
    return 0;
}

int exec_multiple_commands(t_data *data, int tab_size)
{
    int fds[2];
    int prev_fd = 0;
    pid_t pid;
    int i = 0;

    while (i < tab_size)
    {
        if (pipe(fds) == -1) {
            perror("Pipe failed");
            return 1;
        }

        pid = fork();
        if (pid == -1) {
            perror("Fork failed");
            close(fds[0]);
            close(fds[1]);
            return 1;
        }

        if (pid == 0) { // Child process
            printf("Child process %d: Starting execution.\n", i);
            close(fds[0]);

            if (redir_input(data, i, prev_fd) == -1) {
                perror("Child process redir_input failed");
                exit(1);
            }
            printf("Child process %d: Input redirection done.\n", i);
            if (i < tab_size - 1) {
                if (dup2(fds[1], STDOUT_FILENO) == -1) {
                    perror("Failed to duplicate pipe fd for output");
                    exit(1);
                }
                close(fds[1]);
            } else {
                if (redir_output(data, i, tab_size, fds) == -1) {
                    perror("Child process redir_output failed");
                    exit(1);
                }
            }
            printf("Child process %d: Output redirection done.\n", i);

            exec_handler(data, 1);
            perror("exec_handler failed");
            exit(1);
        } else { // Parent process
            close(fds[1]);
            if (prev_fd != 0) {
                close(prev_fd);
            }
            prev_fd = fds[0];
            waitpid(pid, NULL, 0);
            i++;
        }
    }
    if (prev_fd != 0) {
        close(prev_fd);
    }
    return 0;
}
