/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:47:42 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/15 12:09:27 by jean-michel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

typedef struct s_vars
{
    char *key;
    char *value;
    char *new_var;
    char *equal_pos;
} t_vars;

// add variable to a table
char    **add_to_table(t_table *table, const char *new_var)
{
    char    **new_tab;
    int     i;

    i = 0;
    new_tab = malloc((table->size + 2) * sizeof(char *));
    if (!new_tab)
        return (table->tab);

    while (i < table->size)
    {
        new_tab[i] = table->tab[i];
        i++;
    }
    new_tab[i] = ft_strdup(new_var);
    if (!new_tab[i])
    {
        free(new_tab);
        return (table->tab);
    }
    new_tab[i + 1] = NULL;
    if (table->tab)
        free(table->tab);
    table->size += 1;
    return (new_tab);
}

// update a variable in a table
void update_table(char **table, int index, char *new_var)
{
    free(table[index]);
    table[index] = ft_strdup(new_var);
}

// create a new_var with quotes
char *create_quoted_var(char *key, char *value)
{
    int len = ft_strlen(key) + ft_strlen(value) + 3;
    char *new_var = ft_calloc(len, sizeof(char));
    if (!new_var)
        return NULL;
    ft_strcpy(new_var, key);
    ft_strcat(new_var, "=\"");
    ft_strcat(new_var, value);
    ft_strcat(new_var, "\"");
    return new_var;
}

// create a new_var without quotes
char *create_unquoted_var(char *key, char *value)
{
    int len = ft_strlen(key) + ft_strlen(value) + 2;
    char *new_var = ft_calloc(len, sizeof(char));
    if (!new_var)
        return NULL;
    ft_strcpy(new_var, key);
    ft_strcat(new_var, "=");
    ft_strcat(new_var, value);
    return new_var;
}

// checks if a key is a valid identifier
int is_valid_identifier(char *key)
{
    int i = 0;
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

// Helper function to process each variable in the environment table for export
void init_env_var_for_export(t_vars *vars, char *env_var, char **export_tab, int index)
{
    vars->equal_pos = ft_strchr(env_var, '=');
    if (vars->equal_pos)
    {
        vars->key = ft_substr(env_var, 0, vars->equal_pos - env_var);
        vars->value = ft_strdup(vars->equal_pos + 1);
        vars->new_var = create_quoted_var(vars->key, vars->value);
        free(vars->key);
        free(vars->value);
    }
    else
        vars->new_var = ft_strdup(env_var);
    if (vars->new_var)
        export_tab[index] = vars->new_var;
}

// init the exported environment table from the existing environment table
void init_exported_env(t_data *data, t_table *export)
{
    int i = 0;
    t_vars vars;

    export->tab = malloc((data->env.size + 1) * sizeof(char *));
    if (!export->tab)
        return;
    while (i < data->env.size)
    {
        init_env_var_for_export(&vars, data->env.tab[i], export->tab, i);
        i++;
    }
    while (i <= data->env.size)
        export->tab[i++] = NULL;
    export->size = data->env.size;
}

// update or add a variable in a table (no quotes for env)
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

// update or add a variable in a table (with quotes for export)
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

// Helper function to process key-value pair in export argument
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
    free(vars->value);
}

// Helper function to process unquoted key in export argument
void process_unquoted_key(t_vars *vars, t_data *data, t_table *export, int i)
{
    vars->key = ft_strdup(data->args.tab[i]);
    if (!is_valid_identifier(vars->key))
    {
        printf("export: '%s': not a valid identifier\n", data->args.tab[i]);
        free(vars->key);
        return;
    }
    vars->new_var = ft_calloc(ft_strlen(vars->key) + 3, sizeof(char)); // +3 to account for ="" (equal sign and two quotes)
    ft_strcpy(vars->new_var, vars->key);
    ft_strcat(vars->new_var, "=\"\"");
    update_or_add_to_export(vars->new_var, export);
    free(vars->new_var);
    free(vars->key);
}

// process args for the export command
void process_export_arg(int i, t_data *data, t_table *export)
{
    t_vars vars;

    vars.equal_pos = ft_strchr(data->args.tab[i], '=');
    vars.key = NULL;
    vars.value = NULL;
    vars.new_var = NULL;

    if (vars.equal_pos)
        process_full_entry(&vars, data, export, i);
    else
        process_unquoted_key(&vars, data, export, i);
}

// export command implementation
int ft_export(t_data *data, t_table *export)
{
    int i = 1;
    while (data->args.tab[i])
    {
        process_export_arg(i, data, export);
        i++;
    }
    return 0;
}

// print the export table
int ft_export_print(t_table *export)
{
    int i = 0;
    while (export->tab[i])
    {
        printf("declare -x %s\n", export->tab[i]);
        i++;
    }
    return 0;
}
