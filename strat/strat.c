
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
   strat_ctx ctx = (strat_ctx) calloc (sizeof (*ctx), 1);

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

   unit_types_load (ctx);

   map_init (ctx, &ctx->map, "grass");

   /*camera_center (ctx, (ctx->map.width * ctx->map.tile_width) / 2,
                       (ctx->map.height * ctx->map.tile_height) / 2);*/

   camera_center (ctx, 0, 0);
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

   list_each_elem (ctx->units, unit)
   {
      unit_draw (ctx, unit);
   }

   char status[128];
   sprintf (status, "Camera: %f, %f", ctx->camera.x, ctx->camera.y);
   text_draw (&ctx->ui_font, 0, 40, 0, 0, status, 0);

   vec2f m = screenspace_to_mapspace(ctx, ctx->cursor.x, ctx->cursor.y);
   sprintf (status, "Mouse: %f, %f", m.x, m.y);
	
   text_draw (&ctx->ui_font, 0, 0, 0, 0, status, 0);

   if (ctx->selection.start.x != 0)
   {
      glTranslatef (0.5f - ctx->camera.x, 0.5f - ctx->camera.y, 0);
      glDisable (GL_TEXTURE_2D);

      glColor4f (0.8f, 0.8f, 0.8f, 0.8f);

      int x0 = ctx->selection.start.x,
          y0 = ctx->selection.start.y,
          x1 = ctx->selection.end.x,
          y1 = ctx->selection.end.y;

      for (int i = 0; i < 2; ++ i)
      {
         GLfloat vertices [] =
         {
            x0,  y0,
            x1,  y0,
            x1,  y1,
            x0,  y1
         };

         glVertexPointer (2, GL_FLOAT, 0, vertices);
         glDrawArrays (GL_LINE_LOOP, 0, 4);

         x0 += 2;
         y0 += 2; 
           
         x1 -= 2;
         y1 -= 2;
      }

      glTranslatef (-0.5f + ctx->camera.x, -0.5f + ctx->camera.y, 0);
      glEnable (GL_TEXTURE_2D);
   }

   GLenum error = glGetError();

   if (error != GL_NO_ERROR)
   {
      fprintf (stderr, "OpenGL error: %d\n", error);
   }
}

bool strat_tick (strat_ctx ctx)
{
   if (key_down (key_left_mouse))
   {
      if (ctx->selection.start.x == 0)
      {
         ctx->selection.start.x = ctx->cursor.x + ctx->camera.x;
         ctx->selection.start.y = ctx->cursor.y + ctx->camera.y;
      }

      ctx->selection.end.x = ctx->cursor.x + ctx->camera.x;
      ctx->selection.end.y = ctx->cursor.y + ctx->camera.y;
   }
   else
   {
      if (ctx->selection.start.x)
      {
         ctx->selection.start.x -= ctx->camera.x;
         ctx->selection.start.y -= ctx->camera.y;

         ctx->selection.end.x -= ctx->camera.x;
         ctx->selection.end.y -= ctx->camera.y;

         vec2f start = screenspace_to_mapspace
            (ctx, ctx->selection.start.x, ctx->selection.start.y);
         
         vec2f end = screenspace_to_mapspace
            (ctx, ctx->selection.end.x, ctx->selection.end.y);

         trace ("Selection: %f, %f to %f, %f", start.x, start.y, end.x, end.y);

         list_each_elem (ctx->units, unit)
         {
            if (unit->x > start.x && unit->y > start.y &&
                    unit->x < end.x && unit->y < end.y)
            {
               trace ("%s selected", unit->type->name);

               unit->selected = true;
            }
            else
            {
               if (unit->selected)
               {
                  trace ("%s deselected", unit->type->name);
                  unit->selected = false;
               }
            }
         }

         ctx->selection.start.x = 0;
      }
   }

   camera_tick (ctx);

   return true;
}


