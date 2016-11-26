#pragma once

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

#include <stdio.h>
#include <png.h>

typedef struct
{
    png_image info;
    unsigned char *data;
} NormalmapPng;


NormalmapPng *normalmap_load_png(FILE *fp);

void normalmap_save_png(NormalmapPng *npng, FILE *fp);

/* Returns NULL */
NormalmapPng *normalmap_free_png(NormalmapPng *npng);
