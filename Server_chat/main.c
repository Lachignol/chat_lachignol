/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:22:53 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/15 01:52:22 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

int	main(void)
{
	int					srv_fd;
	int					monitoring;
	int					connected_socket_fd[CONNEXION_LIMIT];
	char				buffer[BUFFER_SIZE];
	socklen_t			socket_add_size;
	fd_set				monitoring_fd;
	struct sockaddr_in	sock_address;

	init_and_link_srv_socket(&srv_fd, &sock_address);
	listning(&srv_fd);
	init_var(buffer, connected_socket_fd, &socket_add_size, &sock_address);
	while (1)
	{
		init_monitoring(&monitoring_fd, &srv_fd, connected_socket_fd);
		monitor(&srv_fd, &monitoring_fd, &socket_add_size, connected_socket_fd,
			&sock_address, buffer);
	}
}
