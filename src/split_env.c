/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odib <odib@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 23:43:08 by odib              #+#    #+#             */
/*   Updated: 2024/08/08 23:45:40 by odib             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char **allocate_result_array() {
    char **res = (char **)malloc(sizeof(char *) * 3);
    if (!res)
        return NULL;
    res[2] = NULL;
    return res;
}

static char *extract_key(char *str, char del) {
    int i = 0;
    while (str[i] && str[i] != del)
        i++;
    char *key = (char *)malloc(sizeof(char) * (i + 1));
    if (!key)
        return NULL;
    i = 0;
    while (str[i] && str[i] != del) {
        key[i] = str[i];
        i++;
    }
    key[i] = '\0';
    return key;
}

static char *extract_value(char *str, char del) {
    int i = 0;
    while (str[i] && str[i] != del)
        i++;
    if (str[i] == del)
        i++;
    int j = 0;
    while (str[i + j] != '\0')
        j++;
    char *value = (char *)malloc(sizeof(char) * (j + 1));
    if (!value)
        return NULL;
    j = 0;
    while (str[i]) {
        value[j] = str[i];
        i++;
        j++;
    }
    value[j] = '\0';
    return value;
}

char **ft_split_env(char *str, char del) {
    char **res = allocate_result_array();
    if (!res)
        return NULL;

    res[0] = extract_key(str, del);
    if (!res[0])
        return NULL;

    res[1] = extract_value(str, del);
    if (!res[1])
        return NULL;

    return res;
}