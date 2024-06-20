/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 10:46:50 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/20 18:10:17 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	is_builtin(t_data *data, int i)
{
	// if (ft_strcmp(data[i].args.tab[0], "echo") == 0)
	// 	return (1);
	if (ft_strcmp(data[i].args.tab[0], "cd") == 0)
		return (1);
	// if (ft_strcmp(data[i].args.tab[0], "pwd") == 0)
	// 	return (1);
	// if (ft_strcmp(data[i].args.tab[0], "export") == 0)
	// 	return (1);
	// if (ft_strcmp(data[i].args.tab[0], "unset") == 0)
	// 	return (1);
	// if (ft_strcmp(data[i].args.tab[0], "env") == 0)
	// 	return (1);
	if (ft_strcmp(data[i].args.tab[0], "exit") == 0)
		return (1);
	return (0);
}

int exec_builtin(t_data *data, int tab_size)
{
    if (ft_strcmp(data->args.tab[0], "exit") == 0)
	{
        ft_exit(data->args.tab, 0, data, tab_size);  // Assuming the last exit status is 0 for simplicity
        return 0;
    }
    // Add other built-in command handlers here
    return 1;  // Return 1 if the command is not a built-in
}