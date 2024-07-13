
#include  "exec.h"

void init_exported_env(t_data *data, t_table *export)
{
    int i;

    i = 0;
    export->tab = malloc((data->env.size + 1) * sizeof(char *));
    if (!export->tab)
        return;
    while (i < data->env.size)
    {
        export->tab[i] = ft_strdup(data->env.tab[i]);
        i++;
    }
    while (i <= data->env.size)
    {
        export->tab[i] = NULL;
        i++;
    }
    export->size = data->env.size;
}

char **add_to_exported(t_table *export, const char *new_var)
{
    char **new_exported;
    int i;

    i = 0;
    new_exported = malloc((export->size + 2) * sizeof(char *));
    if (!new_exported)
        return (export->tab);

    while (i < export->size)
    {
        printf("new var for exported : %s\n", new_var);
        new_exported[i] = export->tab[i];
        i++;
    }
    new_exported[i] = ft_strdup(new_var);
    if (!new_exported[i])
    {
        free(new_exported);
        return (export->tab);
    }
    new_exported[i + 1] = NULL;
    if (export->tab)
        free(export->tab);
    export->size++;
    return (new_exported);
}

char **add_to_env(t_table *env, const char *new_var)
{
    char **new_env;
    int i;

    i = 0;
    new_env = malloc((env->size + 2) * sizeof(char *));
    if (!new_env)
        return (env->tab);
    while (i < env->size)
    {
        new_env[i] = env->tab[i];
        i++;
    }
    new_env[i] = ft_strdup(new_var);
    if (!new_env[i])
        return (free(new_env), env->tab);
    new_env[i + 1] = NULL;
    if (env->tab)
        free(env->tab);
    env->size += 1;
    return (new_env);
}

void update_env(t_data *data, int i, char *key, char *value)
{
    int len;

    len = ft_strlen(key) + ft_strlen(value) + 2;
    free(data->env.tab[i]);
    data->env.tab[i] = ft_calloc(len, sizeof(char));
    if (!data->env.tab[i])
        return;
    ft_strcpy(data->env.tab[i], key);
    ft_strcat(data->env.tab[i], "=");
    ft_strcat(data->env.tab[i], value);
}

void update_exported(char **exported, int i, char *new_var)
{
    free(exported[i]);
    exported[i] = ft_strdup(new_var);
}

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

void update_or_add_to_exported(char *new_var, t_table *export)
{
    int i;
    char *key;
    int key_len;

    key_len = 0;
    while (new_var[key_len] && new_var[key_len] != '=')
        key_len++;
    key = ft_substr(new_var, 0, key_len);
    i = 0;
    while (i < export->size)
    {
        if (ft_strncmp(export->tab[i], key, key_len) == 0 && (export->tab[i][key_len] == '\0' || export->tab[i][key_len] == '='))
        {
            update_exported(export->tab, i, new_var);
            free(key);
            return;
        }
        i++;
    }
    export->tab = add_to_exported(export, new_var);
    free(key);
}

void update_or_add_to_env(char *key, char *value, t_data *data)
{
    int i;
    int len;
    char *new_var;

    i = 0;
    len = ft_strlen(key) + ft_strlen(value) + 2;

    while (i < data->env.size)
    {
        if (ft_strncmp(data->env.tab[i], key, ft_strlen(key)) == 0 && data->env.tab[i][ft_strlen(key)] == '=')
        {
            //wset_env(key, value, data->env.tab);
            update_env(data, i, key, value);
            return;
        }
        i++;
    }
    new_var = ft_calloc(len, sizeof(char));
    if (!new_var)
        return;
    ft_strcpy(new_var, key);
    printf("new_var before cat = %s\n", new_var);
    ft_strcat(new_var, "=");
    printf("new_var after 1st cat = %s\n", new_var);
    ft_strcat(new_var, value);
    printf("new_var after 2nd cat = %s\n", new_var);
    data->env.tab = add_to_env(&data->env, new_var);
    free(new_var);
}

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
        len = ft_strlen(key) + ft_strlen(value) + 2;
        new_var = ft_calloc(len, sizeof(char));
        if (!new_var)
            return;
        ft_strcpy(new_var, key);
        ft_strcat(new_var, "=");
        ft_strcat(new_var, value);
    }
    else
        key = ft_strdup(data->args.tab[i]);

    if (!is_valid_identifier(key))
    {
        printf("export: '%s': not a valid identifier\n", data->args.tab[i]);
        free(key);
        if (value)
            free(value);
        return;
    }
    if (new_var)
    {
        update_or_add_to_exported(new_var, export);
        update_or_add_to_env(key, value, data);
        free(new_var);
    }
    else
        update_or_add_to_exported(key, export);
    free(key);
    if (value)
        free(value);
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
    return 0;
}

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