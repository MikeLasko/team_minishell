/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdomange <romitdomange@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:59:41 by rdomange          #+#    #+#             */
/*   Updated: 2024/09/19 20:00:25 by rdomange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_prompt(char **env, char *exit_status)
{
	char	*ret;
	char	*pwd;

	pwd = ft_getcwd();
	if (ft_strchr(pwd, '/') == ft_strrchr(pwd, '/'))
		ret = ft_strjoin("minishell$ ", pwd);
	else if (!ft_strncmp(pwd, ft_env_get(env, "HOME"), ft_strlen(pwd) + 1))
		ret = ft_strdup("minishell$ ~");
	else
		ret = ft_strjoin("minishell$ ./", ft_strrchr(pwd, '/') + 1);
	free(pwd);
	if (!ft_strncmp(exit_status, "0", 2))
		pwd = ft_strjoin(ret, " " CLR_GRN ":3" CLR_RST " $ ");
	else
		pwd = ft_strjoin(ret, " " CLR_RED ":(" CLR_RST " $ ");
	free(ret);
	return (pwd);
}

void	letsgo(t_mshl *m)
{
	int	eroor_line_too_long;

	letsgo_getready(m);
	if (!m->triple)
	{
		m->exit_status = ft_itoa(1);
		return ;
	}
	if (ft_open_redirs(m))
		return ;
	m->comm = m->triple[0];
	ft_strtolower(m->comm[0]);
	if (!m->triple[1] && is_builtin_dontfork(m->comm[0]))
		m->exit_res = ft_exec_builtin(m);
	eroor_line_too_long = is_builtin_dontfork(m->triple[0][0]);
	while (!m->exit_res && m->triple[m->i] && !eroor_line_too_long)
	{
		ft_ready_pipe(m);
		ft_exec_pipesegment(m);
		ft_cleanup_pipe(m);
		m->i++;
	}
	letsgo_cleanup(m);
}

void	init_t_mshl(t_mshl *m, char **envp_main)
{
	init_signals();
	ft_printf("(✿ ◕‿ ◕) hi~~ welcome to minishell (っ＾▿＾)っ\n");
	m->redir_in = NULL;
	m->redir_out = NULL;
	m->env = ft_env_dup(envp_main);
	m->env = ft_export(m->env, "OLDPWD=");
	m->env_extra = ft_split("yea=YEAAA,extra=EXXXTRA", ",");
	m->line = ft_itoa(ft_atoi(ft_env_get(m->env, "SHLVL")) + 1);
	ft_env_set(m->env, "SHLVL", m->line);
	free(m->line);
	m->exit_status = ft_itoa(0);
	m->last_command = ft_strdup("^^\'");
}

int	main(int argc, char **argv, char **envp_main)
{
	t_mshl	m;

	if (argc == 1)
		init_t_mshl(&m, envp_main);
	while (argc == 1 && argv[0])
	{
		m.prompt = ft_get_prompt(m.env, m.exit_status);
		m.line = readline(m.prompt);
		free(m.prompt);
		if (ft_strlen(m.line))
		{
			add_history(m.line);
			letsgo(&m);
		}
		else
		{
			free(m.exit_status);
			m.exit_status = ft_itoa(0);
		}
		if (!m.line)
			return (0 + (0 * ft_printf("o(*￣○￣)ゝ ctrl+d, bye minishell\n")));
		free(m.line);
	}
	return (2 + (0 * ft_printf("usage: %s\n", "./minishell")));
}
