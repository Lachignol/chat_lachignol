/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:05:23 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/18 02:07:52 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H

# define CLIENT_H

# include <sys/socket.h>
# include <stdio.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

# define LISTENING_PORT 8080
# define PSEUDO_MAX_LENGTH 20
# define INPUT_SIZE 512 
# define BUFFER_SIZE 1024
# define SERVER_ADDR "127.0.0.1"
# define PSEUDO "bobby"

typedef struct data_s
{
	char	buffer[BUFFER_SIZE];
	char	input[INPUT_SIZE];
	char	fullmsg[BUFFER_SIZE];
}	t_data;

void	check_pseudo(char *pseudo_macro, size_t max_length_pseudo);
void	init_server_socket(int *server_fd, struct sockaddr_in *socketAddress,
			int *max_fd);
void	try_to_connect_serveur(int *server_fd, struct sockaddr_in *sockAdd);
void	monitor_and_select(fd_set *monitoring_fd, int *server_fd, int *max_fd);
void	monitor_read(int *server_fd, fd_set *monitoring_fd, t_data *data);
void	monitor_input(fd_set *monit_fd, t_data *data, int *srv_fd);

#endif
