/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 09:26:24 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/17 17:32:08 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int process_env_var_for_export(t_vars *vars, char *env_var, char **export_tab, int index)
{
    vars->equal_pos = ft_strchr(env_var, '=');

    if (vars->equal_pos)
    {
        vars->key = ft_substr(env_var, 0, vars->equal_pos - env_var);
        if (!vars->key)
            return (1);
        vars->value = ft_strdup(vars->equal_pos + 1);
        if (!vars->value)
            return (free(vars->key), 1);
        vars->new_var = create_quoted_var(vars->key, vars->value);
        if (!vars->new_var)
            return (free_vars(vars), 1);
        free(vars->key);
        free(vars->value);
    }
    else
    {
        vars->new_var = ft_strdup(env_var);
        if (!vars->new_var)
            return (1);
    }
    export_tab[index] = vars->new_var;
    return (0);
}

int init_exported_env(t_data *data, t_table *export)
{
    int i;
    t_vars vars;

    export->tab = malloc((data->env.size + 1) * sizeof(char *));
    if (!export->tab)
        return (1);
    i = 0;
    while (i < data->env.size)
    {
        if (process_env_var_for_export(&vars, data->env.tab[i], export->tab, i) != 0)
            return (free(export->tab), free_vars(&vars), 1);
        i++;
    }
    export->tab[i] = NULL;
    export->size = data->env.size;
    return (0);
}

int process_export_arg(int i, t_data *data, t_table *export)
{
    t_vars vars;

    vars.equal_pos = ft_strchr(data->args.tab[i], '=');
    vars.key = NULL;
    vars.value = NULL;
    vars.new_var = NULL;

    if (vars.equal_pos)
    {
        if (process_full_entry(&vars, data, export, i) != 0)
            return (free_vars(&vars), 1);
    }
    else
    {
        if (process_uncomplete_entry(&vars, data, export, i) != 0)
            return (free_vars(&vars), 1);
    }

    return (0);
}

int ft_export(t_data *data, t_table *export)
{
    int i;
    
    i = 1;
    while (data->args.tab[i])
    {
        if (process_export_arg(i, data, export) != 0)
            return (1);
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
        ft_printf("declare -x %s\n", export->tab[i]);
        i++;
    }
    return (0);
}
