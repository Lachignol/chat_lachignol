/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:07:23 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/27 23:48:41 by ascordil         ###   ########.fr       */
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

	if (FD_ISSET(srv->server_fd, monitoring_fd))
	{
		if (srv->client_ssl != NULL)
		{
			received_bytes = SSL_read(srv->client_ssl, data->buffer,
					BUFFER_SIZE - 1);
			if (received_bytes <= 0)
			{
				fprintf(stderr, COLOR_ERROR "Le serveur a fermé la connexion TLS.\n" COLOR_RESET);
				SSL_shutdown(srv->client_ssl);
				SSL_free(srv->client_ssl);
				srv->client_ssl = NULL;
				close(srv->server_fd);
				exit(1);
			}
			data->buffer[received_bytes] = '\0';
		}
		printf("\r\33[2K");
		printf("%s\n", data->buffer);
		printf(COLOR_PROMPT "> " COLOR_RESET);
		fflush(stdout);
		memset(data->buffer, '\0', BUFFER_SIZE);
	}
}

void	monitor_input(fd_set *monit_fd, t_data *data, t_server *srv)
{
	int	send_bytes;

	if (FD_ISSET(STDIN_FILENO, monit_fd))
	{
		if (fgets(data->input, INPUT_SIZE, stdin) == NULL)
		{
			fprintf(stderr, COLOR_ERROR "Erreur d'input\n" COLOR_RESET);
			return ;
		}
		data->input[strcspn(data->input, "\n")] = '\0';
		if (srv->client_ssl != NULL)
		{
			send_bytes = SSL_write(srv->client_ssl, data->input,
					strlen(data->input));
			if (send_bytes <= 0)
			{
				fprintf(stderr, COLOR_ERROR "Le serveur a fermé la connexion TLS.\n" COLOR_RESET);
				SSL_shutdown(srv->client_ssl);
				SSL_free(srv->client_ssl);
				srv->client_ssl = NULL;
				close(srv->server_fd);
				exit(1);
			}
		}
		memset(data->input, '\0', INPUT_SIZE);
		memset(data->buffer, '\0', BUFFER_SIZE);
		printf(COLOR_PROMPT "> " COLOR_RESET);
		fflush(stdout);
	}
}
