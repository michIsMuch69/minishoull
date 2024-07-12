/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:24:02 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/12 10:52:41 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void init_exported_env(t_data *data)
{
    int i;

	i = 0;
    g_exported_env = malloc((data->env.size + 1) * sizeof(char *));
    if (!g_exported_env)
        return;
    while (i < data->env.size)
    {
        g_exported_env[i] = ft_strdup(data->env.tab[i]);
		i++;
    }
    g_exported_env[data->env.size] = NULL;
    g_exported_env_size = data->env.size;
}

char **add_to_exported(char **exported, int *size, const char *new_var)
{
    char **new_exported;
    int i;

	i = 0;
    new_exported = malloc((*size + 2) * sizeof(char *));
    if (!new_exported)
        return (exported);

    while (i < *size)
    {
        new_exported[i] = exported[i];
		i++;
    }
    new_exported[i] = ft_strdup(new_var);
    if (!new_exported[i])
    {
        free(new_exported);
        return (exported);
    }
    new_exported[i + 1] = NULL;
    if (exported)
        free(exported);
    *size += 1;
    return (new_exported);
}

char **add_to_env(char **env, int *size, const char *new_var)
{
    char **new_env;
    int i;

	i = 0;
    new_env = malloc((*size + 2) * sizeof(char *));
    if (!new_env)
        return (env);
    while (i < *size)
    {
        new_env[i] = env[i];
		i++;
    }
    new_env[i] = ft_strdup(new_var);
    if (!new_env[i])
        return (free(new_env),env);
    new_env[i + 1] = NULL;
    if (env)
        free(env);
    *size += 1;
    return (new_env);
}

void update_env(char **env, int i, char *key, char *value)
{
    int	len;
	
	len = ft_strlen(key) + ft_strlen(value) + 2;
    free(env[i]);
    env[i] = malloc(len);
    if (!env[i])
        return ;
    ft_strcpy(env[i], key);
    ft_strcat(env[i], "=");
    ft_strcat(env[i], value);
}

void update_exported(char **exported, int i, char *key)
{
    free(exported[i]);
    exported[i] = ft_strdup(key);
}

int is_valid_identifier(char *key)
{
    int i;

	i = 0;
    if (!key || !key[0] || (key[0] >= '0' && key[0] <= '9'))
        return (0);

    while (key[i])
    {
        if (!(key[i] == '_' || ft_isalnum(key[i])))
            return (0);
        i++;
    }

    return (1);
}

void update_or_add_to_exported(char *key)
{
    int i;

	i = 0;
    while (i < g_exported_env_size)
    {
        if (ft_strncmp(g_exported_env[i], key, ft_strlen(key)) == 0 && (g_exported_env[i][ft_strlen(key)] == '\0' || g_exported_env[i][ft_strlen(key)] == '='))
        {
            update_exported(g_exported_env, i, key);
            return;
        }
		i++;
    }
    g_exported_env = add_to_exported(g_exported_env, &g_exported_env_size, key);
}

void update_or_add_to_env(char *key, char *value, t_data *data)
{
    int		i;
	int		len;
	char	*new_var;

	i = 0;
    len = ft_strlen(key) + ft_strlen(value) + 2;

    while (i < data->env.size)
    {
        if (ft_strncmp(data->env.tab[i], key, ft_strlen(key)) == 0 && data->env.tab[i][ft_strlen(key)] == '=')
        {
            update_env(data->env.tab, i, key, value);
            return;
        }
		i++;
    }
    *new_var = malloc(len * sizeof(char));
	if (!new_var)
		return ;
    if (new_var)
    {
        ft_strcpy(new_var, key);
        ft_strcat(new_var, "=");
        ft_strcat(new_var, value);
        data->env.tab = add_to_env(data->env.tab, &data->env.size, new_var);
        free(new_var);
    }
}

void process_export_arg(char *arg, t_data *data)
{
    char	*equals_pos;
    char	*value;
    char	*key;
	
	equals_pos = ft_strchr(arg, '=');
	key = NULL;
	value = NULL;
    if (equals_pos)
    {
        key = ft_substr(arg, 0, equals_pos - arg);
        value = ft_strdup(equals_pos + 1);
    }
    else
        key = ft_strdup(arg);
    if (!is_valid_identifier(key))
    {
        printf("export: '%s': not a valid identifier\\n", arg);
        free(key);
        if (value)
            free(value);
        return ;
    }
    update_or_add_to_exported(key);
    if (value)
    {
        update_or_add_to_env(key, value, data);
        free(value);
    }
    free(key);
}

int ft_export(char **args, t_data *data) // 
{
	static t_table export;
    int i;
	
	i = 1;
	if (!export.tab)
		init_exported_env(data);
    while (args[i])
    {
        process_export_arg(args[i], data);
        i++;
    }
    return 0;
}

int ft_export_print(void)
{
    int i = 0;
    while (g_exported_env[i])
    {
        printf("declare -x %s\n", g_exported_env[i]);
        i++;
    }
    return 0;
}