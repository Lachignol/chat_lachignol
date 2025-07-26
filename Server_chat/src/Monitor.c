/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 23:33:45 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 23:46:22 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

void	handle_client_disconnect(t_fd *fd, int idx)
{
	if (fd->connected_socket_fd[idx] != 0)
	{
		close(fd->connected_socket_fd[idx]);
		broadcast_goodbye_message(fd, idx);
		fd->connected_socket_fd[idx] = 0;
		memset(fd->pseudos[idx], 0, PSEUDO_MAX_LENGTH);
	}
}

void	handle_client_data(t_fd *fd, char *buffer, int idx, int bytes)
{
	int		send_bytes;
	int		j;
	char	message[BUFFER_SIZE + PSEUDO_MAX_LENGTH + 4];

	if (!sanitize_and_check_exit(buffer, bytes, fd, idx))
		return ;
	snprintf(message, sizeof(message), "%s: %s", fd->pseudos[idx], buffer);
	printf("%s\n", message);
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0
			&& fd->connected_socket_fd[j] != fd->srv_fd
			&& fd->connected_socket_fd[j] != fd->connected_socket_fd[idx])
		{
			send_bytes = send(fd->connected_socket_fd[j], message,
					strlen(message), 0);
			if (send_bytes < 0)
			{
				fprintf(stderr, "Erreur lors de l'envoi\n");
				handle_client_disconnect(fd, j);
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
