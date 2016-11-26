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

#include "nmpng.h"

static NormalmapPng *normalmap_png_new()
{
    NormalmapPng *npng = malloc(sizeof(NormalmapPng));

    memset(npng, 0, sizeof(NormalmapPng));
    return npng;
}

NormalmapPng *normalmap_load_png(FILE *fp)
{
    NormalmapPng *npng = normalmap_png_new();

    npng->info.version = PNG_IMAGE_VERSION;
    if (!png_image_begin_read_from_stdio(&npng->info, fp))
    {
        fprintf(stderr, "Error loading PNG (begin): %s\n", npng->info.message);
        return normalmap_free_png(npng);
    }
    npng->info.format = PNG_FORMAT_GRAY;
    npng->data = malloc(npng->info.width * npng->info.height);
    if (!png_image_finish_read(&npng->info, NULL, npng->data,
            npng->info.width, NULL))
    {
        fprintf(stderr, "Error loading PNG (finish): %s\n", npng->info.message);
        return normalmap_free_png(npng);
    }
    return npng;
}

void normalmap_save_png(NormalmapPng *npng, FILE *fp)
{
    /* Make a new NormalmapPng in case resetting opaque clears other fields. */
    NormalmapPng *npng2 = normalmap_png_new();
    memcpy(npng2, npng, sizeof(NormalmapPng));
    npng2->info.opaque = NULL;
    npng2->info.flags = 0;
    if (!png_image_write_to_stdio(&npng2->info, fp, 0, npng2->data, 0, NULL))
    {
        fprintf(stderr, "Error saving PNG: %s\n", npng2->info.message);
    }
    npng2->data = NULL;
    normalmap_free_png(npng2);
}

NormalmapPng *normalmap_free_png(NormalmapPng *npng)
{
    png_image_free(&npng->info);
    free(npng->data);
    free(npng);
    return NULL;
}
