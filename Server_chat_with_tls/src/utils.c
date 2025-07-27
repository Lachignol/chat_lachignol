/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 07:23:20 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/28 00:49:37 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

const char *user_colors[] = {
    "\033[1;31m", // Rouge
    "\033[1;32m", // Vert
    "\033[1;33m", // Jaune
    "\033[1;34m", // Bleu
    "\033[1;35m", // Magenta
    "\033[1;36m", // Cyan
    "\033[1;37m", // Blanc clair
    "\033[0;31m", // Rouge normal
    "\033[0;32m", // Vert normal
    "\033[0;33m", // Jaune normal
    "\033[0;34m", // Bleu normal
    "\033[0;35m", // Magenta normal
    "\033[0;36m", // Cyan normal
};

unsigned long	djb2_hash(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
    {
        if (c >= 'A' && c <= 'Z')
            c += ('a' - 'A');
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

int	get_color_index(const char *pseudo)
{
    return (int)(djb2_hash(pseudo) % NUM_USER_COLORS);
}

void	get_current_time_str(char *buffer, size_t size)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, size, "[%H:%M:%S] ", tm_info);
}

int	send_connected_users_list(t_fd *fd, SSL *ssl, int self_idx)
{
	char user_list_msg[BUFFER_SIZE * 2];
	int offset = 0;
	int j;
	int send_bytes;
	int	count;

	offset += snprintf(user_list_msg + offset, sizeof(user_list_msg) - offset,
                       "\nListe des utilisateurs connectés :\n");
	count = 0;
	j = -1;
	while (++j < CONNEXION_LIMIT)
	{
		if (fd->connected_socket_fd[j] != 0 && j != self_idx)
		{
			offset += snprintf(user_list_msg + offset, sizeof(user_list_msg) - offset, " - %s -\n", fd->pseudos[j]);
			count++;
		}
	}
	if (count == 0)
		offset += snprintf(user_list_msg + offset, sizeof(user_list_msg) - offset, "*aucun autre utilisateur connecté*\n");
	offset += snprintf(user_list_msg + offset, sizeof(user_list_msg) - offset, "\n");
	send_bytes = SSL_write(ssl, user_list_msg, offset);
	if (send_bytes <= 0)
	{
		fprintf(stderr, "Erreur lors de l'envoi de la liste des utilisateurs connectés\n");
		handle_client_disconnect(fd, self_idx);
		return (0);
	}
	return (1);
}

