
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

strat_ctx strat_init (int argc, char * argv [])
{
   strat_ctx ctx = calloc (sizeof (*ctx), 1);

   if (!ctx)
   {
      fprintf (stderr, "Error allocating engine context");
      return 0;
   }

   if (! (ctx->config = sut_json_load ("config.json")))
   {
      fprintf (stderr, "Engine: Unable to load config.json\n");
      return 0;
   }

   ctx->win_width = sut_json_int (ctx->config, "winWidth", 800);
   ctx->win_height = sut_json_int (ctx->config, "winHeight", 600);
   ctx->fullscreen = sut_json_bool (ctx->config, "fullscreen", false);

   if (! (ctx->game_def = sut_json_load ("game/game.json")))
   {
      fprintf (stderr, "Engine: Unable to load game definition file\n");
      return 0;
   }

   ctx->game_title = sut_json_string (ctx->game_def, "title", strat_version);
   ctx->tick_rate = sut_json_int (ctx->game_def, "tickRate", 60);

   return ctx;
}

void strat_init_gfx (strat_ctx ctx)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();

   glViewport (0, 0, ctx->win_width, ctx->win_height);
   glOrtho (0, ctx->win_width, ctx->win_height, 0, 1.0f, -1.0f);

   glDisable (GL_CULL_FACE);
   glEnable (GL_TEXTURE_2D);

   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glEnableClientState (GL_VERTEX_ARRAY);
   glEnableClientState (GL_TEXTURE_COORD_ARRAY);

   font_init (&ctx->ui_font, "Vera", 9);

   tile_init (ctx, &ctx->empty_tile, "empty");
   map_init (ctx, &ctx->map, "grass");

   camera_center (ctx, (ctx->map.width * ctx->map.tile_width) / 2,
                       (ctx->map.height * ctx->map.tile_height) / 2);

   unit_types_load (ctx);
}

void strat_free (strat_ctx ctx)
{
   unit_types_unload (ctx);

   free (ctx);
}

void strat_draw (strat_ctx ctx)
{
   glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
   glClear (GL_COLOR_BUFFER_BIT);

   map_draw (ctx, &ctx->map);

   char status[128];
   //sprintf (status, "Camera: %d, %d", ctx->camera_x, ctx->camera_y);
   sut_point m = screenspace_to_mapspace(ctx->cursor_x, ctx->cursor_y);
   sprintf (status, "Mouse: %d, %d", m.x + ctx->camera_x, m.y + ctx->camera_y);
   text_draw (&ctx->ui_font, 0, 0, 0, 0, status, 0);

   GLenum error = glGetError();

   if (error != GL_NO_ERROR)
   {
      fprintf (stderr, "OpenGL error: %d\n", error);
   }
}

bool strat_tick (strat_ctx ctx)
{
   /*ctx->camera_x = ctx->cursor_x - (ctx->win_width / 2);
   ctx->camera_y = ctx->cursor_y - (ctx->win_height / 2);*/

   return true;
}


