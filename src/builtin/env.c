/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmourtab <zakariamourtaban@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:12:30 by odib              #+#    #+#             */
/*   Updated: 2024/09/01 18:35:36 by zmourtab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int env_command(t_env *env_list)
 {
    t_env *temp;

    if (env_list == NULL) {
        fprintf(stderr, "Error: Environment list is NULL\n");
        return 1; 
    }

    temp = env_list;
    while (temp != NULL) {
        if (temp->hidden == 0) {
            printf("%s=\"%s\"\n", temp->key, temp->value);
        }
        temp = temp->next;
    }

    return 0;
}