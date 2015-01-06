/*=======================================================================
solunar
pointerlist.h
(c)2005-2012 Kevin Boone
=======================================================================*/
#pragma once

#include "defs.h"

struct _PointerListPriv;

typedef struct _PointerList
{
  void *pointer;
  struct _PointerList *next;
} PointerList;

PointerList *PointerList_append (PointerList *self, void *pointer);
int PointerList_get_length (const PointerList *self);
const void *PointerList_get_const_pointer (const PointerList *self, int index);
void *PointerList_get_pointer (PointerList *self, int index);
void PointerList_free (PointerList *self, BOOL free_contents);

