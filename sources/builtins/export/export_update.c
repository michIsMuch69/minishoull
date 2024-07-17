/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:25:24 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/17 17:06:07 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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

int update_or_add_to_env(char *new_var, t_table *table)
{
    int     i;
    char    *key;
    int     key_len;

    key_len = 0;
    while (new_var[key_len] && new_var[key_len] != '=')
        key_len++;
    key = ft_substr(new_var, 0, key_len);
    if (!key)
        return (1); // Memory allocation failure
    i = 0;
    while (i < table->size)
    {
        if (ft_strncmp(table->tab[i], key, key_len) == 0 && (table->tab[i][key_len] == '\0' || table->tab[i][key_len] == '='))
        {
            update_table(table->tab, i, new_var);
            free(key);
            return (0);
        }
        i++;
    }
    table->tab = add_to_table(table, new_var);
    free(key);
    return (0);
}

int update_or_add_to_export(char *new_var, t_table *table)
{
    int     i;
    char    *key;
    int     key_len;

    key_len = 0;
    while (new_var[key_len] && new_var[key_len] != '=')
        key_len++;
    key = ft_substr(new_var, 0, key_len);
    if (!key)
        return (1); // Memory allocation failure
    i = 0;
    while (i < table->size)
    {
        if (ft_strncmp(table->tab[i], key, key_len) == 0 && (table->tab[i][key_len] == '\0' || table->tab[i][key_len] == '='))
        {
            update_table(table->tab, i, new_var);
            free(key);
            return (0);
        }
        i++;
    }
    table->tab = add_to_table(table, new_var);
    free(key);
    return (0);
}

int process_full_entry(t_vars *vars, t_data *data, t_table *export, int i)
{
    vars->key = ft_substr(data->args.tab[i], 0, vars->equal_pos - data->args.tab[i]);
    if (!vars->key)
        return (1); // Memory allocation failure
    vars->value = ft_strdup(vars->equal_pos + 1);
    if (!vars->value)
    {
        free(vars->key);
        return (1); // Memory allocation failure
    }
    if (!is_valid_identifier(vars->key))
    {
        ft_printf("export: '%s': not a valid identifier\n", data->args.tab[i]);
        free_vars(vars);
        return (1);
    }
    vars->new_var = create_quoted_var(vars->key, vars->value);
    if (!vars->new_var)
    {
        free_vars(vars);
        return (1); // Memory allocation failure
    }
    if (update_or_add_to_export(vars->new_var, export) != 0)
    {
        free(vars->new_var);
        free_vars(vars);
        return (1);
    }
    free(vars->new_var);
    vars->new_var = create_unquoted_var(vars->key, vars->value);
    if (!vars->new_var)
    {
        free_vars(vars);
        return (1); // Memory allocation failure
    }
    if (update_or_add_to_env(vars->new_var, &data->env) != 0)
    {
        free(vars->new_var);
        free_vars(vars);
        return (1);
    }
    free(vars->new_var);
    free(vars->key); 
    free(vars->value);
    return (0);
}

int process_uncomplete_entry(t_vars *vars, t_data *data, t_table *export, int i)
{
    vars->key = ft_strdup(data->args.tab[i]);
    if (!vars->key)
        return (1); // Memory allocation failure
    if (!is_valid_identifier(vars->key))
    {
        ft_printf("export: '%s': not a valid identifier\n", data->args.tab[i]);
        free(vars->key);
        return (1);
    }
    vars->new_var = create_var_without_equals(vars->key);
    if (!vars->new_var)
    {
        free(vars->key);
        return (1); // Memory allocation failure
    }
    if (update_or_add_to_export(vars->new_var, export) != 0)
    {
        free(vars->new_var);
        free(vars->key);
        return (1);
    }
    free(vars->new_var);
    free(vars->key);
    return (0);
}
