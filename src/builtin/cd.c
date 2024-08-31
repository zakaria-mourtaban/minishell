/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 11:18:31 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 11:23:18 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int cd_to_home(t_env *env_list)
{
    char *home = get_env(env_list, "HOME");
    char *pwd = get_env(env_list, "PWD");

    if (home == NULL || chdir(home) != 0)
    {
        perror("cd: error changing to home directory");
        return 1;
    }
    set_env(&env_list, "OLDPWD", pwd, 0);
    set_env(&env_list, "PWD", home, 0);
    return 0;
}

int cd_to_oldpwd(t_env *env_list)
{
    char *old_pwd = get_env(env_list, "OLDPWD");
    char *pwd = get_env(env_list, "PWD");

    if (old_pwd == NULL || chdir(old_pwd) != 0)
    {
        perror("cd: error changing to previous directory");
        return 1;
    }

    set_env(&env_list, "OLDPWD", pwd, 0);
    char *new_pwd = getcwd(NULL, 0);
    if (new_pwd == NULL)
    {
        perror("cd: error getting current directory");
        return 1;
    }
    set_env(&env_list, "PWD", new_pwd, 0);
    free(new_pwd);

    return 0;
}

int cd_to_path(char *path, t_env *env_list)
{
    char *pwd = get_env(env_list, "PWD");

    if (chdir(path) != 0)
    {
        perror("cd: error changing directory");
        return 1;
    }
    char *new_pwd = getcwd(NULL, 0);
    if (new_pwd == NULL)
    {
        perror("cd: error getting current directory");
        return 1;
    }
    set_env(&env_list, "OLDPWD", pwd, 0);
    set_env(&env_list, "PWD", new_pwd, 0);
    free(new_pwd);
    return 0;
}

int change_dir(t_arg *args, t_env *env_list)
{
    t_arg *current = args->next; // Assuming `args` contains the command itself, start from the first argument.

    if (current == NULL)
    {
        return cd_to_home(env_list);
    }

    if (current->arg[0] == '-' && current->arg[1] == '\0')
    {
        return cd_to_oldpwd(env_list);
    }

    if (current->next == NULL)
    {
        return cd_to_path(current->arg, env_list);
    }

    fprintf(stderr, "cd: too many arguments\n");
    return 1;
}