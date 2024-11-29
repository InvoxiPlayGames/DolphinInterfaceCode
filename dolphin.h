/*
    dolphin.c by Emma / InvoxiPlayGames
    Free to use anywhere
*/
#include <stdint.h>

typedef struct _DiscordPresenceStruct {
    char *details;
    char *state;
    char *large_image_key;
    char *large_image_text;
    char *small_image_key;
    char *small_image_text;
    int64_t start_timestamp;
    int64_t end_timestamp;
    int party_size;
    int party_max;
} DiscordPresenceStruct_t;

int Dolphin_Open();
int Dolphin_GetVersion(char *outputVersion, int outputLength);
int Dolphin_InitialiseDiscord(const char *clientId);
int Dolphin_SetDiscordPresence(DiscordPresenceStruct_t *presence);
int64_t Dolphin_GetUnixTimestamp();
