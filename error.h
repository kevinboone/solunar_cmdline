/*=======================================================================
solunar
error.h
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

typedef struct _Error
  {
  struct _ErrorPriv *priv;
  } Error; 

void Error_free (Error *self);
Error *Error_new (const char *message);
const char *Error_get_message (Error *self);

