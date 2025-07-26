/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:22:53 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 01:51:22 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

int	main(void)
{
	int						connected_socket_fd[CONNEXION_LIMIT];
	char					buffer[BUFFER_SIZE];
	t_sock_adress			socket_address;
	t_fd					fds;

	memset(&fds, 0, sizeof(fds));
	init_and_link_srv_socket(&fds, &socket_address);
	listning(&(fds.srv_fd));
	init_var(buffer, connected_socket_fd, &socket_address);
	while (1)
	{
		init_monitoring(&fds);
		monitor(&fds, &socket_address, buffer);
	}
}
