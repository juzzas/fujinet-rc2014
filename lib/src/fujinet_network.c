//
// Created by jskists on 16/11/2022.
//
#include <stddef.h>

#define TIMEOUT 0x1f /* approx 30 seconds */

unsigned char nunit(char* devicespec)
{
  unsigned char unit=1;

  // Set unit to 1 unless explicitly specified.
  if (devicespec[1]==':')
    unit=1;
  else if (devicespec[2]==':')
    unit=devicespec[1]-0x30; // convert from alpha to integer.
  else
    unit=1;

  return unit;
}

FUJINET_RC fujinet_network_open(char* devicespec, unsigned char trans)
{
  unsigned char unit=nunit(devicespec);
  struct fujinet_dcb dcb = {};

  dcb.device    = 0x70 + unit;      // Fuji Device Identifier
  dcb.command   = 'O';        // Open
  dcb.buffer    = devicespec; // eg: N:TCP//
  dcb.response_bytes     = 256;        // max size of our device spec
  dcb.timeout   = TIMEOUT;    // approximately 30 second timeout
  dcb.daux1     = OUPDATE;    // Read and write
  dcb.daux2     = trans;      // CR/LF translation

  return fujinet_dcb_exec(&dcb);
}

FUJINET_RC fujinet_network_close(char* devicespec)
{
  unsigned char unit=nunit(devicespec);
  struct fujinet_dcb dcb = {};

  dcb.device    = 0x70 + unit;      // Fuji Device Identifier
  dcb.command   = 'C';        // Close
  dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

  return fujinet_dcb_exec(&dcb);
}

unsigned char fujinet_network_status(char* devicespec, struct network_status *status)
{
  unsigned char unit=nunit(devicespec);
  struct fujinet_dcb dcb = {};

  dcb.device    = 0x70 + unit;      // Fuji Device Identifier
  dcb.command   = 'S';        // Status
  dcb.response  = status;
  dcb.response_bytes     = sizeof(struct network_status);
  dcb.timeout   = TIMEOUT;    // approximately 30 second timeout

  return fujinet_dcb_exec(&dcb);
}

unsigned char fujinet_network_read(char* devicespec, unsigned char* buf, unsigned short len)
{
  unsigned char unit=nunit(devicespec);

  OS.dcb.ddevic   = DFUJI;
  OS.dcb.dunit    = unit;
  OS.dcb.dcomnd   = 'R';    // read
  OS.dcb.dstats   = DREAD;
  OS.dcb.dbuf     = buf;
  OS.dcb.dtimlo   = TIMEOUT;
  OS.dcb.dbyt     = OS.dcb.daux = len; // Set the buffer size AND daux with length
  siov();

  if (OS.dcb.dstats!=SUCCESS)
  {
    // something went wrong
    // do we need to return extended status?
    if (OS.dcb.dstats==DERROR)
    {
      nstatus(devicespec);
      return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return extended error.
    }
  }
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char fujinet_network_write(char* devicespec, unsigned char* buf, unsigned short len)
{
  unsigned char unit=nunit(devicespec);

  OS.dcb.ddevic   = DFUJI;
  OS.dcb.dunit    = unit;
  OS.dcb.dcomnd   = 'W';    // write
  OS.dcb.dstats   = DWRITE;
  OS.dcb.dbuf     = buf;
  OS.dcb.dtimlo   = TIMEOUT;
  OS.dcb.dbyt     = OS.dcb.daux = len;
  siov();

  if (OS.dcb.dstats!=SUCCESS)
  {
    // something went wrong
    // do we need to return extended status?
    if (OS.dcb.dstats==DERROR)
    {
      nstatus(devicespec);
      return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return extended error.
    }
  }
  return OS.dcb.dstats; // Return SIO error or success.
}

unsigned char fujinet_network_login(char* devicespec, char *login, char *password)
{
  unsigned char unit=nunit(devicespec);

  OS.dcb.ddevic=0x71;
  OS.dcb.dunit=unit;
  OS.dcb.dcomnd=0xFD;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=login;
  OS.dcb.dtimlo=0x1f;
  OS.dcb.dbyt=256;
  OS.dcb.daux=0;
  siov();

  if (OS.dcb.dstats!=1)
  {
    nstatus(devicespec);
    return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return ext err
  }

  OS.dcb.dcomnd=0xFE;
  OS.dcb.dstats=0x80;
  OS.dcb.dbuf=password;
  siov();

  if (OS.dcb.dstats!=1)
  {
    nstatus(devicespec);
    return OS.dvstat[DVSTAT_EXTENDED_ERROR]; // return ext err
  }

  return OS.dcb.dstats;
}