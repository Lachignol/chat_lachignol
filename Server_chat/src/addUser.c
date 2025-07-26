/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addUser.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 23:43:59 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 23:44:22 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

int	add_client_to_fd(t_fd *fd, int new_client, char *pseudo)
{
	int	i;
	int	send_bytes;

	i = -1;
	while (++i < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[i] == 0)
		{
			fd->connected_socket_fd[i] = new_client;
			strncpy(fd->pseudos[i], pseudo, PSEUDO_MAX_LENGTH);
			fd->pseudos[i][PSEUDO_MAX_LENGTH - 1] = '\0';
			send_bytes = send(fd->connected_socket_fd[i], WELCOME_MSG,
					strlen(WELCOME_MSG), 0);
			if (send_bytes < 0)
			{
				fprintf(stderr, "Erreur d'envoi du message de bienvenue\n");
				close(new_client);
				return (0);
			}
			broadcast_welcome_message(fd, i);
			return (1);
		}
	}
	return (close_and_notify_server_full(&new_client), 0);
}

void	add_new_contact(t_fd *fd, t_sock_adress *sock_ad)
{
	int			new_client;
	char		pseudo[PSEUDO_MAX_LENGTH];

	if (FD_ISSET(fd->srv_fd, &(fd->monitoring_fd)))
	{
		new_client = accept(fd->srv_fd, (struct sockaddr *)
				&(sock_ad->sock_address), &(sock_ad->size));
		if (!authenticate_user(&new_client, pseudo))
			return ;
		add_client_to_fd(fd, new_client, pseudo);
		return ;
	}
}
