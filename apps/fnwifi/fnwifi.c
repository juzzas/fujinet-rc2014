/**
 * FujiNet Wifi tool
 */

#include <stdio.h>

#include "fujinet.h"
#include "fujinet_device.h"

FUJINET_RC do_wifi_status(void)
{
  uint8_t status_wifi;

  FUJINET_RC rc = fujinet_get_wifi_status(&status_wifi);
  if (rc == FUJINET_RC_OK) {
    switch (status_wifi) {
    case 0:
      printf("WIFI is idle\n");
      break;

    case 1:
      printf("No SSID Available\n");
      break;

    case 2:
      printf("Scan complete\n");
      break;

    case 3:
      printf("Connected to network, and active\n");
      break;

    case 4:
      printf("Last connect failed\n");
      break;

    case 5:
      printf("WiFi Connection Lost\n");
      break;

    case 6:
      printf("WiFi explicitly disconnected\n");
      break;

    default:
      printf("Unexpected WiFi status: %d\n", status_wifi);
      break;
    }
  }

  return rc;
}

int main(void)
{
  printf("FujiNet module - WiFi\n");

  fujinet_init();

  FUJINET_RC rc = do_wifi_status();
  switch (rc) {
    case FUJINET_RC_OK:
        printf("done\n");
        break;

    case FUJINET_RC_TIMEOUT:
        printf("Timed out!\n");
        break;

    default:
        printf("Unexpected error (%d)\n", rc);
    }

    return 0;
}
