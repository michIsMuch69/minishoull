/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:23:55 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/16 09:25:02 by jean-michel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

char **add_to_table(t_table *table, char *new_var)
{
    char **new_tab;
    int i;

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
void update_table(char **table, int i, char *new_var)
{
    free(table[i]);
    table[i] = ft_strdup(new_var);
}
char *create_quoted_var(char *key, char *value)
{
    int len;
    char    *new_var;

    len = ft_strlen(key) + ft_strlen(value) + 4; // +4 pour =,"", and '\0
    new_var = ft_calloc(len, sizeof(char)); 
    if (!new_var)
        return NULL;
    ft_strcpy(new_var, key);
    ft_strcat(new_var, "=\"");
    ft_strcat(new_var, value); 
    ft_strcat(new_var, "\"");
    return new_var;
}

char *create_unquoted_var(char *key, char *value)
{
    int len;
    char *new_var;
    len = ft_strlen(key) + ft_strlen(value) + 2;
    new_var = ft_calloc(len, sizeof(char));
    if (!new_var)
        return NULL;
    ft_strcpy(new_var, key);
    ft_strcat(new_var, "=");
    ft_strcat(new_var, value);
    return new_var;
}

char *create_var_without_equals(char *key)
{
    int len;
    char *new_var;

    len = ft_strlen(key) + 1;
    new_var = ft_calloc(len, sizeof(char)); 
    if (!new_var)
        return NULL;
    ft_strcpy(new_var, key);
    return new_var;
}
