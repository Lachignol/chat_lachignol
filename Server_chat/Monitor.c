/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 23:33:45 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/15 01:49:19 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

void	init_monitoring(fd_set *monitor_fd, int *srv_fd, int *connectedSocketFd)
{
	int		monitoring;
	int		max_fd;
	int		i;

	FD_ZERO(&(*(monitor_fd)));
	FD_SET(*srv_fd, &(*(monitor_fd)));
	max_fd = *srv_fd;
	i = -1;
	while (++i < CONNEXION_LIMIT)
	{
		if (connectedSocketFd[i] > 0)
		{
			FD_SET(connectedSocketFd[i], &(*(monitor_fd)));
		}
		if (connectedSocketFd[i] > max_fd)
			max_fd = connectedSocketFd[i];
	}
	monitoring = select(max_fd + 1, &(*(monitor_fd)), NULL, NULL, NULL);
	if (monitoring < 0 && errno != EINTR)
		perror("select error");
}

void	add_new_contact(int *srv_fd, fd_set *monitoring_fd, socklen_t *socketAdressLength, int *connectedSocketFd, struct sockaddr_in *socketAdrress)
{
	int		new_client;
	int		send_bytes;
	int		i;
	char	*welcome_msg;

	if (FD_ISSET(*srv_fd, &(*(monitoring_fd))))
	{
		i = -1;
		new_client = accept(*srv_fd, (struct sockaddr *)&(*(socketAdrress)) , &(*(socketAdressLength)));
		while (++i < CONNEXION_LIMIT)
		{
			if (connectedSocketFd[i] == 0)
			{
				connectedSocketFd[i] = new_client;
				welcome_msg = WELCOME_MSG;
				send_bytes = send(connectedSocketFd[i], welcome_msg, strlen(welcome_msg), 0);
				if (send_bytes < 0)
					return (fprintf(stderr, "Erreur lors de l'envoi\n"),exit(1));
				printf("New chatter\n");
				return ;
			}
			if (i + 1 == CONNEXION_LIMIT)
				printf("Can't add anymore Client");
		}
	}
}

void	read_and_send(int *srv_fd, fd_set *monitoring_fd, socklen_t *socketAdressLength, int *connectedSocketFd, struct sockaddr_in *socketAdrress, char *buffer)	
{
	int	send_bytes;
	int	received_bytes;
	int	i;
	int	j;

	i = -1;
	while (++i < CONNEXION_LIMIT)
	{
		if (connectedSocketFd[i] > 0 && FD_ISSET(connectedSocketFd[i], &(*(monitoring_fd))))
		{
			received_bytes = read(connectedSocketFd[i], buffer, BUFFER_SIZE);
			if (received_bytes <= 0)
			{
				fprintf(stderr, "client deconnecter\n");
				close(connectedSocketFd[i]);
				connectedSocketFd[i] = 0;
			}
			else
			{
				buffer[received_bytes] = '\0';
				printf("Client: %s\n", buffer);
				j = -1;
				while (++j < CONNEXION_LIMIT)
				{
					if (connectedSocketFd[j] != 0 && connectedSocketFd[j] != *srv_fd
						&& connectedSocketFd[i] != connectedSocketFd[j])
					{
						send_bytes = send(connectedSocketFd[j], buffer, strlen(buffer), 0);
						if (send_bytes < 0)
							return (fprintf(stderr, "Erreur lors de l'envoi\n"), exit(1));
					}
				}
				memset(buffer, '\0', BUFFER_SIZE);
			}
		}
	}
}

void	monitor(int *srv_fd, fd_set *monitoring_fd, socklen_t *socketAdressLength, int *connectedSocketFd, struct sockaddr_in *socketAdrress, char *buffer)
{
	int		new_client;
	int		received_bytes;

	add_new_contact(srv_fd, monitoring_fd, socketAdressLength, connectedSocketFd, socketAdrress);
	read_and_send(srv_fd, monitoring_fd, socketAdressLength, connectedSocketFd, socketAdrress, buffer);
}
