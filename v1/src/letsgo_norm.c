/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   letsgo_norm.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdomange <romitdomange@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:12:43 by rdomange          #+#    #+#             */
/*   Updated: 2025/02/04 17:12:49 by rdomange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	letsgo_getready(t_mshl *m)
{
	m->exit_res = 0;
	m->cash = m->line;
	m->line = cash_money(*m);
	if (ft_strnstr(m->cash, "!!", ft_strlen(m->cash)))
		ft_printf("!!:%s\n", m->line);
	free(m->last_command);
	m->last_command = m->cash;
	m->triple = ft_split_triple(m->line);
	if (!m->triple)
		return ;
	m->exit_res = get_redir_info(m);
	m->i = 0;
	m->pids[0] = 0;
}

void	letsgo_cleanup(t_mshl *m)
{
	if (m->fd_in != -1)
		close(m->fd_in);
	if (m->fd_out != -1)
		close(m->fd_out);
	swap_signal_for_execute();
	m->i = 0;
	while (m->triple[m->i])
	{
		if (m->pids[m->i] > 0)
		{
			waitpid(m->pids[m->i], &m->waitpid_status, 0);
			m->exit_res = ((m->waitpid_status & 0xff00) >> 8);
		}
		m->i++;
	}
	swap_signal_for_execute();
	ft_free_triple(m->triple);
	m->redir_in = ft_free(m->redir_in);
	m->redir_out = ft_free(m->redir_out);
	free(m->exit_status);
	m->exit_status = ft_itoa(m->exit_res);
}

void	ft_ready_pipe(t_mshl *m)
{
	m->comm = m->triple[m->i];
	ft_strtolower(m->comm[0]);
	m->is_first = (m->i == 0);
	m->is_last = (m->triple[m->i + 1] == NULL);
	if (!m->is_last && pipe(m->pipefd) < 0)
		exit(5 + (0 * ft_printf("pipe failed\n")));
	if (m->fd_in != -1 && m->is_first)
		m->prevfd = m->fd_in;
	if (m->fd_out != -1 && m->is_last)
		m->pipefd[1] = m->fd_out;
}

void	ft_cleanup_pipe(t_mshl *m)
{
	if (m->prevfd != STDIN_FILENO)
		close(m->prevfd);
	if (!m->is_last)
		close(m->pipefd[1]);
	if (m->is_last)
		m->prevfd = STDIN_FILENO;
	else
		m->prevfd = m->pipefd[0];
}
