/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckInput.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 22:31:12 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/26 23:51:34 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.h"

int	check_pseudo(char *pseudo)
{
	size_t	len;
	size_t	i;

	len = strlen(pseudo);
	if (len > PSEUDO_MAX_LENGTH)
	{
		fprintf(stderr, "Erreur : pseudo trop long (max %d caractères)\n",
			PSEUDO_MAX_LENGTH);
		return (0);
	}
	i = -1;
	while (++i < len)
	{
		if (!isalnum((unsigned char)pseudo[i]) && pseudo[i]
			!= '_' && pseudo[i] != '-')
		{
			fprintf(stderr,
				"Erreur : pseudo contient des caractères invalides\n");
			return (0);
		}
	}
	return (1);
}

int	check_port(char *portchar, int *port)
{
	long	port_long;
	char	*endptr;

	port_long = strtol(portchar, &endptr, 10);
	if (*endptr != '\0' || port_long <= 0 || port_long > 65535)
	{
		fprintf(stderr, "Erreur : port invalide\n");
		return (0);
	}
	*port = (int)port_long;
	return (1);
}

int	check_adress(char *server_addr)
{
	struct in_addr	addr;

	if (inet_pton(AF_INET, server_addr, &addr) != 1)
	{
		fprintf(stderr, "Erreur : adresse IP invalide\n");
		return (0);
	}
	return (1);
}

int	check_inputs(char **argv, t_server *srv)
{
	char	*pseudo;
	char	*server_addr;
	int		port;

	pseudo = argv[1];
	server_addr = argv[2];
	port = atol(argv[3]);
	if (!check_pseudo(pseudo))
		return (0);
	if (!check_port(argv[3], &port))
		return (0);
	if (!check_adress(server_addr))
		return (0);
	srv->server_addr = argv[2];
	srv->pseudo = argv[1];
	srv->port = (int)port;
	return (1);
}
