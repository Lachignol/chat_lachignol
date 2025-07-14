/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:48:40 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/14 23:39:00 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

void	init_and_link_srv_socket(int *srv_fd, struct sockaddr_in *sockadd)
{
	*srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (*srv_fd < 0)
	{
		fprintf(stderr, "Erreur lors de la creation du socket\n");
		exit(1);
	}
	(*(sockadd)).sin_family = AF_INET;
	(*(sockadd)).sin_port = LISTENING_PORT;
	(*(sockadd)).sin_addr.s_addr = INADDR_ANY;
	if (bind(*srv_fd, (struct sockaddr *) &(*(sockadd)), sizeof((*(sockadd)))) < 0)
	{
		fprintf(stderr, "Erreur de linkage pour le socket\n");
		exit(1);
	}
}

void	listning(int *srv_fd)
{
	if (listen(*srv_fd, CONNEXION_LIMIT) < 0)
	{
		fprintf(stderr, "Erreur lors de la mise en ecoute\n");
		exit(1);
	}
	puts("Serveur en ecoute\n");
}

void	init_var(char *buffer, int *connectedSocketFd, socklen_t *socket_add_size, struct sockaddr_in *sock_address)
{
	*socket_add_size = sizeof(*sock_address);
	memset(buffer, '\0', BUFFER_SIZE);
	memset(connectedSocketFd, 0, sizeof(int) * CONNEXION_LIMIT);
}
