#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S_IBUF_SIZE		(1 << 30)
#define S_OBUF_SIZE		(1 << 30)

int main( int argc, char *argv[] )
{
	int ncpus;
	char *ifile;
	int selcpu;

	FILE *fp;

	char *ibuf;
	char *obuf;

	int is_subaddr;

	int i;

	char *token;
	char *ep;


	if ( argc < 3 )
	{
		return 1;
	}

	ifile = argv[ 1 ];
	ncpus = atoi( argv[ 2 ] );
	selcpu = ( argc > 3 ) ? atoi( argv[ 3 ] ) : -1;
	if ( ( ncpus < 1 ) || ( selcpu >= ncpus ) )
	{
		return 2;
	}

	fp = fopen( ifile, "r" );
	if ( fp == NULL )
	{
		return 3;
	}

	ibuf = malloc( S_IBUF_SIZE );
	obuf = malloc( S_OBUF_SIZE );
	if ( ( ibuf == NULL ) || ( obuf == NULL ) )
	{
		return 4;
	}

	/* header */
	strcpy( obuf, "addr1,addr2" );
	for ( i = 0; i < ncpus; i++ )
	{
		sprintf( ibuf, ",cpu%d", i );
		strcat( obuf, ibuf );
	}
	strcat( obuf, ",location" );
	puts( obuf );

	while ( fgets( ibuf, S_IBUF_SIZE, fp ) != NULL )
	{
		obuf[ 0 ] = '\0';
		is_subaddr = ( ibuf[ 0 ] == ' ' );

		/* address */
		token = strtok( ibuf, " " );
		if ( token == NULL )
		{
			continue;
		}
		if ( is_subaddr )
		{
			strcat( obuf, "," );
			strcat( obuf, token );
		}
		else
		{
			strcat( obuf, token );
			strcat( obuf, "," );
		}
		strcat( obuf, "," );

		for ( i = 0; i < ncpus; i++ )
		{
			/* sample */
			token = strtok( NULL, " " );
			if ( token == NULL )
			{
				break;
			}
			strtol( token, &ep, 10 );
			if ( *ep != '\0' )
			{
				break;
			}

			strcat( obuf, token );
			strcat( obuf, "," );

			/* percent (not print) */
			token = strtok( NULL, " " );
			if ( token == NULL )
			{
				break;
			}
		}

		if ( i != ncpus )
		{
			continue;
		}

		/* note */
		token = strtok( NULL, "\n" );
		if ( token == NULL )
		{
			continue;
		}
		strcat( obuf, token );

		puts( obuf );
	}

	return 0;
}
