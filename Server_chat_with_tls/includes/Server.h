/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 22:15:18 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/28 00:46:58 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H

# define SERVER_H

# include <sys/socket.h>
# include <stdio.h>
# include <time.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <ctype.h>
# include <errno.h>
# include <ifaddrs.h>
# include <openssl/ssl.h>
# include <openssl/err.h>
# include <arpa/inet.h>

# define NUM_USER_COLORS (sizeof(user_colors) / sizeof(user_colors[0]))
# define COLOR_INFO    "\033[1;33m"
# define COLOR_RESET   "\033[0m"
# define COLOR_WELCOME  "\033[1;32m"
# define COLOR_GOODBYE  "\033[1;31m"
# define COLOR_RED     "\033[0;31m"
# define COLOR_GREEN   "\033[0;32m"
# define COLOR_YELLOW  "\033[0;33m"
# define COLOR_BLUE    "\033[0;34m"
# define COLOR_MAGENTA "\033[0;35m"
# define COLOR_CYAN    "\033[0;36m"
# define COLOR_BOLD    "\033[1m"
# define LISTENING_PORT 8080
# define CONNEXION_LIMIT 10 
# define PSEUDO_MAX_LENGTH 20
# define BUFFER_SIZE 1024
# define WELCOME_MSG "Bienvenue sur le serveur de la chignol\n"
#define SECURITY_BANNER_COLORED "\033[1;34m\n" \
"*****************************************************\n" \
"*                                                   *\n" \
"*            Serveur de Chat Sécurisé               *\n" \
"*                                                   *\n" \
"*   Ce serveur garantit la confidentialité et la    *\n" \
"*       sécurité de vos échanges grâce à TLS.       *\n" \
"*                                                   *\n" \
"*   Toutes les communications sont chiffrées pour   *\n" \
"*       protéger vos données en transit.            *\n" \
"*                                                   *\n" \
"*        Créé avec passion par Lachignol            *\n" \
"*                                                   *\n" \
"*****************************************************\n\n" \
"\033[0m"

extern const char *user_colors[]; 

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
	SSL		*client_ssl[CONNEXION_LIMIT];

}	t_fd;

int		send_connected_users_list(t_fd *fd, SSL *ssl, int self_idx);
int		sanitize_and_check_exit(char *buffer, int bytes, t_fd *fd, int idx);
int		authenticate_user(int *new_client, char pseudo[PSEUDO_MAX_LENGTH],
			SSL *ssl);
int		add_client_to_fd(t_fd *fd, int new_client, char *pseudo, SSL *ssl);
int		get_color_index(const char *pseudo);
void	init_monitoring(t_fd *fd);
void	monitor(t_fd *fd, t_sock_adress *socket_address, char *buffer,
			SSL_CTX *ctx);
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
void	close_and_notify_server_full(int *new_client, SSL *ssl);
void	add_new_contact(t_fd *fd, t_sock_adress *sock_ad, SSL_CTX *ctx);
void	get_current_time_str(char *buffer, size_t size);
SSL_CTX	*init_tls_context(void);

#endif
