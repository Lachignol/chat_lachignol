/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 23:33:45 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/18 03:08:49 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

void	add_new_contact(t_fd *fd, t_sock_adress *sock_ad)
{
	int		new_client;
	int		send_bytes;
	int		i;

	if (FD_ISSET(fd->srv_fd, &(fd->monitoring_fd)))
	{
		i = -1;
		new_client = accept(fd->srv_fd, (struct sockaddr *)
				&(sock_ad->sock_address), &(sock_ad->size));
		while (++i < CONNEXION_LIMIT)
		{
			if (fd->connected_socket_fd[i] == 0)
			{
				fd->connected_socket_fd[i] = new_client;
				send_bytes = send(fd->connected_socket_fd[i], WELCOME_MSG,
						strlen(WELCOME_MSG), 0);
				if (send_bytes < 0)
					return (fprintf(stderr, "Erreur d' envoi\n"), exit(1));
				printf("New chatter\n");
				return ;
			}
		}
		return (send(new_client, "Full: serveur saturé\n", 21, 0),
			close(new_client), printf("Max Connexion reached\n"), (void) NULL);
	}
}

void	handle_client_disconnect(t_fd *fd, int idx)
{
	fprintf(stderr, "client deconnecter\n");
	close(fd->connected_socket_fd[idx]);
	fd->connected_socket_fd[idx] = 0;
}

void	handle_client_data(t_fd *fd, char *buffer, int idx, int bytes)
{
	int	send_bytes;
	int	j;

	buffer[bytes] = '\0';
	printf("Client: %s\n", buffer);
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0
			&& fd->connected_socket_fd[j] != fd->srv_fd
			&& fd->connected_socket_fd[j] != fd->connected_socket_fd[idx])
		{
			send_bytes = send(fd->connected_socket_fd[j], buffer,
					strlen(buffer), 0);
			if (send_bytes < 0)
			{
				fprintf(stderr, "Erreur lors de l'envoi\n");
				exit(1);
			}
		}
	}
	memset(buffer, '\0', BUFFER_SIZE);
}

void	read_and_send(t_fd *fd, char *buffer)
{
	int	received_bytes;
	int	i;

	i = -1;
	while (++i < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[i] > 0
			&& FD_ISSET(fd->connected_socket_fd[i], &(fd->monitoring_fd)))
		{
			received_bytes = read(fd->connected_socket_fd[i],
					buffer, BUFFER_SIZE);
			if (received_bytes <= 0)
				handle_client_disconnect(fd, i);
			else
			{
				handle_client_data(fd, buffer, i, received_bytes);
			}
		}
	}
}

void	monitor(t_fd *fd, t_sock_adress *socket_adrress, char *buffer)
{
	add_new_contact(fd, socket_adrress);
	read_and_send(fd, buffer);
}
