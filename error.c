/*=======================================================================
solunar
error.c
Definition of Error object
(c)2005-2012 Kevin Boone
=======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

typedef struct _ErrorPriv
  {
  char *str;
  } ErrorPriv;

/*=======================================================================
Error_free
=======================================================================*/
void Error_free (Error *self)
  {
  if (!self) return;
  if (self->priv) 
    {
    if (self->priv->str) free (self->priv->str);
    free (self->priv);
    }
  free (self);
  }

/*=======================================================================
Error_new
=======================================================================*/
Error *Error_new (const char *message)
  {
  Error *self = (Error *) malloc (sizeof (Error));
  self->priv = (ErrorPriv *) malloc (sizeof (ErrorPriv));
  self->priv->str = strdup (message);
  return self;
  }

/*=======================================================================
Error_get_message
=======================================================================*/
const char *Error_get_message (Error *self)
  {
  if (!self) return "";
  if (!self->priv) return "";
  if (!self->priv->str) return "";
  return self->priv->str;
  }

