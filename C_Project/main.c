//./project1 input.txt my-output.txt
//gcc -lm -Wall -Wextra -Werror -std=gnu11 -o project1 main.c zipData.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zipData.h"

int main( int argc, char* argv[] )
{
	if ( argc != 3)
	{
		fprintf(stderr, "Usage: vectors <input-file> <output-file>\n");
		exit(1);
	}


	FILE* in;
	in = fopen(argv[1], "r");
	FILE* out;
	out = fopen(argv[2], "w");

	char command[10];
    char zipcode[25];
	char zipcode2[25];

	int nameMaxLength = 25;
	char file_name[nameMaxLength];
	char place_name[nameMaxLength];
	char county_name[nameMaxLength];
	FILE* info = fopen("US.txt", "r");

	fscanf(in, "%s", command );
	while( !feof(in) )
	{
		if ( strcmp( command, "filename" ) == 0 )
		{
			fscanf( in, " %s", file_name);
            info = filename(file_name);
			fprintf(out, "filename %s\n", file_name);
		}
		else if ( strcmp( command, "postal" ) == 0 )
		{
			fscanf( in, "%s", zipcode);
			filename(file_name);
            postal(zipcode, info, out);
        }
		else if ( strcmp( command, "place" ) == 0 )
		{
			fscanf(in, " %[^\n]s", place_name);
			filename(file_name);
            place(place_name, info, out);
        }
		else if ( strcmp( command, "county" ) == 0 )
		{
			fscanf(in, " %[^\n]s", county_name);

            county(county_name, info, out);
        }
		else if ( strcmp( command, "distance" ) == 0 )
		{
			fscanf( in, " %s %s", zipcode, zipcode2);

            distance(zipcode, zipcode2, info, out);
        }
		else
		{
			fprintf( stderr, "Unknown command: %s\n", command );
		}
		fscanf(in, "%s", command );

	}

	fclose(in);
	fclose(out);
	fclose(info);

	return 0;
}