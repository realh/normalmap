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

#include <math.h>
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

inline static unsigned char sample_pixel(NormalmapPng *heightmap,
        unsigned x, unsigned y)
{
    return heightmap->data[y * heightmap->info.width + x];
}

inline static int pixel_diff(NormalmapPng *heightmap,
        unsigned x1, unsigned x2, unsigned y1, unsigned y2)
{
    return (int) sample_pixel(heightmap, x2, y2) -
        (int) sample_pixel(heightmap, x1, y1);
}

static int row_dh(NormalmapPng *heightmap, unsigned x, unsigned y, int wrap)
{
    unsigned w = heightmap->info.width;

    if (x == 1)
        return 0;

    if (x == 0)
    {
        if (wrap)
        {
            return pixel_diff(heightmap, 1, w - 1, y, y);
        }
        else
        {
            return pixel_diff(heightmap, 0, 1, y, y);
        }
    }
    else if (x == w - 1)
    {
        if (wrap)
        {
            return pixel_diff(heightmap, w - 2, 0, y, y);
        }
        else
        {
            return pixel_diff(heightmap, w - 2, w - 1, y, y);
        }
    }
    /* else */
    return pixel_diff(heightmap, x - 1, x + 1, y, y);
}

static int col_dh(NormalmapPng *heightmap, unsigned x, unsigned y, int wrap)
{
    unsigned h = heightmap->info.height;

    if (h == 1)
        return 0;

    if (y == 0)
    {
        if (wrap)
        {
            return pixel_diff(heightmap, x, x, 1, h - 1);
        }
        else
        {
            return pixel_diff(heightmap, x, x, 0, 1);
        }
    }
    else if (y == h - 1)
    {
        if (wrap)
        {
            return pixel_diff(heightmap, x, x, h - 2, 0);
        }
        else
        {
            return pixel_diff(heightmap, x, x, h - 2, h - 1);
        }
    }
    /* else */
    return pixel_diff(heightmap, x, x, y - 1, y + 1);
}

static NormalVector sobel(NormalmapPng *heightmap, unsigned x, unsigned y,
        double scale, int wrap)
{
    NormalVector nv;
    int dh;
    unsigned w = heightmap->info.width;
    unsigned h = heightmap->info.height;
    double div = 5.0;

    dh = row_dh(heightmap, x, y, wrap) * 2;
    if (y == 0)
    {
        if (wrap)
            dh += row_dh(heightmap, x, h - 1, wrap);
        else
            div -= 1.0;
    }
    else
    {
        dh += row_dh(heightmap, x, y - 1, wrap);
    }
    if (y == h - 1)
    {
        if (wrap)
            dh += row_dh(heightmap, x, 0, wrap);
        else
            div -= 1.0;
    }
    else
    {
        dh += row_dh(heightmap, x, y + 1, wrap);
    }
    nv.x = scale * (double) dh / div;

    div = 5.0;
    dh = col_dh(heightmap, x, y, wrap) * 2;
    if (x == 0)
    {
        if (wrap)
            dh += row_dh(heightmap, w - 1, y, wrap);
        else
            div -= 1.0;
    }
    else
    {
        dh += row_dh(heightmap, x - 1, y, wrap);
    }
    if (x == w - 1)
    {
        if (wrap)
            dh += row_dh(heightmap, 0, y, wrap);
        else
            div -= 1.0;
    }
    else
    {
        dh += row_dh(heightmap, x + 1, y, wrap);
    }
    nv.y = scale * (double) dh / div;

    div = sqrt(nv.x * nv.x + nv.y * nv.y + 1.0);
    nv.x /= div;
    nv.y /= div;
    nv.z = 1 / div;

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
    double scale;

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

    /*
     * We don't need to keep track of the minimum (offset), because we're
     * only interested in the difference between values.
     */
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
    }
    else
    {
        scale = options->scale / 255.0;
    }

    n = 0;
    for (y = 0; y < nmap->info.height; ++y)
    {
        for (x = 0; x < nmap->info.width; ++x)
        {
            NormalVector v = sobel(heightmap, x, y, scale, options->wrap);

            nmap->data[n + xo] = d_to_signed_byte(v.x);
            nmap->data[n + yo] = d_to_signed_byte(v.y);
            nmap->data[n + zo] = options->unsigned_z ?
                d_to_unsigned_byte(v.z) : d_to_signed_byte(v.z);

            n += bpp;
        }
    }

    return nmap;
}
