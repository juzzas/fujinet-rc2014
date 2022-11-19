//
// Created by jskists on 16/11/2022.
//


FUJINET_RC fujinet_modem_stream(uint8_t modem_id)
{
  struct fujinet_dcb dcb = {};

  if (modem_id > MAX_MODEM_ID)
    return FUJINET_RC_INVALID;

  dcb.device = 0x50 + modem_id;
  dcb.command = 'X';
  dcb.timeout = 15;

  return fujinet_dcb_exec(&dcb);
}
