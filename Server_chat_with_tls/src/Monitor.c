/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 23:33:45 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/27 23:25:57 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

void	handle_client_disconnect(t_fd *fd, int idx)
{
	if (fd->connected_socket_fd[idx] != 0)
	{
		if (fd->client_ssl[idx] != NULL)
		{
			SSL_shutdown(fd->client_ssl[idx]);
			SSL_free(fd->client_ssl[idx]);
			fd->client_ssl[idx] = NULL;
		}
		close(fd->connected_socket_fd[idx]);
		fd->connected_socket_fd[idx] = 0;
		broadcast_goodbye_message(fd, idx);
		memset(fd->pseudos[idx], 0, PSEUDO_MAX_LENGTH);
	}
}

void	handle_client_data(t_fd *fd, char *buffer, int idx, int bytes)
{
	int		j;
	int		ret;
	char	message[BUFFER_SIZE + PSEUDO_MAX_LENGTH + 64];
	SSL		*ssl;
	char		timebuf[16];
	time_t		now;
	struct tm	*tm_info;
	int color_idx = get_color_index(fd->pseudos[idx]);
	const char *color = user_colors[color_idx];

	if (!sanitize_and_check_exit(buffer, bytes, fd, idx))
		return ;
	now = time(NULL);
	tm_info = localtime(&now);
	strftime(timebuf, sizeof(timebuf), "%H:%M:%S", tm_info);
	snprintf(message, sizeof(message), "[" COLOR_INFO "%s" COLOR_RESET "] %s%s%s: %s", timebuf, color, fd->pseudos[idx], COLOR_RESET, buffer);
	printf("%s\n", message);
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0
			&& fd->connected_socket_fd[j] != fd->srv_fd
			&& fd->connected_socket_fd[j] != fd->connected_socket_fd[idx])
		{
			ssl = fd->client_ssl[j];
			if (ssl != NULL)
			{
				ret = SSL_write(ssl, message, strlen(message));
				if (ret <= 0)
				{
					fprintf(stderr, "Erreur envoi SSL: %s\n", fd->pseudos[j]);
					handle_client_disconnect(fd, j);
				}
			}
		}
	}
	memset(buffer, 0, BUFFER_SIZE);
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
			if (fd->client_ssl[i] != NULL)
			{
				received_bytes = SSL_read(fd->client_ssl[i], buffer, BUFFER_SIZE - 1);
				if (received_bytes <= 0)
				{
					handle_client_disconnect(fd, i);
					continue ;
				}
				buffer[received_bytes] = '\0';
			}
			handle_client_data(fd, buffer, i, received_bytes);
		}
	}
}

void	monitor(t_fd *fd, t_sock_adress *socket_adr, char *buff, SSL_CTX *ctx)
{
	add_new_contact(fd, socket_adr, ctx);
	read_and_send(fd, buff);
}
