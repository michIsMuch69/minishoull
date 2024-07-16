/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:26:24 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/16 10:11:25 by jean-michel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void process_env_var_for_export(t_vars *vars, char *env_var, char **export_tab, int index)
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

int init_exported_env(t_data *data, t_table *export)
{
    int i;
    t_vars vars;
    
    i = 0;
    export->tab = malloc((data->env.size + 1) * sizeof(char *));
    if (!export->tab)
        return (-1);
    while (i < data->env.size)
    {
        process_env_var_for_export(&vars, data->env.tab[i], export->tab, i);
        i++;
    }
    while (i <= data->env.size)
        export->tab[i++] = NULL;
    export->size = data->env.size;
    return (0);
}

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

int ft_export(t_data *data, t_table *export)
{
    int i;
    
    i = 1;
    while (data->args.tab[i])
    {
        process_export_arg(i, data, export);
        i++;
    }
    return (0);
}

int ft_export_print(t_table *export)
{
    int i;
    
    i = 0;
    sort_tab(export);
    while (export->tab[i])
    {
        printf("declare -x %s\n", export->tab[i]);
        i++;
    }
    return (0);
}