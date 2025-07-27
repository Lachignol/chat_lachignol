/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:05:23 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/27 22:54:01 by ascordil         ###   ########.fr       */
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
# include <openssl/ssl.h>
# include <openssl/err.h>
// attention a modifie pour le serveur aussi 
// si on change la longeur max du pseudo
# define PSEUDO_MAX_LENGTH 20 
# define INPUT_SIZE 512 
# define BUFFER_SIZE 1024
# define SERVER_ADDR "127.0.0.1"
# define COLOR_RESET   "\033[0m"
# define COLOR_PROMPT  "\033[1;32m"  // vert clair / gras
# define COLOR_MESSAGE "\033[0;36m"  // cyan
# define COLOR_ERROR   "\033[0;31m"  // rouge
# define COLOR_INFO    "\033[0;33m"  // jaune

typedef struct server_info
{
	int		server_fd;
	int		port;
	char	*pseudo;
	char	*server_addr;
	SSL		*client_ssl;
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
int		try_to_connect_serveur(t_server *server, struct sockaddr_in *sockAdd,
			SSL_CTX *ctx);
void	monitor_and_select(fd_set *monitoring_fd,
			t_server *server, int *max_fd);
void	monitor_read(t_server *server, fd_set *monitoring_fd, t_data *data);
void	monitor_input(fd_set *monit_fd, t_data *data, t_server *server);
SSL_CTX	*init_tls_context(void);
#endif
