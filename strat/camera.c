
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

void camera_center (strat_ctx ctx, int x, int y)
{
   ctx->camera_x = x - (ctx->win_width / 2);
   ctx->camera_y = y - (ctx->win_height / 2);
}

/*#define theta  26.565f
#define c  (cos(theta/2))
#define s  (sin(theta/2))

sut_point screenspace_to_mapspace (int x, int y)
{
   sut_point point;

   point.x = round(x / c - (y / s));
   point.y = round(x / c + (y / s));

   return point; 
}*/

point screenspace_to_mapspace (strat_ctx ctx, int x, int y)
{
   point point;
 
   x += ctx->camera_x;
   y += ctx->camera_y;

   y += ctx->map.tile_height / 2;

   point.x = (x - y) / ctx->map.tile_width;
   point.y = (x - 3 * y) / ctx->map.tile_width;

   return point; 
}

point mapspace_to_screenspace (strat_ctx ctx, int x, int y)
{
   point point;

   point.x = 0;
   point.y = 0;

   return point; 
}

void handle_accel (float * accel, int direction)
{
   if (direction > 0)
   {
      if (*accel <= 0)
         *accel = 1.0f;
      else
      {
         *accel += 0.1f;
         *accel *= 1.5f;

         if (*accel > 6.0f)
            *accel = 6.0f;
      }
   }
   else if (direction < 0)
   {
      if (*accel >= 0)
         *accel = -1.0f;
      else
      {
         *accel -= 0.1f;
         *accel *= 1.5f;

         if (*accel < -6.0f)
            *accel = -6.0f;
      }
   }
   else
   {
      if (*accel > 0)
      {
         *accel -= 0.2f;

         if (*accel < 0)
            *accel = 0;
      }
      else if (*accel < 0)
      {
         *accel += 0.2f;

         if (*accel > 0)
            *accel = 0;
      }
   }
}

void camera_tick (strat_ctx ctx)
{
   int x_direction = 0;

   if (key_down (key_left_arrow))
      x_direction = -1;

   if (key_down (key_right_arrow))
      x_direction = 1;

   handle_accel (&ctx->camera_accel_x, x_direction);
   ctx->camera_x += ctx->camera_accel_x;

   int y_direction = 0;

   if (key_down (key_up_arrow))
      y_direction = -1;

   if (key_down (key_down_arrow))
      y_direction = 1;

   handle_accel (&ctx->camera_accel_y, y_direction);
   ctx->camera_y += ctx->camera_accel_y;
}



