normalmap
=========

This is a command-line utility to convert PNGs to normal maps for 3D texturing.
The input is converted to a greyscale, then a 3x3 Sobel filter is applied,
which interprets the luminance of each pixel as a height, and generates a
normal map.

Installation
------------

###Linux, Unix, OS X

Make sure you have installed an appropriate toolchain for your platform,
**pkg-config** and development headers etc for **libpng** (version 1.6.0 or
later) and the **popt** library.

If the **configure** script isn't present, also install **autoconf** and
**automake**, then run `autoreconf -i`.

The compilation and installation sequence is the usual:

```
./configure
make
sudo make install
```

The **INSTALL** file contains more details about the GNU buildsystem.

###Windows

I hope to make a standalone Windows executable available shortly.

Runtime options
---------------

`normalmap --help` shows a summary of the options. Arguments to short (single
dash, single letter) options should be separated by a space, arguments for long
options should follow an equals sign wiht no spaces. Here are some details of
each option.

###--input/-i and --output/-o

Self-explanatory, these are compulsory. Input and output are both in PNG format.

###--xyz/-x

The mapping of each normal's X, Y and Z components to colour channels in the
output. The default is `rgb` ie X maps to the red channel, Y to green, and Z to
blue. You may use `a` for the alpha channel.

This program follows the convention that X and Y are axes in the plane of the
texture and Z is perpendicular to the plane. Some game engines swap Y and Z.

###--normalise/-n

The greyscale image the program uses internally has one byte per pixel, with 0
being the minimum and 255 being the maximum. If this option is given, the image
is first scanned to find the lowest/dimmest and highest/brightest pixels. All
values are then scaled to expand the range of heights to the full 255. For
example, if the lowest is 64 and the highest 191, the heights will all be
doubled so that the difference between the highest and lowest becomes 255.

No apologies for the British spelling :-).

###--scale/-s

Luminance is interpreted so that 255 corresponds to a height of 1, equal to the
width of one pixel. You can use this option to scale that height.

###--unsigned/-u

When converting the vector values back to bytes for the output pixels, the
convention is that 0 in the output corresponds to -1.0 in the vector, 128
corresponds to 0.0, and 255 corresponds to 1.0. The Z component should always
be positive, so it's possible for game engines to use a different range for Z,
where a byte of 0 means Z = 0.0 and 255 corresponds to 1.0. This option enables
that alternative Z mapping.

###--wrap/-w

Use for textures that are usually tiled, so that one pixel beyond the right
edge is asumed to have the same value as the pixel on the left edge at the same
Y value.

###--tm/-t

A convenience option equivalent to `-xyz=agb --unsigned`. This makes normal
maps suitable for Trackmania.

Licence
-------

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
