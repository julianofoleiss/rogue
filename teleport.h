#ifndef __TELEPORT__
#define	__TELEPORT__

#include "object.h"
#include "CACaves/CACaves.h"

typedef struct teleport_t{
    tile_t* targetTile;
    room_t* targetRoom;
}teleport_t;

object_t* TELEPORT_new(tile_t* tile, tile_t* dest, room_t* targetRoom);
void TELEPORT_interact(object_t* teleport);

#endif	/* __TELEPORT__ */