
/* vim: set et ts=3 sw=3 ft=c:
 *
 * Copyright (C) 2013 James McLaughlin.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "common.h"

bool map_init (strat_ctx ctx, strat_map map, const char * name)
{
   char filename [strat_max_path];
   snprintf (filename, sizeof (filename), "game/map/%s.json", name);

   if (! (map->json = sut_json_load (filename)))
   {
      trace ("Error loading JSON for map %s", name);
      return false;
   }

   map->width = sut_json_int (map->json, "width", 4);
   map->height = sut_json_int (map->json, "height", 4);

   if (! (map->tiles = malloc (sizeof (strat_tile) * map->width * map->height)))
   {
      trace ("Error allocating memory for map %s", name);
      return false;
   }

   if (!tile_init (ctx, &map->default_tile, sut_json_string (map->json, "defaultTile", "")))
   {
      trace ("Error loading default tile for map %s", name);
      return false;
   }

   map->tile_width = map->default_tile.image.width;
   map->tile_height = map->default_tile.image.height;

   for (long x = 0; x < map->width; ++ x)
   {
      for (long y = 0; y < map->height; ++ y)
      {
         map->tiles [y * map->width + x] = &ctx->empty_tile;
      }
   }

   trace ("Loaded map %s (tile width = %d, tile height = %d)",
             name, map->tile_width, map->tile_height);

   return map;
}

void map_cleanup (strat_map map)
{
   tile_cleanup (&map->default_tile);
}

void map_draw (strat_ctx ctx, strat_map map)
{
   int num_drawn = 0;

   for (int i = 0; i < map->height; ++ i)
   {
      for (int j = map->width - 1; j >= 0; -- j)
      {
         strat_tile tile = map->tiles [i * map->width + j];

         int x = j * tile->image.width / 2 + i * tile->image.width / 2;
         int y = (i * tile->image.height / 2 - j * tile->image.height / 2);
 
         y -= (tile->image.height / 2);

         x -= ctx->camera.x;
         y -= ctx->camera.y;

         image_draw (&tile->image, x, y);

         char desc[128];
         sprintf(desc, "%d: %d, %d", num_drawn ++, j, i);

         text_draw (&ctx->ui_font,
                    x,
                    y,
                    tile->image.width,
                    tile->image.height,
                    desc,
                    text_draw_center);
      }
   }
}


