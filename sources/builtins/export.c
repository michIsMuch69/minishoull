/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 12:47:42 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/13 12:48:03 by jean-michel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// General function to add a new variable to a table
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

// General function to update a variable in a table
void update_table(char **table, int index, char *new_var)
{
    free(table[index]);
    table[index] = ft_strdup(new_var);
}

// Initializes the exported environment table from the existing environment table
void init_exported_env(t_data *data, t_table *export)
{
    int i;
    char *key;
    char *value;
    char *new_var;
    char *equals_pos;
    int len;

    i = 0;
    export->tab = malloc((data->env.size + 1) * sizeof(char *));
    if (!export->tab)
        return;

    while (i < data->env.size)
    {
        equals_pos = ft_strchr(data->env.tab[i], '=');
        if (equals_pos)
        {
            key = ft_substr(data->env.tab[i], 0, equals_pos - data->env.tab[i]);
            value = ft_strdup(equals_pos + 1);
            len = ft_strlen(key) + ft_strlen(value) + 3;
            new_var = ft_calloc(len, sizeof(char));
            if (new_var)
            {
                ft_strcpy(new_var, key);
                ft_strcat(new_var, "=\"");
                ft_strcat(new_var, value);
                ft_strcat(new_var, "\"");
                export->tab[i] = new_var;
            }
            free(key);
            free(value);
        }
        else
        {
            export->tab[i] = ft_strdup(data->env.tab[i]);
        }
        i++;
    }
    while (i <= data->env.size)
    {
        export->tab[i] = NULL;
        i++;
    }
    export->size = data->env.size;
}

// Checks if a key is a valid identifier
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

// General function to update or add a variable in a table
void update_or_add_to_table(char *new_var, t_table *table)
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

// Processes an argument for the export command
void process_export_arg(int i, t_data *data, t_table *export)
{
    char *equals_pos;
    char *value;
    char *key;
    char *new_var;
    int len;

    equals_pos = ft_strchr(data->args.tab[i], '=');
    key = NULL;
    value = NULL;
    new_var = NULL;
    if (equals_pos)
    {
        key = ft_substr(data->args.tab[i], 0, equals_pos - data->args.tab[i]);
        value = ft_strdup(equals_pos + 1);
        len = ft_strlen(key) + ft_strlen(value) + 3;
        new_var = ft_calloc(len, sizeof(char));
        if (!new_var)
            return;
        ft_strcpy(new_var, key);
        ft_strcat(new_var, "=\"");
        ft_strcat(new_var, value);
        ft_strcat(new_var, "\"");
    }
    else
    {
        key = ft_strdup(data->args.tab[i]);
        len = ft_strlen(key) + 3;
        new_var = ft_calloc(len, sizeof(char));
        if (!new_var)
            return;
        ft_strcpy(new_var, key);
        ft_strcat(new_var, "=\"\"");
    }

    if (!is_valid_identifier(key))
    {
        printf("export: '%s': not a valid identifier\n", data->args.tab[i]);
        free(key);
        if (value)
            free(value);
        return;
    }

    update_or_add_to_table(new_var, export);
    if (value)
    {
        update_or_add_to_table(new_var, &data->env);
        free(value);
    }

    free(new_var);
    free(key);
}

// Export command implementation
int ft_export(t_data *data, t_table *export)
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

// Print the export table
int ft_export_print(t_table *export)
{
    int i;

    i = 0;
    while (export->tab[i])
    {
        printf("declare -x %s\n", export->tab[i]);
        i++;
    }
    return 0;
}
