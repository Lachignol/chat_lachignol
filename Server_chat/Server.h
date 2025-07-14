/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:15:18 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/15 01:54:15 by ascordil         ###   ########.fr       */
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
# include <errno.h>

# define LISTENING_PORT 8080
# define CONNEXION_LIMIT 3
# define BUFFER_SIZE 1024
# define WELCOME_MSG "Bienvenue sur le serveur de la chignol\n"

void	init_monitoring(fd_set *monitor_fd, int *srv_fd, int *connectedSocketFd);
void	monitor(int *srv_fd, fd_set *monitoring_fd, socklen_t *socketAdressLength, int *connectedSocketFd, struct sockaddr_in *socketAdrress, char *buffer);
void	init_var(char *buffer, int *connectedSocketFd, socklen_t *socket_add_size, struct sockaddr_in *sock_address);
void	listning(int *srv_fd);
void	init_and_link_srv_socket(int *srv_fd, struct sockaddr_in *sockadd);

#endif
