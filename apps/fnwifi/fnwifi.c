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
      printf("WiFi status (%d): ", status_wifi);

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

FUJINET_RC do_wifi_scan(void)
{
    uint8_t wifi_count;
    uint8_t i;

    FUJINET_RC rc = fujinet_scan_for_networks(&wifi_count);
    if (rc == FUJINET_RC_OK) {
        printf("Found %d WiFi networks\n", wifi_count);

        for (i = 0; i < wifi_count; i++) {
            SSIDInfo info;

            rc = fujinet_get_scan_result(i, &info);
            if (rc == FUJINET_RC_OK) {
                printf("SSID: [%ddB] %s\n", info.rssi, info.ssid);
            }
        }
    }

    return rc;
}

int main(int argc, char **argv)
{
  printf("FujiNet module - WiFi\n");

  fujinet_init();

  FUJINET_RC rc = FUJINET_RC_INVALID;

  if (argc > 1) {
      if (strcmp(argv[1], "/SCAN") == 0) {
          rc = do_wifi_scan();
      }
  } else {
      rc = do_wifi_status();
  }
  switch (rc) {
    case FUJINET_RC_OK:
        printf("done\n");
        break;

      case FUJINET_RC_TIMEOUT:
          printf("Timed out!\n");
          break;

    case FUJINET_RC_INVALID:
        printf("Invalid parameter!\n");
        break;

    default:
        printf("Unexpected error (%d)\n", rc);
    }

    return 0;
}
