/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:48:40 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/27 22:46:44 by ascordil         ###   ########.fr       */
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

int	try_to_connect_serveur(t_server *srv, struct sockaddr_in *sockAdd, SSL_CTX *ctx)
{
	ssize_t	sent_bytes;

    if (connect(srv->server_fd, (struct sockaddr *)sockAdd, sizeof(*sockAdd)) < 0)
	{
		fprintf(stderr, "Erreur lors de la connexion au serveur\n");
		return (0);
	}
	srv->client_ssl = SSL_new(ctx);
	if (!srv->client_ssl)
	{
		fprintf(stderr, "Erreur SSL_new\n");
		close(srv->server_fd);
		return (0);
	}
	SSL_set_fd(srv->client_ssl, srv->server_fd);
	if (SSL_connect(srv->client_ssl) <= 0)
	{
		fprintf(stderr, "Erreur SSL_connect (handshake TLS)\n");
		SSL_free(srv->client_ssl);
		srv->client_ssl = NULL;
		close(srv->server_fd);
		return (0);
	}
	sent_bytes = SSL_write(srv->client_ssl, srv->pseudo, strlen(srv->pseudo));
	if (sent_bytes <= 0)
	{
		perror("Erreur lors de l'envoi du pseudo via SSL");
		SSL_shutdown(srv->client_ssl);
		SSL_free(srv->client_ssl);
		srv->client_ssl = NULL;
		close(srv->server_fd);
		return (0);
	}
	return (1);
}
