/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_parent.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 08:27:25 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/18 10:31:51 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int ft_cd(char **args, char **env)
{
    char cwd[1024];
    char *new_dir = NULL;

    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return (perror("getcwd"), -1);
    if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
        if (ft_getenv("HOME", env, &new_dir) != 0)
            return (ft_putstr_fd("cd: HOME not set\\n", 2), -1);
    }
	else if (ft_strcmp(args[1], "-") == 0)
	{
        if (ft_getenv("OLDPWD", env, &new_dir) != 0)
            return (ft_putstr_fd("cd: OLDPWD not set\\n", 2), -1);
        ft_printf("%s\\n", new_dir); 
    }
	else
        new_dir = args[1];
    if (chdir(new_dir) != 0)
        return (perror("cd"), -1);
    set_env("OLDPWD", cwd, env);
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        set_env("PWD", cwd, env);
    else
        perror("getcwd");
    return (0);
}



//copie pour export, copie env.
//buffer static export (t_table)
//si entree non complete, copie unisuqment dans export + error message print_error
//si entree complete : ok pour copie dans env ++ export.

int ft_unset(char *var, t_table *env)
{
    int i;
    int j;

    i = 0;
    j = 0;
	printf("My Unset\n");
	
    while (env->tab[i])
    {
        if (ft_strncmp(env->tab[i], var, ft_strlen(var)) == 0)
        {
            free(env->tab[i]);
            j = i;
            while (env->tab[j])
            {
                env->tab[j] = env->tab[j + 1];
                j++;
            }
    		env->tab[j + 1] = NULL;
            env->size--;
            return (0);
        }
        i++;
    }
    return (1);
}

void	ft_exit(t_data *data, int tab_size, int last_status)
{
	int	status;

	printf("My exit\n");
	status = 0;
	if (data->args.tab[1])
	{
		if (is_numeric_str(data->args.tab[1]) == 0)
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			status = 255;
		}
		else
		{
			status = ft_atoi(data->args.tab[1]);
			status = status % 256;
			if (status < 0)
				status += 256;
			printf("Status in ft_exit = %d\n", status);
		}
	}
	else
	{
		printf("Status in ft_exit no agrs = %d\n", status);
		status = last_status;
	}
	free_struct(data, tab_size);
	exit(status);
}

//close_fds --> input output files.
//free_pipes -->  close pipes, free pipe_ptr.
