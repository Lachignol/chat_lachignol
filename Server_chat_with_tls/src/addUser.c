/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addUser.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 23:43:59 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/28 12:11:38 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

int	add_client_to_fd(t_fd *fd, int new_client, char *pseudo, SSL *ssl)
{
	int	i;
	int	send_bytes;

	i = -1;
	while (++i < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[i] == 0)
		{
			fd->connected_socket_fd[i] = new_client;
			fd->client_ssl[i] = ssl;
			strncpy(fd->pseudos[i], pseudo, PSEUDO_MAX_LENGTH);
			fd->pseudos[i][PSEUDO_MAX_LENGTH - 1] = '\0';
			send_bytes = SSL_write(ssl, SECURITY_BANNER_COLORED, strlen(SECURITY_BANNER_COLORED));
			if (send_bytes < 0)
			{
				fprintf(stderr, "Erreur d'envoi du message de bienvenue\n");
				handle_client_disconnect(fd, i);
				return (0);
			}
			if (!send_connected_users_list(fd, ssl, i))
				return (0);
			broadcast_welcome_message(fd, i);
			return (1);
		}
	}
	return (close_and_notify_server_full(&new_client, ssl), 0);
}

void	add_new_contact(t_fd *fd, t_sock_adress *sock_ad, SSL_CTX *ctx)
{
	int			new_client;
	char		pseudo[PSEUDO_MAX_LENGTH];
	SSL			*ssl;

	if (FD_ISSET(fd->srv_fd, &(fd->monitoring_fd)))
	{
		new_client = accept(fd->srv_fd, (struct sockaddr *)
				&(sock_ad->sock_address), &(sock_ad->size));
		if (new_client < 0)
			return (perror("accept"), (void) NULL);
		ssl = SSL_new(ctx);
		if (!ssl)
		{
			fprintf(stderr, "Erreur SSL_new\n");
			return (close(new_client), (void) NULL);
		}
		SSL_set_fd(ssl, new_client);
		if (SSL_accept(ssl) <= 0)
		{
			fprintf(stderr, "Erreur SSL_accept (handshake)\n");
			return (SSL_free(ssl), ssl = NULL, close(new_client), (void) NULL);
		}
		if (!authenticate_user(&new_client, pseudo, ssl))
			return ;
		if (!add_client_to_fd(fd, new_client, pseudo, ssl))
			return ;
	}
}
