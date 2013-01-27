
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

bool sut_load_file (const char * filename, char ** buffer, size_t * out_size)
{
   FILE * file = fopen (filename, "rb");

   if (!file)
      return false;

   size_t size;

   fseek (file, 0, SEEK_END);
   size = ftell (file);
   fseek (file, 0, SEEK_SET);

   if (! (*buffer = malloc (size + 1)))
   {
      fclose (file);
      return false;
   }

   if (fread (*buffer, 1, size, file) != size)
   {
      fclose (file);
      free (*buffer);

      return false;
   }

   fclose (file);

   (*buffer) [size] = 0;

   if (out_size)
      *out_size = size;

   return true;
}

json_value * sut_json_load (const char * filename)
{
   char * json;

   if (!sut_load_file (filename, &json, 0))
   {
      fprintf (stderr, "Error loading JSON file: %s\n", filename);
      return 0;
   }

   json_settings settings = {};
   char json_error [128];

   json_value * value;

   if (! (value = json_parse_ex (&settings, json, json_error)))
   {
      fprintf (stderr, "Error parsing JSON: %s\n", json_error);
      free (json);

      return 0;
   }

   free (json);

   return value;
}

bool load_config (strat_ctx ctx)
{
   char * config_json;

   if (!sut_load_file ("config.json", &config_json, 0))
   {
      fprintf (stderr, "Error loading configuration file\n");
      return false;
   }

   json_settings settings = {};
   char json_error [128];

   if (! (ctx->config = json_parse_ex (&settings, config_json, json_error)))
   {
      fprintf (stderr, "Error parsing configuration file: %s\n", json_error);
      free (config_json);

      return false;
   }

   return true;
}

json_value * sut_json_value (json_value * value, const char * name)
{
   if (value->type != json_object)
      return 0;

   for (int i = 0; i < value->u.object.length; ++ i)
      if (!strcasecmp (value->u.object.values [i].name, name))
         return value->u.object.values [i].value;

   return 0;
}

long sut_json_int (json_value * value, const char * name, int def)
{
   value = sut_json_value (value, name);

   if ( (!value) || value->type != json_integer)
      return def;

   return value->u.integer;
}

long sut_json_bool (json_value * value, const char * name, bool def)
{
   value = sut_json_value (value, name);

   if ( (!value) || value->type != json_boolean)
      return def;

   return value->u.boolean;
}

const char * sut_json_string (json_value * value,
                              const char * name,
                              const char * def)
{
   value = sut_json_value (value, name);

   if ( (!value) || value->type != json_string)
      return def;

   return value->u.string.ptr;
}

char * sut_format (const char * format, ...)
{

   return strdup (format);
}

