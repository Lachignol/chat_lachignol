/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:22:53 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/28 11:52:19 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

int	main(void)
{
	int						connected_socket_fd[CONNEXION_LIMIT];
	char					buffer[BUFFER_SIZE];
	t_sock_adress			socket_address;
	t_fd					fds;
	SSL_CTX					*ctx;

	ctx = init_tls_context();
	memset(&fds, 0, sizeof(fds));
	memset(fds.client_ssl, 0, sizeof(fds.client_ssl));
	init_and_link_srv_socket(&fds, &socket_address);
	listning(&(fds.srv_fd));
	printf(COLOR_BLUE "Serveur en ecoute sur le port : %d\n\n\n" COLOR_RESET, LISTENING_PORT);
	init_var(buffer, connected_socket_fd, &socket_address);
	while (1)
	{
		init_monitoring(&fds);
		monitor(&fds, &socket_address, buffer, ctx);
		if (exit_input(&fds))
			break ;
	}
	SSL_CTX_free(ctx);
	clear_and_exit_all(&fds, &socket_address);
	return (0);
}
