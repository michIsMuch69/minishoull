/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jedusser <jedusser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 09:14:53 by jedusser          #+#    #+#             */
/*   Updated: 2024/06/13 10:38:38 by jedusser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	arrow_count(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i++] == c)
		;
	return (i);
}

char	*skip_redir_symbol(char *token_file, bool direction)
{
	char	*file;
	int		i;
	int		tok_nb;
	int		size;

	if (direction == 1)
		tok_nb = arrow_count(token_file, '>');
	else
		tok_nb = arrow_count(token_file, '<');
	if (tok_nb > 3)
		return (NULL);
	size = (ft_strlen(token_file) - (tok_nb -1)) + 1;
	file = ft_calloc(size, sizeof(char));
	if (!file)
		return (NULL);
	file = ft_strcpy(file, &token_file[tok_nb - 1]);
	//ft_printf("file == %s\n", file);
	return (file);
}
