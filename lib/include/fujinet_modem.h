//
// Created by jskists on 16/11/2022.
//
#include <stdint.h>
#include "fujinet.h"
#include "fuji_typedefs.h"

#ifndef FUJINET_RC2014_FUJINET_MODEM_H
#define FUJINET_RC2014_FUJINET_MODEM_H

#define MAX_MODEM_UNIT  3

FUJINET_RC fujinet_modem_stream(uint8_t modem_unit);

#endif //FUJINET_RC2014_FUJINET_MODEM_H
