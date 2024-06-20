/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handler.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 08:58:22 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/11 10:10:38 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

#include "libft.h"
#include "struct.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <signal.h>

void	print_tab(t_table tab);
void	print_struct(t_data *data, int tab_size);

/*===========================build_exec_path.c===============================*/

char	*ft_concat_path(char *directory, char *prompt);
char	*check_all_dirs(char *exec_searched);

/*===========================exec_utils.c===============================*/

int		ft_strcmp(char *s1, char *s2);
void	free_array(char **array);
void	wait_all(int tab_size);
int		ft_perror(char *err_message);
char	*format_exec_path(t_data *data, int i);

/*===========================builtins.c===============================*/

int		ft_cd(char **args);
void ft_exit(char **args, int last_exit_status, t_data *data, int tab_size);

/*===========================builtins_utils.c===============================*/

int		is_builtin(t_data *data, int i);
int exec_builtin(t_data *data, int tab_size);

/*===========================redirections.c===============================*/

int		redir_input(t_data *data, int i, int prev_fd);
int		redir_output(t_data *data, int i, int tab_size, int *fds);

/*===========================redirections_utils.c===============================*/

char	*skip_redir_symbol(char *token_file, bool direction);
int		arrow_count(char *str, char c);

/*===========================exec_handler.c===============================*/

int exec_handler(t_data *data, int tab_size);

int exec_single_command(t_data *data);
int	exec_multiple_commands(t_data *data, int tab_size);
void cleanup_resources(t_data *data, size_t tab_size);
void	free_struct(t_data *struc, int tab_size);




int				expand_management(t_data *data, char **envp);


#endif