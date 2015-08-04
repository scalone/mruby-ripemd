#ifndef RMDsize
#define RMDsize 160
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mruby.h"
#include "mruby/string.h"
#include "mruby/value.h"

#if RMDsize == 128
#include "rmd128.h"
#elif RMDsize == 160
#include "rmd160.h"
#endif

#if MRUBY_RELEASE_NO < 10000
#include "error.h"
#else
#include "mruby/error.h"
#endif

byte *RMD(byte *message)
/*
 * returns RMD(message)
 * message should be a string terminated by '\0'
 */
{
   dword         MDbuf[RMDsize/32];   /* contains (A, B, C, D(, E))   */
   static byte   hashcode[RMDsize/8]; /* for final hash-value         */
   dword         X[16];               /* current 16-word chunk        */
   dword         i;                   /* counter                      */
   dword         length;              /* length in bytes of message   */
   dword         nbytes;              /* # of bytes not yet processed */

   /* initialize */
   MDinit(MDbuf);
   length = (dword)strlen((char *)message);

   /* process message in 16-word chunks */
   for (nbytes=length; nbytes > 63; nbytes-=64) {
      for (i=0; i<16; i++) {
         X[i] = BYTES_TO_DWORD(message);
         message += 4;
      }
      compress(MDbuf, X);
   }                                    /* length mod 64 bytes left */

   /* finish: */
   MDfinish(MDbuf, message, length, 0);

   for (i=0; i<RMDsize/8; i+=4) {
      hashcode[i]   =  MDbuf[i>>2];         /* implicit cast to byte  */
      hashcode[i+1] = (MDbuf[i>>2] >>  8);  /*  extracts the 8 least  */
      hashcode[i+2] = (MDbuf[i>>2] >> 16);  /*  significant bits.     */
      hashcode[i+3] = (MDbuf[i>>2] >> 24);
   }

   return (byte *)hashcode;
}

/********************************************************************/

mrb_value
mrb_rmd160_s__hexdigest(mrb_state *mrb, mrb_value self)
{
   byte *hashcode;
   char *content;
   mrb_value content_mrb, result_mrb;
   char result[100];
   char *hexcode = result;
   int i;

   mrb_get_args(mrb, "S", &content_mrb);

   content = mrb_str_to_cstr(mrb, content_mrb);
   hashcode = RMD((byte *)content);

   for (i=0; i<RMDsize/8; i++) {
      hexcode += sprintf(hexcode, "%02x", hashcode[i]);
   }

   result_mrb = mrb_str_new(mrb, result, (size_t)strlen(result));

   return result_mrb;
}

mrb_value
mrb_rmd160_s__digest(mrb_state *mrb, mrb_value self)
{
   byte *hashcode;
   char *content;
   mrb_value content_mrb;
   char result[100];
   char *hexcode = result;
   int i;

   mrb_get_args(mrb, "S", &content_mrb);

   content = mrb_str_to_cstr(mrb, content_mrb);
   hashcode = RMD((byte *)content);

   for (i=0; i<RMDsize/8; i++) {
      hexcode += sprintf(hexcode, "%02x", hashcode[i]);
   }

   return mrb_str_new(mrb, (char *)hashcode, 20);
}

void
mrb_mruby_ripemd_gem_init(mrb_state* mrb)
{
  struct RClass *rmd160;

  rmd160 = mrb_define_class(mrb, "RMD160", mrb->object_class);
  mrb_define_class_method(mrb , rmd160, "hexdigest", mrb_rmd160_s__hexdigest, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb , rmd160, "digest", mrb_rmd160_s__digest, MRB_ARGS_REQ(1));
}

void
mrb_mruby_ripemd_gem_final(mrb_state* mrb)
{
}
