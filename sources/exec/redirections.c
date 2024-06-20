/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */ 
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 12:47:54 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/18 10:33:09 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

static int here_docs(char *delimiter)
{
    char *prompt;
    int fd2;

    fd2 = open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 == -1) {
        perror("Failed to open temp file for here-doc");
        return -1;
    }
    while (1)
    {
        prompt = readline(">");
        if (strcmp(prompt, delimiter) == 0) {
            free(prompt);
            break;
        } else {
            ft_putstr_fd(prompt, fd2);
            ft_putstr_fd("\n", fd2);
        }
        free(prompt);
    }
    close(fd2);
    fd2 = open("temp.txt", O_RDONLY);
    if (fd2 == -1) {
        perror("Failed to reopen temp file for here-doc");
        return -1;
    }
    return fd2;
}

static int define_input_fd(t_data *data, int i)
{
    int input_fd;
    char *input_file;
    char *delimiter;

    printf("define_input_fd: i = %d\n", i);

    if (data[i].input.tab != NULL && arrow_count(data[i].input.tab[0], '<') == 1)
    {
        input_file = skip_redir_symbol(data[i].input.tab[0], 0);
        if (access(input_file, F_OK) == -1) {
            perror("File doesn't exist");
            return -1;
        }
        input_fd = open(input_file, O_RDONLY);
    }
    else if (data[i].input.tab != NULL && arrow_count(data[i].input.tab[0], '<') == 2)
    {
        delimiter = skip_redir_symbol(data[i].input.tab[0], 0);
        input_fd = here_docs(delimiter);
        unlink("temp.txt");
    }
    else
    {
        input_fd = STDIN_FILENO;
    }
    printf("define_input_fd: input_fd = %d\n", input_fd);
    return input_fd;
}

static int define_output_fd(t_data *data, int i)
{
    int output_fd;
    char *output_file;

    printf("define_output_fd: i = %d\n", i);

    if (data[i].output.tab != NULL) {
        output_file = skip_redir_symbol(data[i].output.tab[0], 1);
        if (arrow_count(data[i].output.tab[0], '>') == 1)
        {
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_fd == -1) {
                perror("Failed to open output file for truncation");
            }
        }
        else if (arrow_count(data[i].output.tab[0], '>') == 2)
        {
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (output_fd == -1) {
                perror("Failed to open output file for appending");
            }
        }
        else
        {
            output_fd = STDOUT_FILENO;
        }
    } else {
        output_fd = STDOUT_FILENO;
    }
    printf("define_output_fd: output_fd = %d\n", output_fd);
    return output_fd;
}

int redir_input(t_data *data, int i, int prev_fd)
{
    int input_fd;

    printf("redir_input: i = %d, prev_fd = %d\n", i, prev_fd);

    input_fd = define_input_fd(data, i);
    if (input_fd == -1) {
        perror("Failed to open input file");
        return -1;
    }
    if (dup2(input_fd, STDIN_FILENO) == -1) {
        perror("Failed to redirect input");
        close(input_fd);
        return -1;
    }
    if (input_fd != STDIN_FILENO)
        close(input_fd); // Ensure file descriptor is closed after duplication
    if (prev_fd != 0 && prev_fd != STDIN_FILENO)
        close(prev_fd); // Ensure previous file descriptor is closed
    printf("redir_input: input redirection completed.\n");
    return 0;
}

int redir_output(t_data *data, int i, int tab_size, int *fds)
{
    int output_fd;

    printf("redir_output: i = %d, tab_size = %d\n", i, tab_size);

    output_fd = define_output_fd(data, i);
    if (output_fd == -1) {
        perror("Failed to open output file");
        return -1;
    }
    if (i < tab_size - 1) {
        if (dup2(fds[1], STDOUT_FILENO) == -1) {
            perror("Failed to duplicate pipe fd for output");
            close(output_fd);
            return -1;
        }
        close(fds[1]); // Ensure pipe file descriptor is closed
    } else {
        if (dup2(output_fd, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            close(output_fd);
            return -1;
        }
        if (output_fd != STDOUT_FILENO)
            close(output_fd); // Ensure file descriptor is closed after duplication
    }
    printf("redir_output: output redirection completed.\n");
    return 0;
}
