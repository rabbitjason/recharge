/*
   (c) Copyright 2000-2002  convergence integrated media GmbH.
   (c) Copyright 2002       convergence GmbH.
   
   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de> and
              Sven Neumann <sven@convergence.de>.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __IFUSIONSOUNDMUSICPROVIDER_H__
#define __IFUSIONSOUNDMUSICPROVIDER_H__

#include <direct/stream.h>

/*
 * probing context
 */
typedef struct {
     const char    *filename;
     
     const char    *mimetype;
     
     unsigned char  header[64];
     
     DirectStream  *stream;
} IFusionSoundMusicProvider_ProbeContext;


DFBResult IFusionSoundMusicProvider_Create( const char                 *filename, 
                                            IFusionSoundMusicProvider **interface );

#endif
