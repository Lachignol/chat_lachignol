/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 02:48:40 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/18 03:09:58 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.h"

void	check_pseudo(char *pseudo_macro, size_t max_length_pseudo)
{
	if (strlen(pseudo_macro) > max_length_pseudo)
	{
		printf("PSEUDO trop long maximum 20 caracteres");
		exit(1);
	}
}

void	init_server_socket(int *srv_fd, struct sockaddr_in *sockAd, int *max_fd)
{
	int	inet_return_code;

	*srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (*srv_fd < 0)
	{
		fprintf(stderr, "Erreur lors de la creation du socket\n");
		exit(1);
	}
	(*(sockAd)).sin_family = AF_INET;
	(*(sockAd)).sin_port = LISTENING_PORT;
	inet_return_code = inet_pton(AF_INET, SERVER_ADDR, &(*(sockAd)).sin_addr);
	if (inet_return_code < 0)
	{
		fprintf(stderr, "Adresse invalide\n");
		exit(1);
	}
	if (*srv_fd > STDIN_FILENO)
		*max_fd = *srv_fd;
	else
		*max_fd = STDIN_FILENO;
}

void	try_to_connect_serveur(int *server_fd, struct sockaddr_in *sockAdd)
{
	if (connect(*server_fd, (struct sockaddr *) &(*(sockAdd)),
			sizeof(*sockAdd)) < 0)
	{
		fprintf(stderr, "Erreur lors de la conexion\n");
		exit(1);
	}
}
