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
    int len = ft_strlen(key) + ft_strlen(value) + 2;
    char *new_var = ft_calloc(len, sizeof(char));
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

void init_exported_env(t_data *data, t_table *export)
{
    int i;
    t_vars vars;
    
    i = 0;
    export->tab = malloc((data->env.size + 1) * sizeof(char *));
    if (!export->tab)
        return;
    while (i < data->env.size)
    {
        process_env_var_for_export(&vars, data->env.tab[i], export->tab, i);
        i++;
    }
    while (i <= data->env.size)
        export->tab[i++] = NULL;
    export->size = data->env.size;
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
    int i = 1;
    while (data->args.tab[i])
    {
        process_export_arg(i, data, export);
        i++;
    }
    return 0;
}

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
