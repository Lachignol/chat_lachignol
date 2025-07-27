/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 20:45:30 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/28 00:00:03 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

void	broadcast_message(t_fd *fd, const char *msg, int exclude_idx)
{
	int		j;
	int		ret;
	SSL		*ssl;
	char	timebuf[16];
	char styled_msg[BUFFER_SIZE + 64];

	get_current_time_str(timebuf, sizeof(timebuf));
	snprintf(styled_msg, sizeof(styled_msg), "%s" COLOR_INFO "%s" COLOR_RESET, timebuf, msg);
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0 && fd->connected_socket_fd[j]
			!= fd->srv_fd && j != exclude_idx)
		{
			ssl = fd->client_ssl[j];
			if (ssl != NULL)
			{
				ret = SSL_write(ssl, msg, strlen(msg));
				if (ret <= 0)
				{
					fprintf(stderr, "Erreur lors de l'envoi SSL vers %s\n",
						fd->pseudos[j]);
					handle_client_disconnect(fd, j);
				}
			}
		}
	}
}

void	broadcast_welcome_message(t_fd *fd, int exclude_idx)
{
	int		j;
	int		ret;
	char	msg[BUFFER_SIZE];
	SSL		*ssl;

	printf(COLOR_INFO "%s nous a rejoint \n" COLOR_RESET, fd->pseudos[exclude_idx]);
	snprintf(msg, sizeof(msg), COLOR_WELCOME "*** %s a rejoint le chat ***" COLOR_RESET, fd->pseudos[exclude_idx]);
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0 && fd->connected_socket_fd[j]
			!= fd->srv_fd && j != exclude_idx)
		{
			ssl = fd->client_ssl[j];
			if (ssl != NULL)
			{
				ret = SSL_write(ssl, msg, strlen(msg));
				if (ret <= 0)
				{
					fprintf(stderr, "Erreur envoi SSL:  %s\n", fd->pseudos[j]);
					handle_client_disconnect(fd, j);
				}
			}
		}
	}
}

void	broadcast_goodbye_message(t_fd *fd, int exclude_idx)
{
	int		j;
	int		ret;
	char	msg[BUFFER_SIZE];
	SSL		*ssl;

	printf(COLOR_GOODBYE "%s déconnecté \n" COLOR_RESET, fd->pseudos[exclude_idx]);
	snprintf(msg, sizeof(msg), COLOR_GOODBYE "*** %s a quitté le chat ***" COLOR_RESET, fd->pseudos[exclude_idx]);
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0 && fd->connected_socket_fd[j]
			!= fd->srv_fd && j != exclude_idx)
		{
			ssl = fd->client_ssl[j];
			if (ssl != NULL)
			{
				ret = SSL_write(ssl, msg, strlen(msg));
				if (ret <= 0)
				{
					fprintf(stderr, "Erreur envoi SSL: %s\n", fd->pseudos[j]);
					handle_client_disconnect(fd, j);
				}
			}
		}
	}
}

void	close_and_notify_server_full(int *new_client, SSL *ssl)
{
	const char	*msg = COLOR_RED "Full: serveur saturé\n" COLOR_RESET ;

	if (ssl)
	{
		if (SSL_write(ssl, msg, (int)strlen(msg)) <= 0)
		{
			fprintf(stderr, "Erreur envoi du message serveur plein via SSL\n");
		}
		SSL_shutdown(ssl);
		SSL_free(ssl);
	}
	if (new_client && *new_client > 0)
		close(*new_client);
	printf("Max Connexion reached\n");
}
