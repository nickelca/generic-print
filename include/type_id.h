#ifndef TYPE_ID_H
#define TYPE_ID_H

#include <stddef.h>
#include <stdint.h>
#include <uchar.h>
#include <wchar.h>

#include "type_id_common.h"

#define TYPE_ID_INTEGRALS_DEFAULT(x) TYPE_ID_POINTERS(x)
#define TYPE_ID_POINTERS_DEFAULT(x) TYPE_ID_ARRAYS(x)
#define TYPE_ID_ARRAYS_DEFAULT(x) TIDI(Invalid)

#include "type_id_arrays.h"
#include "type_id_pointers.h"
#include "type_id_integrals.h"

#define TYPE_ID(x) TYPE_ID_INTEGRALS(x)

#endif
