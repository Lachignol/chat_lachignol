/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sslInit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ascordil <ascordil@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/27 04:51:17 by ascordil          #+#    #+#             */
/*   Updated: 2025/07/27 22:14:56 by ascordil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.h"

SSL_CTX	*init_tls_context(void)
{
	SSL_CTX	*ctx;

	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
	ctx = SSL_CTX_new(TLS_server_method());
	if (!ctx)
	{
		fprintf(stderr, "Erreur création contexte SSL\n");
		ERR_print_errors_fp(stderr);
		exit(EXIT_FAILURE);
	}
	if (SSL_CTX_use_certificate_file(ctx, "cert.pem", SSL_FILETYPE_PEM) <= 0
		||SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0)
	{
		fprintf(stderr, "Erreur chargement certif/clé\n");
		ERR_print_errors_fp(stderr);
		SSL_CTX_free(ctx);
		exit(EXIT_FAILURE);
	}
	return (ctx);
}
