/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:11:36 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 23:51:11 by ascordil         ###   ########.fr       */
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

	if (argc == 4)
	{
		if (!check_inputs(argv, &server))
			return (1);
		init_server_socket(&server, &socket_address, &max_fd);
		try_to_connect_serveur(&server, &socket_address);
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
