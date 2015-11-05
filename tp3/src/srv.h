#ifndef __srv_h__
#define __srv_h__

#include "tp3.h"
#include <stdbool.h>

#define max(a, b) (a > b ? a : b)

#define TAG_REQUEST 60
#define TAG_REPLY 70

void servidor(int mi_cliente);

#endif
