
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

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef _MSC_VER
    #include "../deps/glew/include/GL/glew.h"
    #include "../msvc/typeof.h"
    #include "../msvc/dirent.h"
    #define snprintf sprintf_s
    #define strcasecmp stricmp
#else
    #include <dirent.h>
    #include <stdbool.h>
    #include <opengl/gl.h>
#endif

#include "../deps/libpng/png.h"

#include "../deps/json-parser/json.h"
#include "../deps/uthash/uthash.h"
#include "../deps/list/list.h"

#include "../deps/freetype-gl/vertex-buffer.h"
#include "../deps/freetype-gl/texture-font.h"

typedef struct _strat_ctx * strat_ctx;

#include "util.h"
#include "image.h"
#include "tile.h"
#include "font.h"
#include "unit-type.h"
#include "unit.h"
#include "map.h"
#include "camera.h"

#define strat_version "Strat 0.1.0"
#define strat_max_path 512

struct _strat_ctx
{
   json_value * game_def, * config;

   long win_width, win_height;
   bool fullscreen;

   point cursor;
   point camera;

   float camera_accel_x, camera_accel_y;

   const char * game_title;
   int tick_rate;

   struct _strat_map map;

   struct _strat_tile empty_tile;

   struct _strat_font ui_font;

   unit_type unit_types;
   list (struct _unit, units);

   struct
   {
      point start, end;

   } selection;
};

strat_ctx strat_init (int argc, char * argv []);
void strat_free (strat_ctx);

bool strat_tick (strat_ctx);

void strat_init_gfx (strat_ctx ctx);
void strat_draw (strat_ctx);

#define trace(...) do { \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
} while(0);

#define key_left_arrow   1
#define key_right_arrow  2
#define key_up_arrow     3
#define key_down_arrow   4
#define key_left_mouse   5
#define key_right_mouse  6

bool key_down (int key);


