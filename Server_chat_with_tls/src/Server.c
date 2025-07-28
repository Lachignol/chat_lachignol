/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:48:40 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/28 12:11:17 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

void	init_monitoring(t_fd *fd)
{
	int		monitoring;
	int		max_fd;
	int		i;

	max_fd = 0;
	monitoring = 0;
	FD_ZERO(&(fd->monitoring_fd));
	FD_SET(fd->srv_fd, &(fd->monitoring_fd));
	FD_SET(STDIN_FILENO, &(fd->monitoring_fd));
	max_fd = fd->srv_fd;
	i = -1;
	while (++i < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[i] > 0)
		{
			FD_SET(fd->connected_socket_fd[i], &(fd->monitoring_fd));
		}
		if (fd->connected_socket_fd[i] > max_fd)
			max_fd = fd->connected_socket_fd[i];
	}
	monitoring = select(max_fd + 1, &(fd->monitoring_fd), NULL, NULL, NULL);
	if (monitoring < 0 && errno != EINTR)
		perror("select error");
}

void	init_and_link_srv_socket(t_fd *fd, t_sock_adress *socket_adress)
{
	fd->srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd->srv_fd < 0)
	{
		fprintf(stderr, "Erreur lors de la creation du socket\n");
		exit(1);
	}
	socket_adress->sock_address.sin_family = AF_INET;
	socket_adress->sock_address.sin_port = htons(LISTENING_PORT);
	socket_adress->sock_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(fd->srv_fd, (struct sockaddr *) &(socket_adress->sock_address),
			sizeof(socket_adress->sock_address)) < 0)
	{
		fprintf(stderr, "Erreur de linkage pour le socket\n");
		clear_and_exit(fd, socket_adress);
	}
}

void	listning(int *srv_fd)
{
	if (listen(*srv_fd, CONNEXION_LIMIT) < 0)
	{
		fprintf(stderr, "Erreur lors de la mise en ecoute\n");
		exit(1);
	}
}

void	init_var(char *buffer, int *connectedSocketFd, t_sock_adress *sock_ad)
{
	sock_ad->size = sizeof(sock_ad->sock_address);
	memset(buffer, '\0', BUFFER_SIZE);
	memset(connectedSocketFd, 0, sizeof(int) * CONNEXION_LIMIT);
}

void	clear_and_exit(t_fd *fd, t_sock_adress *sock_address)
{
	memset(fd, 0, sizeof(*fd));
	memset(sock_address, 0, sizeof(*sock_address));
	exit(1);
}

void	clear_and_exit_all(t_fd *fd, t_sock_adress *sock_address)
{
	int	i;

	if (fd == NULL)
		return ;
	i = 0;
	while (i < CONNEXION_LIMIT)
	{
		if (fd->client_ssl[i] != NULL)
		{
			SSL_shutdown(fd->client_ssl[i]);
			SSL_free(fd->client_ssl[i]);
			fd->client_ssl[i] = NULL;
		}
		if (fd->connected_socket_fd[i] > 0)
		{
			close(fd->connected_socket_fd[i]);
			fd->connected_socket_fd[i] = 0;
		}
		i++;
	}
	memset(fd, 0, sizeof(*fd));
	memset(sock_address, 0, sizeof(*sock_address));
	exit(1);
}
