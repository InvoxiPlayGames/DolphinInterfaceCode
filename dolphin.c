/*
    dolphin.c by Emma / InvoxiPlayGames
    Free to use anywhere
*/
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <rvl/ipc.h>
#include "dolphin.h"

typedef enum _dolphin_ioctlv {
  IOCTL_DOLPHIN_GET_ELAPSED_TIME = 0x01,
  IOCTL_DOLPHIN_GET_VERSION = 0x02,
  IOCTL_DOLPHIN_GET_SPEED_LIMIT = 0x03,
  IOCTL_DOLPHIN_SET_SPEED_LIMIT = 0x04,
  IOCTL_DOLPHIN_GET_CPU_SPEED = 0x05,
  IOCTL_DOLPHIN_GET_REAL_PRODUCTCODE = 0x06,
  IOCTL_DOLPHIN_DISCORD_SET_CLIENT = 0x07,
  IOCTL_DOLPHIN_DISCORD_SET_PRESENCE = 0x08,
  IOCTL_DOLPHIN_DISCORD_RESET = 0x09,
  IOCTL_DOLPHIN_GET_SYSTEM_TIME = 0x0A,
} dolphin_ioctlv;

static int __dolphinFd = -1;

static int64_t alwaysZero = 0;

int Dolphin_Open() {
    __dolphinFd = IOS_Open("/dev/dolphin", IPC_OPEN_NONE);
    if (__dolphinFd < 0)
        return __dolphinFd; // return code
    else
        return IPC_OK;
}

int Dolphin_GetVersion(char *outputVersion, int outputLength) {
    if (__dolphinFd < 0)
        return -1;

    ioctlv vec = {
        .data = outputVersion,
        .len = outputLength
    };
    int r = IOS_Ioctlv(__dolphinFd, IOCTL_DOLPHIN_GET_VERSION, 0, 1, &vec);
    return r;
}

int Dolphin_InitialiseDiscord(const char *clientId) {
    if (__dolphinFd < 0)
        return -1;

    ioctlv vec = {
        .data = (char *)clientId,
        .len = strlen(clientId) + 1
    };
    int r = IOS_Ioctlv(__dolphinFd, IOCTL_DOLPHIN_DISCORD_SET_CLIENT, 1, 0, &vec);
    return r;
}

int Dolphin_SetDiscordPresence(DiscordPresenceStruct_t *presence) {
	ioctlv statusVector[10] = { 0 };

    if (__dolphinFd < 0)
        return -1;

    if (presence->details != NULL) {
	    statusVector[0].data = presence->details;
	    statusVector[0].len = strlen(presence->details) + 1;
    }

    if (presence->state != NULL) {
	    statusVector[1].data = presence->state;
	    statusVector[1].len = strlen(presence->state) + 1;
    }

    if (presence->large_image_key != NULL) {
	    statusVector[2].data = presence->large_image_key;
	    statusVector[2].len = strlen(presence->large_image_key) + 1;
    }

    if (presence->large_image_text != NULL) {
	    statusVector[3].data = presence->large_image_text;
	    statusVector[3].len = strlen(presence->large_image_text) + 1;
    }

    if (presence->small_image_key != NULL) {
	    statusVector[4].data = presence->small_image_key;
	    statusVector[4].len = strlen(presence->small_image_key) + 1;
    }

    if (presence->small_image_text != NULL) {
	    statusVector[5].data = presence->small_image_text;
	    statusVector[5].len = strlen(presence->small_image_text) + 1;
    }

    if (presence->start_timestamp != 0) {
	    statusVector[6].data = &presence->start_timestamp;
    } else {
	    statusVector[6].data = &alwaysZero;
    }
	statusVector[6].len = sizeof(int64_t);

    if (presence->end_timestamp != 0) {
	    statusVector[7].data = &presence->end_timestamp;
    } else {
        statusVector[7].data = &alwaysZero;
    }
    statusVector[7].len = sizeof(int64_t);

    if (presence->party_size != 0) {
	    statusVector[8].data = &presence->party_size;
    } else {
        statusVector[8].data = &alwaysZero;
    }
	statusVector[8].len = sizeof(int);

    if (presence->party_max != 0) {
	    statusVector[9].data = &presence->party_max;
    } else {
        statusVector[9].data = &alwaysZero;
    }
	statusVector[9].len = sizeof(int);

    int r = IOS_Ioctlv(__dolphinFd, IOCTL_DOLPHIN_DISCORD_SET_PRESENCE, 10, 0, statusVector);
    return r;
}

int Dolphin_ResetDiscord() {
    if (__dolphinFd < 0)
        return -1;

    ioctlv vec;
    int r = IOS_Ioctlv(__dolphinFd, IOCTL_DOLPHIN_DISCORD_RESET, 0, 0, &vec);
    return r;
}

int64_t Dolphin_GetUnixTimestamp() {
    if (__dolphinFd < 0)
        return -1;

    int64_t ret;
    ioctlv vec = {
        .data = &ret,
        .len = sizeof(int64_t)
    };
    int r = IOS_Ioctlv(__dolphinFd, IOCTL_DOLPHIN_GET_SYSTEM_TIME, 0, 1, &vec);
    if (r == IPC_OK)
        return ret;
    else
        return r;
}
