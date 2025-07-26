/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:45:30 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 23:21:49 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

void	broadcast_message(t_fd *fd, const char *msg, int exclude_idx)
{
	int		j;
	ssize_t	sent_bytes;

	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0 && fd->connected_socket_fd[j]
			!= fd->srv_fd && j != exclude_idx)
		{
			sent_bytes = send(fd->connected_socket_fd[j], msg, strlen(msg), 0);
			if (sent_bytes < 0)
			{
				fprintf(stderr, "Erreur lors de l'envoi\n");
				handle_client_disconnect(fd, j);
			}
		}
	}
}

void	broadcast_welcome_message(t_fd *fd, int exclude_idx)
{
	int		j;
	ssize_t	sent_bytes;
	char	msg[BUFFER_SIZE];

	printf("%s nous a rejoint \n", fd->pseudos[exclude_idx]);
	snprintf(msg, BUFFER_SIZE, "*** %s a rejoint le chat ***",
		fd->pseudos[exclude_idx]);
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0 && fd->connected_socket_fd[j]
			!= fd->srv_fd && j != exclude_idx)
		{
			sent_bytes = send(fd->connected_socket_fd[j], msg, strlen(msg), 0);
			if (sent_bytes < 0)
			{
				fprintf(stderr, "Erreur lors de l'envoi\n");
				handle_client_disconnect(fd, j);
			}
		}
	}
}

void	broadcast_goodbye_message(t_fd *fd, int exclude_idx)
{
	int		j;
	ssize_t	sent_bytes;
	char	msg[BUFFER_SIZE];

	printf("%s déconnecté \n", fd->pseudos[exclude_idx]);
	snprintf(msg, BUFFER_SIZE, "*** %s a quitté le chat ***",
		fd->pseudos[exclude_idx]);
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0 && fd->connected_socket_fd[j]
			!= fd->srv_fd && j != exclude_idx)
		{
			sent_bytes = send(fd->connected_socket_fd[j], msg, strlen(msg), 0);
			if (sent_bytes < 0)
			{
				fprintf(stderr, "Erreur lors de l'envoi\n");
				handle_client_disconnect(fd, j);
			}
		}
	}
}

void	close_and_notify_server_full(int *new_client)
{
	send(*new_client, "Full: serveur saturé\n", 21, 0);
	close(*new_client);
	printf("Max Connexion reached\n");
}
