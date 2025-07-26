/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:48:40 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 23:15:33 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.h"

void	init_server_socket(t_server *srv, struct sockaddr_in *skAd, int *max_fd)
{
	int	inet_return_code;

	srv->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (srv->server_fd < 0)
	{
		fprintf(stderr, "Erreur lors de la creation du socket\n");
		exit(1);
	}
	(*(skAd)).sin_family = AF_INET;
	(*(skAd)).sin_port = htons(srv->port);
	inet_return_code = inet_pton(AF_INET, srv->server_addr,
			&skAd->sin_addr);
	if (inet_return_code < 0)
	{
		fprintf(stderr, "Adresse invalide\n");
		exit(1);
	}
	if (srv->server_fd > STDIN_FILENO)
		*max_fd = srv->server_fd;
	else
		*max_fd = STDIN_FILENO;
}

// voir pour ajouter mdp
void	try_to_connect_serveur(t_server *srv, struct sockaddr_in *sockAdd)
{
	ssize_t	sent_bytes;

	if (connect(srv->server_fd, (struct sockaddr *) sockAdd,
			sizeof(*sockAdd)) < 0)
	{
		fprintf(stderr, "Erreur lors de la conexion\n");
		exit(1);
	}
	sent_bytes = 0;
	sent_bytes = send(srv->server_fd, srv->pseudo, strlen(srv->pseudo), 0);
	if (sent_bytes < 0)
	{
		perror("Erreur lors de l'envoi du pseudo");
		close(srv->server_fd);
		exit(1);
	}
}
