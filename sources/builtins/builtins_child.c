/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michismuch <michismuch@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:28:16 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/15 17:55:06 by michismuch       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int ft_echo(char **args)
{
    int flag = 0;
    int i = 1;

    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        flag = 1;
        i++;
    }
    while (args[i])
    {
        printf("%s", args[i]);
        i++;
    }
    if (!flag)
        printf("\n");
    return (0);
}

int	ft_env(char **env)
{
	int	i;

	i = 0;
    while (env[i])
	{
        printf("%s\n", env[i]);
		i++;
	}
    return 0;
}



int	ft_pwd(void)
{
	char	cwd[1024];
	char *temp;

	ft_printf("My pwd\n"); //last exit code
	temp = getcwd(cwd, sizeof(cwd));
	if (temp != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		return (1);
	}
}
