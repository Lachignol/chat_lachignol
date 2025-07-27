/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:11:36 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/27 22:39:57 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.h"

int	main(int argc, char **argv)
{
	int							max_fd;
	t_data						data;
	fd_set						monitoring_fd;
	struct sockaddr_in			socket_address;
	t_server					server;
	SSL_CTX						*ctx;

	if (argc == 4)
	{
		if (!check_inputs(argv, &server))
			return (1);
		ctx = init_tls_context();
		// pour ne pas verifier le certificat decomenter
		// SSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, NULL);
		init_server_socket(&server, &socket_address, &max_fd);
		if (!try_to_connect_serveur(&server, &socket_address, ctx))
		{
			fprintf(stderr, "Erreur de connexion TLS au serveur\n");
			SSL_CTX_free(ctx);
			return (1);
		}
		while (1)
		{
			monitor_and_select(&monitoring_fd, &server, &max_fd);
			monitor_read(&server, &monitoring_fd, &data);
			monitor_input(&monitoring_fd, &data, &server);
		}
	}
	else
	{
		fprintf(stderr, "Usage: %s <pseudo> <adresse_serveur> <port>\n",
			argv[0]);
		return (1);
	}
}
