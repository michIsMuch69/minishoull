/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:39:26 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/12 10:39:06 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"



int parent_routine(t_data *data, int i, int tab_size, int **fd);

int	exec_builtin_child(t_data *data, int tab_size, int i, int **fd)
{
	int	status;

	status = 0;
	if (ft_strcmp(data->args.tab[0], "pwd") == 0)
	{
		status = ft_pwd();
	}
	else if (ft_strcmp(data->args.tab[0], "echo") == 0)
	{
	    status = ft_echo(data->args.tab);
	}
	else if (ft_strcmp(data->args.tab[0], "env") == 0)
	{
	    status = ft_env(data->env.tab);
	}
	close_pipes(fd, tab_size - 1, i, 0);
	while (i < tab_size)
	{
		close_fds(data[i].in_out_fd);
		i++;
	}
	free_struct(data, tab_size);
	free_pipes(fd, tab_size - 1);
	exit(status);
}

int exec_builtin_parent(t_data *data, int tab_size, int i, int **fd)
{
    if (ft_strcmp(data[i].args.tab[0], "cd") == 0)
    {
        data[0].exit_status = ft_cd(data[i].args.tab, data[i].env.tab);
    }
    else if (ft_strcmp(data[i].args.tab[0], "exit") == 0)
    {
        ft_exit(data, tab_size, data[i].exit_status);
    }
    else if (ft_strcmp(data[i].args.tab[0], "export") == 0)
    {
        // if (g_exported_env == NULL)
        // {
        //     init_exported_env(data);
        // }

        if (data[i].args.tab[1] == NULL)
        {
            data[0].exit_status = ft_export_print(); // (data[0].env)
        }
        else
        {
            data[0].exit_status = ft_export(data[i].args.tab, data); // export : (data, i tab_size);
        }
    }
    else if (ft_strcmp(data[i].args.tab[0], "unset") == 0)
        data[0].exit_status = ft_unset(data[i].args.tab[1], &(data[i].env));
    parent_routine(data, i, tab_size, fd);
    close_fds(data[i].in_out_fd);
    return (data[0].exit_status);
}

