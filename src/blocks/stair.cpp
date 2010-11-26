/*
   Copyright (c) 2010, The Mineserver Project
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
 * Neither the name of the The Mineserver Project nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
   ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "stair.h"

void BlockStair::onStartedDigging(User* user, sint8 status, sint32 x, sint8 y, sint32 z, sint8 direction)
{

}

void BlockStair::onDigging(User* user, sint8 status, sint32 x, sint8 y, sint32 z, sint8 direction)
{

}

void BlockStair::onStoppedDigging(User* user, sint8 status, sint32 x, sint8 y, sint32 z, sint8 direction)
{

}

void BlockStair::onBroken(User* user, sint8 status, sint32 x, sint8 y, sint32 z, sint8 direction)
{
}

void BlockStair::onNeighbourBroken(User* user, sint8 status, sint32 x, sint8 y, sint32 z, sint8 direction)
{
   /* TODO: add code to align stairs? */
}

void BlockStair::onPlace(User* user, sint8 newblock, sint32 x, sint8 y, sint32 z, sint8 direction)
{
   uint8 oldblock;
   uint8 oldmeta;
   uint8 topblock;
   uint8 topmeta;

   if (Map::get().getBlock(x, y, z, &oldblock, &oldmeta))
   {
      /* Check block below allows blocks placed on top */
      switch(oldblock)
      {
         case BLOCK_WORKBENCH:
         case BLOCK_FURNACE:
         case BLOCK_BURNING_FURNACE:
         case BLOCK_CHEST:
         case BLOCK_JUKEBOX:
         case BLOCK_TORCH:
         case BLOCK_REDSTONE_TORCH_OFF:
         case BLOCK_REDSTONE_TORCH_ON: 
         case BLOCK_WATER:
         case BLOCK_STATIONARY_WATER:
         case BLOCK_LAVA:
         case BLOCK_STATIONARY_LAVA:
          return;
         break;
         default:
            if (Map::get().getBlock(x, y+1, z, &topblock, &topmeta) && topblock == BLOCK_AIR)
            {
               // Get the correct orientation
               topmeta = 1;
               if (direction)
                  topmeta = 6 - direction;
               /*
                  TODO: Compare result with code below

                if (y == oy && (x != ox || y != oy || z != oz))
                {
                  // We place according to the target block

                  if (x < ox)
                    metadata = 0x0;
                  else if (x > ox)
                    metadata = 0x1;
                  else if (z < oz)
                    metadata = 0x2;
                  else
                    metadata = 0x3;
                }
                else
                {
                  // We place according to the player's position

                  double diffX = x - user->pos.x;
                  double diffZ = z - user->pos.z;
                  
                  if (std::abs(diffX) > std::abs(diffZ))
                  {
                    // We compare on the x axis
                    
                    if (diffX > 0)
                      metadata = 0x0;
                    else
                      metadata = 0x1;
                  }
                  else
                  {
                    // We compare on the z axis
                    
                    if (diffZ > 0)
                      metadata = 0x2;
                    else
                      metadata = 0x3;
                  }
                }
               */
                             
               Map::get().setBlock(x, y+1, z, (char)newblock, topmeta);
               Map::get().sendBlockChange(x, y+1, z, (char)newblock, topmeta);
            }
         break;
      }
   }
}

void BlockStair::onNeighbourPlace(User* user, sint8 newblock, sint32 x, sint8 y, sint32 z, sint8 direction)
{
   /* Align neighbour to this stair */
}

void BlockStair::onReplace(User* user, sint8 newblock, sint32 x, sint8 y, sint32 z, sint8 direction)
{
   uint8 oldblock;
   uint8 oldmeta;
   
   switch(newblock)
   {
      case BLOCK_WATER:
      case BLOCK_STATIONARY_WATER:
         if (Map::get().getBlock(x, y, z, &oldblock, &oldmeta))
         {
            // spawn item
            Map::get().sendBlockChange(x, y, z, 0, 0);
            Map::get().setBlock(x, y, z, 0, 0);
            Map::get().createPickupSpawn(x, y, z, oldblock, 1);
         }
      break;
      case BLOCK_LAVA:
      case BLOCK_STATIONARY_LAVA:
         if (Map::get().getBlock(x, y, z, &oldblock, &oldmeta))
         {
            // destroy
            Map::get().sendBlockChange(x, y, z, 0, 0);
            Map::get().setBlock(x, y, z, 0, 0);
         }
      break;
      default:
         return;
      break;
   }
}
