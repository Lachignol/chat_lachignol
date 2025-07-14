/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:11:36 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/14 22:12:35 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.h"

int	main(void)
{
	int							server_fd;
	int							max_fd;
	t_data						data;
	fd_set						monitoring_fd;
	struct sockaddr_in			socket_address;

	check_pseudo(PSEUDO, PSEUDO_MAX_LENGTH);
	init_server_socket(&server_fd, &socket_address, &max_fd);
	try_to_connect_serveur(&server_fd, &socket_address);
	while (1)
	{
		monitor_and_select(&monitoring_fd, &server_fd, &max_fd);
		monitor_read(&server_fd, &monitoring_fd, &data);
		monitor_input(&monitoring_fd, &data, &server_fd);
	}
}
