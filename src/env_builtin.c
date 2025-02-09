/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdomange <romitdomange@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 16:32:32 by rdomange          #+#    #+#             */
/*   Updated: 2025/02/06 16:38:16 by rdomange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		ft_printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

char	**ft_export(char **env, char *arg)
{
	char	**ret;
	int		len;

	len = ft_env_namelen(arg);
	if (!len || (arg[len] != 0 && arg[len] != '='))
	{
		ft_printf("minishell: export: \'%s\': not a valid identifier\n", arg);
		return (env);
	}
	if (ft_env_get(env, arg))
	{
		if (arg[len] == '=')
			ft_env_set(env, arg, &arg[len + 1]);
		return (env);
	}
	if (arg[len] != '=')
		return (env);
	ret = ft_env_dup(env);
	ft_split_free(env);
	if (arg[len] == '=')
		ret[ft_split_len(ret)] = ft_strdup(arg);
	return (ret);
}

char	**ft_unset(char **env, char *name)
{
	int	i;
	int	j;
	int	len;

	name = ft_env_name(name);
	if (!name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env && env[i])
	{
		if (!ft_strncmp(name, env[i], len) && env[i][len] == '=')
		{
			free(env[i]);
			j = i;
			while (env[j++ + 1])
				env[j - 1] = env[j];
			env[j - 1] = NULL;
		}
		i++;
	}
	free(name);
	return (env);
}
