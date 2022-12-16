/**
 * FujiNet Wifi tool
 */

#include <conio.h>
#include <string.h>

#include "fujinet.h"
#include "fujinet_device.h"

NetConfig nc;
AdapterConfig adapter;

FUJINET_RC do_wifi_status(void)
{
  uint8_t status_wifi;

  FUJINET_RC rc = fujinet_get_wifi_status(&status_wifi);
  if (rc == FUJINET_RC_OK) {
      cprintf("WiFi status (%d): ", status_wifi);

      switch (status_wifi) {
      case 0:
          cprintf("WIFI is idle\n");
          break;

      case 1:
          cprintf("No SSID Available\n");
          break;

      case 2:
          cprintf("Scan complete\n");
          break;

      case 3:
      {
          cprintf("Connected to network, and active\n");
          rc = fujinet_get_adapter_config(&adapter);
          if (rc == FUJINET_RC_OK) {
              cputs("ssid: ");
              cputs(adapter.ssid);
              cputs("\n");
              cprintf("ip: %d.%d.%d.%d\n", adapter.localIP[0], adapter.localIP[1], adapter.localIP[2], adapter.localIP[3]);
              cprintf("netmask: %d.%d.%d.%d\n", adapter.netmask[0], adapter.netmask[1], adapter.netmask[2], adapter.netmask[3]);
              cprintf("gateway: %d.%d.%d.%d\n", adapter.gateway[0], adapter.gateway[1], adapter.gateway[2], adapter.gateway[3]);
          }
          break;
      }

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

char password[80];
FUJINET_RC do_wifi_set_ssid(char *ssid)
{
    uint8_t wifi_count;
    uint8_t i;

    printf("password: ");
    fgets_cons(password, 80);

    FUJINET_RC rc = fujinet_scan_for_networks(&wifi_count);
    if (rc == FUJINET_RC_OK) {
        cprintf("Found %d WiFi networks\n", wifi_count);

        for (i = 0; i < wifi_count; i++) {
            SSIDInfo info;

            rc = fujinet_get_scan_result(i, &info);
            if (rc == FUJINET_RC_OK) {
                cprintf("SSID: [%ddBm] %s\n", info.rssi, info.ssid);
            }
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
        cprintf("Found %d WiFi networks\n", wifi_count);

        for (i = 0; i < wifi_count; i++) {
            SSIDInfo info;

            rc = fujinet_get_scan_result(i, &info);
            if (rc == FUJINET_RC_OK) {
                cprintf("SSID: [%ddBm] %s\n", info.rssi, info.ssid);
            }
        }
    }

    return rc;
}

int main(int argc, char **argv)
{
  cprintf("FujiNet module - WiFi\n");

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
        cprintf("done\n");
        break;

      case FUJINET_RC_TIMEOUT:
          cprintf("Timed out!\n");
          break;

    case FUJINET_RC_INVALID:
        cprintf("Invalid parameter!\n");
        break;

    default:
        printf("Unexpected error (%d)\n", rc);
    }

    return 0;
}
