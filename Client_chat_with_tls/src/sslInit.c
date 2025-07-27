/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sslInit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 05:54:03 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/27 22:41:24 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.h"

SSL_CTX	*init_tls_context(void)
{
	SSL_CTX	*ctx;

	if (!OPENSSL_init_ssl(0, NULL))
	{
		fprintf(stderr, "Erreur initialisation OpenSSL\n");
		exit(EXIT_FAILURE);
	}
	ctx = SSL_CTX_new(TLS_client_method());
	if (!ctx)
	{
		fprintf(stderr, "Erreur création contexte SSL client\n");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	return (ctx);
}
