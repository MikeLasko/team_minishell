/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdomange <romitdomange@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 18:04:23 by rdomange          #+#    #+#             */
/*   Updated: 2024/09/20 18:04:25 by rdomange         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	finaly_done_with_norm(t_mshl *o)
{
	o->i = 0;
	o->j = 0;
	while (o->str && o->str[o->i])
	{
		while (o->str[o->i] == ' ' || o->str[o->i] == o->d)
			o->i++;
		o->cap = DEFAULT_CAP;
		if (o->str[o->i])
			o->pipe[o->k] = (char *)ft_malloc(o->cap * sizeof(char));
		o->j = 0;
		while (o->str[o->i] && (o->str[o->i] != o->d || o->quote))
		{
			if ((o->d == '|' && quoted(o->str[o->i], &(o->quote)) + 1)
				|| (o->d == ' ' && !quoted(o->str[o->i], &(o->quote))))
				o->pipe[o->k][o->j++] = o->str[o->i];
			o->pipe[o->k][o->j] = 0;
			if (o->j + 1 == (int)o->cap)
				o->pipe[o->k] = ft_realloc
					(o->pipe[o->k], o->j, o->cap * 2, &(o->cap));
			o->i++;
		}
		o->pipe[++o->k] = NULL;
		if (o->k + 1 == (int)o->cap2)
			o->pipe = ft_split_realloc(o->pipe, o->cap2 * 2, &(o->cap2));
	}
}

int	check_for_semicolon(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ';')
		{
			printf("minishell: syntax error near unexpected token ';'\n");
			return (0);
		}
		i++;
	}
	return (1);
}

char	**ft_split_quotes(char *str, char d)
{
	t_mshl	o;

	if (!check_for_semicolon(str))
		return (NULL);
	o.str = str;
	o.d = d;
	o.k = 0;
	o.cap2 = DEFAULT_CAP;
	o.quote = 0;
	o.pipe = (char **)ft_malloc(o.cap2 * sizeof(char *));
	finaly_done_with_norm(&o);
	if (o.quote && o.d == '|')
	{
		ft_printf("minishell: unclosed quote haha\n");
		ft_split_free(o.pipe);
		return (NULL);
	}
	return (o.pipe);
}

char	***ft_split_triple(char *line)
{
	char	***triple;
	char	**pipe;
	int		i;

	pipe = ft_split_quotes(line, '|');
	if (!pipe)
		return (NULL);
	triple = (char ***)malloc((ft_split_len(pipe) + 1) * sizeof(char **));
	i = 0;
	while (pipe[i])
	{
		triple[i] = ft_split_quotes(pipe[i], ' ');
		if (!triple[i])
		{
			ft_free_triple(triple);
			ft_split_free(pipe);
			return (NULL);
		}
		i++;
	}
	triple[i] = NULL;
	ft_split_free(pipe);
	return (triple);
}

void	ft_free_triple(char ***triple)
{
	int	i;
	int	j;

	i = 0;
	while (triple && triple[i])
	{
		j = 0;
		while (triple[i][j])
		{
			free(triple[i][j]);
			j++;
		}
		free(triple[i]);
		i++;
	}
	free(triple);
}
