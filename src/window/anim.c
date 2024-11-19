
#include "window.h"

void update_animation(AnimationController *anim_ctrl)
{
    anim_ctrl->frame_delay_counter++;
    if (anim_ctrl->frame_delay_counter < anim_ctrl->frame_delay)
    {
        return;
    }

    anim_ctrl->frame_delay_counter = 0;

    if (anim_ctrl->is_walking)
    {
        anim_ctrl->frame = anim_ctrl->max_frames + (anim_ctrl->frame + 1) % 3; // Walking frames
        return;
    }

    // Idle animation: 0-0-0-0
    anim_ctrl->frame = 0;
}