#ifndef _GBT_PLAYER_
#define _GBT_PLAYER_

#include <gb/gb.h>

// Plays the song pointed by data (pointer array to patterns) in given bank at
// given initial speed.
void gbt_play(void *data, UINT8 bank, UINT8 speed);

// Pauses or unpauses music.
void gbt_pause(UINT8 pause);

// Stops music and turns off sound system. Called automatically when the last
// pattern ends and autoloop isn't activated.
void gbt_stop(void);

// Enables or disables autoloop
void gbt_loop(UINT8 loop);

// Updates player, should be called every frame.
// NOTE: This will change the active ROM bank to 1.
void gbt_update(void);

// Set enabled channels to prevent the player from using that channel.
void gbt_enable_channels(UINT8 channel_flags);

#define GBT_CHAN_1 (1<<0)
#define GBT_CHAN_2 (1<<1)
#define GBT_CHAN_3 (1<<2)
#define GBT_CHAN_4 (1<<3)

#endif //_GBT_PLAYER_