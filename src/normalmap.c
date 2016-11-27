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

#include <stdlib.h>
#include <string.h>

#include "normalmap.h"

static int byte_offset(char axis)
{
    switch (axis)
    {
        case 'r':
            return 0;
        case 'g':
            return 1;
        case 'b':
            return 2;
        case 'a':
            return 3;
    }
    return 0;
}

typedef struct
{
    double x, y, z;
} NormalVector;

static NormalVector sobel(NormalmapPng *heightmap, int x, int y,
        double scale, double offset, int wrap)
{
    NormalVector nv;

    return nv;
}

static unsigned char d_to_signed_byte(double v)
{
    v *= 128;
    if (v == 128)
        v = 127;
    return (unsigned char) v + 128;
}

static unsigned char d_to_unsigned_byte(double v)
{
    v *= 256;
    if (v == 256)
        v = 255;
    return (unsigned char) v;
}

NormalmapPng *normalmap_convert(NormalmapPng *heightmap,
        NormalmapOptions *options)
{
    unsigned bpp;           /* bytes per pixel */
    NormalmapPng *nmap;
    int xo, yo, zo;
    unsigned int y, x, n;
    double scale, offset;

    nmap = normalmap_png_new();
    nmap->info.width = heightmap->info.width;
    nmap->info.height = heightmap->info.height;
    if (strchr(options->xyz, 'a'))
    {
        bpp = 4;
        nmap->info.format = PNG_FORMAT_RGBA;
    }
    else
    {
        bpp = 3;
        nmap->info.format = PNG_FORMAT_RGB;
    }
    nmap->data = malloc(nmap->info.height * nmap->info.width * bpp);

    xo = byte_offset(options->xyz[0]);
    yo = byte_offset(options->xyz[1]);
    zo = byte_offset(options->xyz[2]);

    if (options->normalise)
    {
        unsigned char min = 255, max = 0;
        unsigned char pix;

        x = heightmap->info.height * heightmap->info.width;
        for (y = 0; y < x; ++y)
        {
            pix = heightmap->data[y];
            if (pix > max)
                max = pix;
            if (pix < min)
                min = pix;
        }
        scale = (double) (max - min) / 255.0 * options->scale;
        offset = (double) min / -255.0;
    }
    else
    {
        scale = options->scale / 255.0;
        offset = 0.0;
    }

    n = 0;
    for (y = 0; y < nmap->info.height; ++y)
    {
        for (x = 0; x < nmap->info.width; ++x)
        {
            NormalVector v = sobel(heightmap, x, y, scale, offset,
                    options->wrap);

            nmap->data[n + xo] = d_to_signed_byte(v.x);
            nmap->data[n + yo] = d_to_signed_byte(v.y);
            nmap->data[n + zo] = options->unsigned_z ?
                d_to_unsigned_byte(v.z) : d_to_signed_byte(v.z);

            n += bpp;
        }
    }

    return nmap;
}
