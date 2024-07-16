/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:25:24 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/16 09:26:10 by jean-michel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int is_valid_identifier(char *key)
{
    int i;
    
    i = 0;
    if (!key || !key[0] || (key[0] >= '0' && key[0] <= '9'))
        return 0;
    while (key[i])
    {
        if (!(key[i] == '_' || ft_isalnum(key[i])))
            return 0;
        i++;
    }
    return 1;
}

void update_or_add_to_env(char *new_var, t_table *table)
{
    int i;
    char *key;
    int key_len;

    key_len = 0;
    while (new_var[key_len] && new_var[key_len] != '=')
        key_len++;
    key = ft_substr(new_var, 0, key_len);
    i = 0;
    while (i < table->size)
    {
        if (ft_strncmp(table->tab[i], key, key_len) == 0 && (table->tab[i][key_len] == '\0' || table->tab[i][key_len] == '='))
        {
            update_table(table->tab, i, new_var);
            free(key);
            return;
        }
        i++;
    }
    table->tab = add_to_table(table, new_var);
    free(key);
}

void update_or_add_to_export(char *new_var, t_table *table)
{
    int i;
    char *key;
    int key_len;

    key_len = 0;
    while (new_var[key_len] && new_var[key_len] != '=')
        key_len++;
    key = ft_substr(new_var, 0, key_len);

    i = 0;
    while (i < table->size)
    {
        if (ft_strncmp(table->tab[i], key, key_len) == 0 && (table->tab[i][key_len] == '\0' || table->tab[i][key_len] == '='))
        {
            update_table(table->tab, i, new_var);
            free(key);
            return;
        }
        i++;
    }
    table->tab = add_to_table(table, new_var);
    free(key);
}

void process_full_entry(t_vars *vars, t_data *data, t_table *export, int i)
{
    vars->key = ft_substr(data->args.tab[i], 0, vars->equal_pos - data->args.tab[i]);
    vars->value = ft_strdup(vars->equal_pos + 1);
    if (!is_valid_identifier(vars->key))
    {
        printf("export: '%s': not a valid identifier\n", data->args.tab[i]);
        free(vars->key);
        free(vars->value);
        return;
    }
    vars->new_var = create_quoted_var(vars->key, vars->value);
    update_or_add_to_export(vars->new_var, export);
    free(vars->new_var);
    vars->new_var = create_unquoted_var(vars->key, vars->value);
    update_or_add_to_env(vars->new_var, &data->env);
    free(vars->new_var);
    free(vars->key); 
    free(vars->value); 
}

void process_unquoted_key(t_vars *vars, t_data *data, t_table *export, int i)
{
    vars->key = ft_strdup(data->args.tab[i]);
    if (!is_valid_identifier(vars->key))
    {
        printf("export: '%s': not a valid identifier\n", data->args.tab[i]);
        free(vars->key);
        return;
    }
    vars->new_var = create_var_without_equals(vars->key);
    update_or_add_to_export(vars->new_var, export);
    free(vars->new_var);
    free(vars->key);
}