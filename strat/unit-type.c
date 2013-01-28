
/* vim: set et ts=3 sw=3 ft=c:
 *
 * Copyright (C) 2013 James McLaughlin & Anders Riggelsen.
 * All rights reserved.
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

bool unit_type_init (strat_ctx ctx, unit_type type, const char * name)
{
   memset (type, 0, sizeof (*type));

   char filename [strat_max_path];
   snprintf (filename, sizeof (filename), "game/unit/%s/unit.json", name);

   json_value * json = sut_json_load (filename);

   if (!json)
       return false;

   type->name = strdup (name);

   trace ("Init unit type: %s", type->name);

   return true;
}

void unit_type_cleanup (unit_type type)
{
   json_value_free (type->json);
   free (type->name);
}

void unit_type_load (unit_type type)
{
   if (type->flags & unit_type_flag_loaded)
      return;

   type->flags |= unit_type_flag_loaded;

   char filename [strat_max_path];

   snprintf (filename, sizeof (filename), "game/unit/%s/stand-0.png", type->name);
   image_init (&type->image.stand, filename);

   type->width = type->image.stand.width;
   type->height = type->image.stand.height;
}

void unit_type_unload (unit_type type)
{
   image_cleanup (&type->image.stand);

   type->flags &= ~ unit_type_flag_loaded;
}

bool unit_types_load (strat_ctx ctx, unit_type * types)
{
   DIR * unit_dir = opendir ("game/unit");

   if (!unit_dir)
   {
      trace ("Couldn't open unit dir");
      return false;
   }

   struct dirent * entry;

   while ((entry = readdir (unit_dir)))
   {
      if (entry->d_type != DT_DIR)
         continue;

      if (*entry->d_name == '.')
         continue;

      unit_type type = (unit_type) malloc (sizeof (*type));

      unit_type_init (ctx, type, entry->d_name);

      HASH_ADD_KEYPTR (hh,
                       *types,
                       type->name,
                       strlen (type->name),
                       type);
   }

   closedir (unit_dir);

   return true;
}

void unit_types_unload (strat_ctx ctx, unit_type * types)
{
   unit_type type, tmp;

   HASH_ITER (hh, (*types), type, tmp)
   {
      HASH_DEL (*types, type);

      unit_type_cleanup (type);
      free (type);
   }
}

