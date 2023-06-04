//
// Created by jskists on 16/11/2022.
//

#ifndef FUJINET_RC2014_FUJINET_PRINTER_H
#define FUJINET_RC2014_FUJINET_PRINTER_H

#define MAX_PRINTER_UNIT  3

FUJINET_RC fujinet_printer_write(uint8_t printer_unit, uint8_t* buf, uint16_t len);

FUJINET_RC fujinet_printer_stream(uint8_t printer_unit);


#endif //FUJINET_RC2014_FUJINET_PRINTER_H
