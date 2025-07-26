/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:05:23 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 23:15:56 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H

# define CLIENT_H

# include <sys/socket.h>
# include <stdio.h>
# include <ctype.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
// attention a modifie pour le serveur aussi 
// si on change la longeur max du pseudo
# define LISTENING_PORT 8080
# define PSEUDO_MAX_LENGTH 20 
# define INPUT_SIZE 512 
# define BUFFER_SIZE 1024
# define SERVER_ADDR "127.0.0.1"
# define PSEUDO "bobby"

typedef struct server_info
{
	int		server_fd;
	int		port;
	char	*pseudo;
	char	*server_addr;
}	t_server;

typedef struct data_s
{
	char	buffer[BUFFER_SIZE];
	char	input[INPUT_SIZE];
	char	fullmsg[BUFFER_SIZE];
}	t_data;

int		check_inputs(char **argv, t_server *srv);
int		check_pseudo(char *pseudo);
int		check_port(char *portchar, int *port);
int		check_adress(char *adress);
void	init_server_socket(t_server *server, struct sockaddr_in *socketAddress,
			int *max_fd);
void	try_to_connect_serveur(t_server *server, struct sockaddr_in *sockAdd);
void	monitor_and_select(fd_set *monitoring_fd,
			t_server *server, int *max_fd);
void	monitor_read(t_server *server, fd_set *monitoring_fd, t_data *data);
void	monitor_input(fd_set *monit_fd, t_data *data, t_server *server);

#endif
