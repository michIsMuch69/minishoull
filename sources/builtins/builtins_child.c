/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:28:16 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/16 09:42:28 by jean-michel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int ft_echo(char **args)
{
    int flag;
    int i;

    flag = 0;
    i = 1;
    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        flag = 1;
        i++;
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            ft_printf(" ");
        i++;
    }
    if (!flag)
        ft_printf("\n");
    return (0);
}

int	ft_env(char **env)
{
	int	i;

	i = 0;
    while (env[i])
	{
        ft_printf("%s\n", env[i]);
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
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
		return (1);
}
