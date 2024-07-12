/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:24:02 by jedusser          #+#    #+#             */
/*   Updated: 2024/07/12 12:49:13 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void init_exported_env(t_data *data, t_table *export)
{
    int i;

	i = 0;
    export->tab = malloc((data->env.size + 1) * sizeof(char *));
    if (!export->tab)
        return;
    while (i < data->env.size)
    {
        export->tab[i] = ft_strdup(data->env.tab[i]);
		i++;
    }
    while (i <= data->env.size)
	{
        export->tab[i] = NULL;
        i++;
    }
    export->size = data->env.size;
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

void update_env(t_data *data, int i, char *key, char *value)
{
    int	len;
	
	len = ft_strlen(key) + ft_strlen(value) + 2;
    free(data->env.tab[i]);
    data->env.tab[i] = ft_calloc(len, sizeof(char));
    if (!data->env.tab[i])
        return ;
    ft_strcpy(data->env.tab[i], key);
    ft_strcat(data->env.tab[i], "=");
    ft_strcat(data->env.tab[i], value);
}

void update_exported(char **exported, int i, char *new_var)
{
    free(exported[i]);
    exported[i] = ft_strdup(new_var);
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

void update_or_add_to_exported(char *key, t_table *export)
{
    int i;

	i = 0;
    while (i < export->size)
    {
        if (ft_strncmp(export->tab[i], key, ft_strlen(key)) == 0 && (export->tab[i][ft_strlen(key)] == '\0' || export->tab[i][ft_strlen(key)] == '='))
        {
            update_exported(export->tab, i, key);
            return;
        }
		i++;
    }
    export->tab = add_to_exported(export->tab, &export->size, key);
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
            update_env(data, i, key, value);
            return ;
        }
		i++;
    }
    new_var = ft_calloc(len,sizeof(char));
	if (!new_var)
		return ;
	ft_strcpy(new_var, key);
	printf("new_var before cat = %s\n", new_var);
	ft_strcat(new_var, "=");
	printf("new_var after 1st cat = %s\n", new_var);
	ft_strcat(new_var, value);
	printf("new_var after 2nd cat = %s\n", new_var);
	data->env.tab = add_to_env(data->env.tab, &data->env.size, new_var);
	free(new_var);
}

void	process_export_arg(int i, t_data *data, t_table *export)
{
    char	*equals_pos;
    char	*value;
    char	*key;
	
	equals_pos = ft_strchr(data->args.tab[i], '=');
	key = NULL;
	value = NULL;
    if (equals_pos)
    {
        key = ft_substr(data->args.tab[i], 0, equals_pos - data->args.tab[i]);
        value = ft_strdup(equals_pos + 1);
    }
    else
        key = ft_strdup(data->args.tab[i]);
    if (!is_valid_identifier(key))
    {
        printf("export: '%s': not a valid identifier\\n", data->args.tab[i]);
        free(key);
        if (value)
            free(value);
        return ;
    }
	// if !value : 
    update_or_add_to_exported(key, export);
	//else 
 
    if (value)
    {
	//   update_or_add_to_exported(new_var, export);
        update_or_add_to_env(key, value, data);
        free(value);
    }
    free(key);
}

int ft_export(t_data *data, t_table *export) // 
{
    int i;
	
	i = 1;
	
    while (data->args.tab[i])
    {
        process_export_arg(i, data, export);
        i++;
    }
    return 0;
}

int ft_export_print(t_table *export)
{
    int	i;
	
	i = 0;
    while (export->tab[i])
    {
        printf("declare -x %s\n", export->tab[i]);
        i++;
    }
    return 0;
}