/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fberthou <fberthou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 10:19:42 by fberthou          #+#    #+#             */
/*   Updated: 2024/02/13 22:57:36 by fberthou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdr/libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	count;

	count = ft_strlen(s);
	if (fd)
		write(fd, s, count);
}
