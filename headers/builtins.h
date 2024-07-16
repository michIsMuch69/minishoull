/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jean-micheldusserre <jean-micheldusserr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:39:55 by jean-michel       #+#    #+#             */
/*   Updated: 2024/07/16 12:40:43 by jean-michel      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include "struct.h"

/*===========================builtins.c===============================*/

void    ft_exit(t_data *data, int tab_size, int last_status);
int     ft_cd(char **args, char **env);
int     ft_pwd(void);
int     ft_echo(char **args);
int     ft_env(char **env);
int     ft_unset(char *var, t_table *env);
void    set_env(char *key, char *value, char **env);

/*===========================export===============================*/

int     ft_export_print(t_table *export);
int     init_exported_env(t_data *data, t_table *export);
int     ft_export(t_data *data, t_table *export);
void    update_table(char **table, int i, char *new_var);
char    **add_to_table(t_table *table, char *new_var);
char    *create_quoted_var(char *key, char *value);
char    *create_unquoted_var(char *key, char *value);
char    *create_var_without_equals(char *key);
int     process_full_entry(t_vars *vars, t_data *data, t_table *export, int i);
int     process_unquoted_key(t_vars *vars, t_data *data, t_table *export, int i);

/*===========================builtins_utils.c===============================*/

int     is_builtin_parent(t_data *data);
int     is_builtin_child(t_data *data);
void    sort_tab(t_table *table);
int     process_uncomplete_entry(t_vars *vars, t_data *data, t_table *export, int i);
int     exec_builtin_parent(t_data *data, int tab_size, int i, int **fd);
int     exec_builtin_child(t_data *data, int tab_size, int i, int **fd);
int     is_numeric_str(char *str);