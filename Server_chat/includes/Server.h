/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:15:18 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 23:45:21 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H

# define SERVER_H

# include <sys/socket.h>
# include <stdio.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <ctype.h>
# include <errno.h>

# define LISTENING_PORT 8080
# define CONNEXION_LIMIT 3
# define PSEUDO_MAX_LENGTH 20
# define BUFFER_SIZE 1024
# define WELCOME_MSG "Bienvenue sur le serveur de la chignol\n"

typedef struct adress_s
{
	socklen_t			size;
	struct sockaddr_in	sock_address;

}	t_sock_adress;

typedef struct fd_s
{
	int		srv_fd;
	int		connected_socket_fd[CONNEXION_LIMIT];
	char	pseudos[CONNEXION_LIMIT][PSEUDO_MAX_LENGTH];
	fd_set	monitoring_fd;

}	t_fd;

int		sanitize_and_check_exit(char *buffer, int bytes, t_fd *fd, int idx);
int		authenticate_user(int *new_client, char pseudo[PSEUDO_MAX_LENGTH]);
int		add_client_to_fd(t_fd *fd, int new_client, char *pseudo);
void	init_monitoring(t_fd *fd);
void	monitor(t_fd *fd, t_sock_adress *socket_address, char *buffer);
void	read_and_send(t_fd *fd, char *buffer);
void	init_var(char *buffer, int *connectedSocketFd,
			t_sock_adress *socket_address);
void	listning(int *srv_fd);
void	init_and_link_srv_socket(t_fd *fd, t_sock_adress *socket_adress);
void	handle_client_data(t_fd *fd, char *buffer, int idx, int bytes);
void	handle_client_disconnect(t_fd *fd, int idx);
void	clear_and_exit(t_fd *fd, t_sock_adress *sock_address);
void	broadcast_message(t_fd *fd, const char *msg, int exclude_idx);
void	broadcast_goodbye_message(t_fd *fd, int exclude_idx);
void	broadcast_welcome_message(t_fd *fd, int exclude_idx);
void	broadcast_message(t_fd *fd, const char *msg, int exclude_idx);
void	close_and_notify_server_full(int *new_client);
void	add_new_contact(t_fd *fd, t_sock_adress *sock_ad);

#endif
