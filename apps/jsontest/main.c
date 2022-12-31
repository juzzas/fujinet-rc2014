/**
 * jsontest.c
 *
 * A test program showing how to use the JSON parsing functions to traverse
 * live data from e.g. HTTPS, for the Atari 8-bit.
 *
 * Bill Kendrick <bill@newbreedsoftware.com>
 * Thomas Cherryhomes <thom dot cherryhomes at gmail dot com>
 *
 * License: v. 3, see LICENSE.md for details
 *
 * 2022-05-25 - 2022-05-27
 */

#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "fujinet_network.h"

// #define ABORT_ON_ERROR
//const char *url = "N:HTTPS://oldbytes.space/api/v1/timelines/public?limit=1";
const char *url = "N:HTTP://api.open-notify.org/iss-now.json";

/*
[
  {
    "id": "108361296757279278",
    "created_at": "2022-05-25T07:00:06.000Z",
    "in_reply_to_id": null,
    "in_reply_to_account_id": null,
    "sensitive": false,
    "spoiler_text": "",
    "visibility": "public",
    "language": "en",
    "uri": "https://botsin.space/users/osxthemes/statuses/108361286061805267",
    "url": "https://botsin.space/@osxthemes/108361286061805267",
    "replies_count": 0,
    "reblogs_count": 0,
    "favourites_count": 0,
    "edited_at": null,
    "local_only": null,
    "content": "<p>Floppies! - Robert Davis</p>",
    "reblog": null,
    "account": {
      "id": "23439",
      "username": "osxthemes",
      "acct": "osxthemes@botsin.space",
      "display_name": "Macintosh Themes",
      "locked": false,
      "bot": true,
      "discoverable": true,
      "group": false,
      "created_at": "2018-03-28T00:00:00.000Z",
      "note": "<p>I tweet Mac OSX (pre-10.5) and Kaleidoscope (Classic) themes. Bot by <span class=\"h-card\"><a href=\"https://octodon.social/@Eramdam\" class=\"u-url mention\" rel=\"nofollow noopener noreferrer\" target=\"_blank\">@<span>Eramdam</span></a></span>, inspired by kaleidoscopemac@twitter.com. Also on  Twitter at <a href=\"https://twitter.com/osxthemes\" rel=\"nofollow noopener noreferrer\" target=\"_blank\"><span class=\"invisible\">https://</span><span class=\"\">twitter.com/osxthemes</span><span class=\"invisible\"></span></a></p>",
      "url": "https://botsin.space/@osxthemes",
      "avatar": "https://assets.oldbytes.space/assets.oldbytes.space/accounts/avatars/000/023/439/original/322ac0c621b55624.png",
      "avatar_static": "https://assets.oldbytes.space/assets.oldbytes.space/accounts/avatars/000/023/439/original/322ac0c621b55624.png",
      "header": "https://assets.oldbytes.space/assets.oldbytes.space/cache/accounts/headers/000/023/439/original/ea0e0cd513b5a9f7.png",
      "header_static": "https://assets.oldbytes.space/assets.oldbytes.space/cache/accounts/headers/000/023/439/original/ea0e0cd513b5a9f7.png",
      "followers_count": 157,
      "following_count": 1,
      "statuses_count": 17615,
      "last_status_at": "2022-05-25",
      "emojis": [],
      "fields": []
    },
    "media_attachments": [
      {
        "id": "108361296738754794",
        "type": "image",
        "url": "https://assets.oldbytes.space/assets.oldbytes.space/cache/media_attachments/files/108/361/296/738/754/794/original/5785ab0a51d0db1f.gif",
        "preview_url": "https://assets.oldbytes.space/assets.oldbytes.space/cache/media_attachments/files/108/361/296/738/754/794/small/5785ab0a51d0db1f.png",
        "remote_url": "https://files.botsin.space/media_attachments/files/108/361/285/793/211/606/original/7fe52f343cf0c99a.gif",
        "preview_remote_url": null,
        "text_url": null,
        "meta": {
          "original": {
            "width": 213,
            "height": 181,
            "size": "213x181",
            "aspect": 1.1767955801104972
          },
          "small": {
            "width": 213,
            "height": 181,
            "size": "213x181",
            "aspect": 1.1767955801104972
          }
        },
        "description": "Floppies! - Robert Davis",
        "blurhash": "UbLNcMO@QkAAx{jJX4V@8yX9xYX7D@kXoZkV"
      }
    ],
    "mentions": [],
    "tags": [],
    "emojis": [],
    "card": null,
    "poll": null
  }
]
*/


void dbg_abort(void) {
#ifdef ABORT_ON_ERROR
  printf("ABORTING\n");
    exit(1);
#endif
}

void success_or_fail(FUJINET_RC err) {
  if (err != FUJINET_RC_OK) {
    printf("Error = %d\n", err);
    dbg_abort();
  } else {
    printf("Success!\n");
  }
}

#if 0
#define NUM_ELEMENTS 3
const char * elements[NUM_ELEMENTS] = {
  "/0/account/display_name",
  "/0/created_at",
  "/0/content",
};
#endif

#if 1
#define NUM_ELEMENTS 3
const char * elements[NUM_ELEMENTS] = {
    "/iss_position/latitude",
    "/iss_position/longitude",
    "/timestamp",
};
#endif

char query[256];
char buf[256];
struct network_status status;

void main(void) {
  int data_len;
  FUJINET_RC rc;

  printf("JSON Test\n\n");


  /* Open the JSON file over the network */

  printf("Opening %s...", url);

  rc = fujinet_network_open(url, 0);
  success_or_fail(rc);

  /* Switch to JSON mode */

  printf("Setting channel mode to JSON...");

  rc = fujinet_channel_mode(url, CHANNELMODE_JSON);
  success_or_fail(rc);


  /* Parse the JSON */ 

  printf("Parsing JSON...");

  rc = fujinet_json_parse(url);
  success_or_fail(rc);


  /* Read the elements */

  printf("Reading elements...\n\n");
  for (int i = 0; i < NUM_ELEMENTS; i++) {
    printf("Querying \"%s\"...", elements[i]);

    sprintf(query, "N1:%s", elements[i]);
    rc = fujinet_json_query(url, (uint8_t*)query, 256);
    success_or_fail(rc);

    if (rc == FUJINET_RC_OK) {
      printf("Requesting status...");
      rc = fujinet_network_status(url, &status);
      success_or_fail(rc);

      if (rc == FUJINET_RC_OK) {
          data_len = (status.bytes_waiting_high << 8) + status.bytes_waiting_low;

        if (data_len != 0) {
          printf("Reading %d bytes of data...", data_len);

          buf[0] = '\0';
          rc = fujinet_network_read(url, buf, data_len);
          success_or_fail(rc);

          if (rc == FUJINET_RC_OK) {
            printf("FETCHED: %s\n\n", buf);
          }
        } else {
          printf("NOT FOUND\n\n");
        }
      }
    }
  }
  fujinet_network_close(url);
}

