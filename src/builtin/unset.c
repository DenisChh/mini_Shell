/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xuwang <xuwang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 14:53:21 by xuwang            #+#    #+#             */
/*   Updated: 2021/11/02 17:20:08 by xuwang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_is_cmd(char *s)
{
	int	i;

	i = 0;
	if (!ft_isalpha(s[i]) && s[i] != '_')
		return (0);
	i++;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

t_list	*check_exist(char *cmd, t_list *env_list)
{
	int	len;

	len = ft_strlen(cmd);
	while (env_list)
	{
		if (ft_strncmp(cmd, (char *)env_list->content, len) == 0
			&& (((char *)env_list->content)[len] == '\0'
			|| ((char *)env_list->content)[len] == '='))
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

static void	del_part(t_list **env_list, t_list *to_del)
{
	t_list	*ite;

	ite = *env_list;
	if (ite == to_del)
	{
		*env_list = (*env_list)->next;
		ft_lstdelone(to_del, free);
	}
	else
	{
		while (ite->next != to_del)
			ite = ite->next;
		ite->next = ite->next->next;
		ft_lstdelone(to_del, free);
	}
}

static t_list	*del_env(char *cmd, t_list *env_list)
{
	t_list	*to_del;

	to_del = check_exist(cmd, env_list);
	if (!to_del)
		return (env_list);
	del_part(&env_list, to_del);
	return (env_list);
}

int	ft_unset(t_cmd *cmd, t_list **env_list)
{
	int	i;
	int	ret;

	i = 1;
	ret = SUCCESS;
	if (!cmd)
		return (ERROR);
	while (cmd && cmd->cmd[i])
	{
		if (!(check_is_cmd(cmd->cmd[i])))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(cmd->cmd[i], STDERR_FILENO);
			ft_putendl_fd("\': not a valid identifier", STDERR_FILENO);
			ret = ERROR;
		}
		else
			*env_list = del_env(cmd->cmd[i], *env_list);
		i++;
	}
	return (ret);
}
