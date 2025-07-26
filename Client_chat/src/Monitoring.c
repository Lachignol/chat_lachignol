/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:07:23 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 22:05:45 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.h"

void	monitor_and_select(fd_set *monitoring_fd, t_server *srv, int *max_fd)
{
	int	monitoring;

	FD_ZERO(monitoring_fd);
	FD_SET(srv->server_fd, &(*monitoring_fd));
	FD_SET(STDIN_FILENO, &(*(monitoring_fd)));
	monitoring = select(*max_fd + 1, &(*(monitoring_fd)), NULL, NULL, NULL);
	if (monitoring < 0 && errno != EINTR)
		perror("select error");
}

void	monitor_read(t_server *srv, fd_set *monitoring_fd, t_data *data)
{
	int	received_bytes;

	if (FD_ISSET(srv->server_fd, &(*(monitoring_fd))))
	{
		received_bytes = read(srv->server_fd, data->buffer, BUFFER_SIZE - 1);
		if (received_bytes <= 0)
		{
			printf("Le serveur a fermé la connexion.\n");
			close(srv->server_fd);
			exit(1);
		}
		else
		{
			data->buffer[received_bytes] = '\0';
			printf("\r\033[2K");
			fflush(stdout);
			printf("%s\n> ", data->buffer);
			fflush(stdout);
			memset(data->buffer, '\0', BUFFER_SIZE);
		}
	}
}

void	monitor_input(fd_set *monit_fd, t_data *data, t_server *srv)
{
	int	send_bytes;

	if (FD_ISSET(STDIN_FILENO, &(*(monit_fd))))
	{
		if (fgets(data->input, INPUT_SIZE, stdin) == NULL)
			return ((void)fprintf(stderr, "Erreur d'input \n"));
		else
		{
			data->input[strcspn(data->input, "\n")] = '\0';
			send_bytes = send(srv->server_fd, data->input,
					strlen(data->input), 0);
			if (send_bytes < 0)
			{
				fprintf(stderr, "Erreur lors de l'envoi\n");
				exit(1);
			}
			memset(data->input, '\0', INPUT_SIZE);
			memset(data->buffer, '\0', BUFFER_SIZE);
			printf("> ");
			fflush(stdout);
		}
	}
}
