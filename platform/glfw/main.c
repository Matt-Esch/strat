
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
#include <GL/glfw.h>

int flags = 0;

#define flag_window_closed 1

static int on_window_closed ()
{
   flags |= flag_window_closed;
   return GL_TRUE;
}

int main (int argc, char * argv [])
{
   if (!glfwInit ())
   {
      trace ("Error initializing GLFW");
      return 1;
   }

   strat_ctx ctx = strat_init (argc, argv);

   if (!ctx)
      return 1;

   if (!glfwOpenWindow (ctx->win_width, ctx->win_height, 0, 0, 0, 0, 0, 0,
                        ctx->fullscreen ?  GLFW_FULLSCREEN : GLFW_WINDOW))
   {
      trace ("Error opening window");
      return 2;
   }
   
   glfwSetWindowCloseCallback (on_window_closed);

   glfwSetWindowTitle (ctx->game_title);

   strat_init_gfx (ctx);

   for (;;)
   {
      if (flags & flag_window_closed)
         break;

      glfwGetMousePos (&ctx->cursor.x, &ctx->cursor.y);

      if (!strat_tick (ctx))
         break;

      strat_draw (ctx);

      glfwSwapBuffers();
   }

   strat_free (ctx);

   glfwTerminate ();

   return 0;
}

bool key_down (int key)
{
   switch (key)
   {
      case key_left_arrow:
         return glfwGetKey (GLFW_KEY_LEFT) == GLFW_PRESS;

      case key_right_arrow:
         return glfwGetKey (GLFW_KEY_RIGHT) == GLFW_PRESS;

      case key_up_arrow:
         return glfwGetKey (GLFW_KEY_UP) == GLFW_PRESS;

      case key_down_arrow:
         return glfwGetKey (GLFW_KEY_DOWN) == GLFW_PRESS;

      case key_left_mouse:
         return glfwGetMouseButton (GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

      case key_right_mouse:
         return glfwGetMouseButton (GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
   };

   assert (false);
}



