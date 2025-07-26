/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 23:33:45 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 21:19:38 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

// on peu ajouter d'autre filtre pour securiser 
// les input en reflexion pour le moment
int	sanitize_and_check_exit(char *buffer, int bytes, t_fd *fd, int idx)
{
	int	i;

	i = 0;
	if (bytes >= BUFFER_SIZE)
		bytes = BUFFER_SIZE - 1;
	buffer[bytes] = '\0';
	while (i < bytes)
	{
		if (!isprint(buffer[i]) && buffer[i] != '\n'
			&& buffer[i] != '\r')
			buffer[i] = '?';
		i++;
	}
	if (strcmp(buffer, "/quit") == 0)
	{
		handle_client_disconnect(fd, idx);
		return (0);
	}
	return (1);
}

int	authenticate_user(int *new_client, char pseudo[PSEUDO_MAX_LENGTH])
{
	ssize_t			received;

	if (*new_client < 0)
	{
		perror("accept");
		return (0);
	}
	received = recv(*new_client, pseudo, PSEUDO_MAX_LENGTH - 1, 0);
	if (received <= 0)
	{
		fprintf(stderr, "Erreur lors de la réception du pseudo\n");
		close(*new_client);
		return (0);
	}
	pseudo[received] = '\0';
	return (1);
}
