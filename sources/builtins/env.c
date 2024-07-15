#include "exec.h"

char **add_to_table(t_table *table, char *new_var);
void update_table(char **table, int i, char *new_var);
char *create_quoted_var(char *key, char *value);
char *create_unquoted_var(char *key, char *value);
char *create_var_without_equals(char *key);
int is_valid_identifier(char *key);
void update_or_add_to_env(char *new_var, t_table *table);
void update_or_add_to_export(char *new_var, t_table *table);
void process_full_entry(t_vars *vars, t_data *data, t_table *export, int i);
void process_unquoted_key(t_vars *vars, t_data *data, t_table *export, int i);
void process_env_var_for_export(t_vars *vars, char *env_var, char **export_tab, int index);
void init_exported_env(t_data *data, t_table *export);
void process_export_arg(int i, t_data *data, t_table *export);
int ft_export(t_data *data, t_table *export);
int ft_export_print(t_table *export);

int print_env(t_table *env)
{
    int i;
    
    i = 0;
    while (i < env->size)
    {
        if (env->tab[i])
            printf("%s\n", env->tab[i]);
        i++;
    }
    return 0;
}

int contains_equal_sign(char *str)
{
    while (*str)
    {
        if (*str == '=')
            return 1;
        str++;
    }
    return 0;
}

int ft_env(t_data *data)
{
    int i;
    
    i = 1;
    if (data->args.size == 1)
    {
        return print_env(&data->env);
    }

    while (i < data->args.size)
    {
        if (contains_equal_sign(data->args.tab[i]))
        {
            update_or_add_to_env(data->args.tab[i], &data->env);
        }
        else
        {
            printf("Executing command with modified environment: %s\n", data->args.tab[i]);
            break;
        }
        i++;
    }
    return 0;
}
