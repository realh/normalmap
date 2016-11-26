/*
    normalmap - Convert PNGs from height maps to normal maps for games.
    Copyright (C) 2016 Tony Houghton <h@realh.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include "options.h"

#ifdef _WIN32
void set_stdin_to_binary(void)
{
    _setmode(0, O_to_binary);
}

void set_stdout_to_binary(void)
{
    _setmode(1, O_to_binary);
}
#else
void set_stdin_to_binary(void)
{
    freopen(NULL, "rb", stdin);
}

void set_stdout_to_binary(void)
{
    freopen(NULL, "wb", stdout);
}
#endif

int main(int argc, char **argv)
{
    NormalmapOptions *options = normalmap_options_get(argc, argv);
    FILE *fp_in, *fp_out;

    /*
    printf("xyz = '%s', unsigned = %d, normalise = %d\n",
            options->xyz, options->unsigned_z, options->normalise);
    */

    if (!options->input || !strcmp(options->input, "-"))
    {
        set_stdin_to_binary();
        fp_in = stdin;
    }
    else
    {
        fp_in = fopen(options->input, "rb");
        if (!fp_in)
        {
            fprintf(stderr, "Could not open input file '%s': %s\n",
                    options->input, strerror(errno));
            exit(1);
        }
    }

    if (!options->input || !strcmp(options->output, "-"))
    {
        set_stdout_to_binary();
        fp_out = stdout;
    }
    else
    {
        fp_out = fopen(options->output, "wb");
        if (!fp_out)
        {
            fprintf(stderr, "Could not open output file '%s': %s\n",
                    options->output, strerror(errno));
            exit(1);
        }
    }

    return 0;
}
