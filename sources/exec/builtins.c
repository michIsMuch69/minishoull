/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 08:39:26 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/20 18:17:44 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// int ft_echo(char **args);
// int ft_cd(char **args);
// int ft_export(char **args);
// int ft_unset(char **args);
// int ft_exit(void);
// int ft_env(char **env);
// int	ft_pwd(void);



void ft_exit(char **args, int last_exit_status, t_data *data, int tab_size)
{
    int exit_status = last_exit_status;

    if (args[1] != NULL) {
        // Check if the argument is a valid number
        char *endptr;
        exit_status = strtol(args[1], &endptr, 10);
        if (*endptr != '\0') {
            fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
            exit_status = 255;
        }
    }
    
    // Perform cleanup
    cleanup_resources(data, tab_size);

    // Print message if interactive shell
    if (isatty(STDIN_FILENO)) {
        printf("Exiting shell\n");
    }

    // Terminate child processes if any
    kill(0, SIGTERM);
    exit(exit_status);
}

void cleanup_resources(t_data *data, size_t tab_size)
{
    // Free allocated memory
    free_struct(data, tab_size);
    
    // Additional cleanup if necessary
    // close_file_descriptors();  // Example function, implement if needed
    // free_other_resources();    // Example function, implement if needed
}

// int	ft_env(char **env)
// {
// 	int	i;

// 	i = 0;
// 	while (env[i])
// }

int	ft_cd(char **args)
{
	if (args[1] == NULL)
		return (ft_perror("No arguments to cd\n"), -1);
	else
	{
		ft_printf("This is my own cmd CD\n");
		if (chdir(args[1]) != 0)
			perror("cd");
	}
	return (0);
}

