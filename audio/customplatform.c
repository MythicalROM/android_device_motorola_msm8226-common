/*
 * Copyright 2013, The CyanogenMod Project
 *   Shareef Ali
 *   Hashcode
 *   Dhacker29
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Custom-Platform_Api.h"


#include <stdlib.h>
#include <dlfcn.h>
#include <cutils/log.h>
#include "audio_extn.h"
#include <audio_hw.h>
#include <platform_api.h>
#include "platform.h"
#include "voice_extn.h"

snd_device_t custom_platform_get_input_snd_device(void *platform, audio_devices_t out_device)
{
    struct platform_data *my_data = (struct platform_data *)platform;
    struct audio_device *adev = my_data->adev;
    audio_source_t  source = (adev->active_input == NULL) ?
                                AUDIO_SOURCE_DEFAULT : adev->active_input->source;

    audio_mode_t    mode   = adev->mode;
    audio_channel_mask_t channel_mask = (adev->active_input == NULL) ?
                                AUDIO_CHANNEL_IN_MONO : adev->active_input->channel_mask;
    snd_device_t snd_device = -2;
    int channel_count = popcount(channel_mask);

    if ((out_device != AUDIO_DEVICE_NONE) && ((mode == AUDIO_MODE_IN_CALL) ||
        voice_extn_compress_voip_is_active(adev))) {
        if (adev->voice.tty_mode == TTY_MODE_OFF) {
            if (out_device & AUDIO_DEVICE_OUT_EARPIECE ||
                out_device & AUDIO_DEVICE_OUT_WIRED_HEADPHONE) {
                if (!(out_device & AUDIO_DEVICE_OUT_EARPIECE &&
                    audio_extn_should_use_handset_anc(channel_count))) {
                    if (my_data->fluence_type == FLUENCE_DUAL_MIC ||
                        my_data->fluence_in_voice_call == false) {
                        snd_device = SND_DEVICE_IN_VOICE_DMIC;
                    }
                }
            } else if (out_device & AUDIO_DEVICE_OUT_SPEAKER) {
                if (!(my_data->fluence_type != FLUENCE_DUAL_MIC &&
                    my_data->fluence_in_voice_call &&
                    my_data->fluence_in_spkr_mode)) {
                    snd_device = SND_DEVICE_IN_VOICE_SPEAKER_MIC;
                }
            }
        }
    }
    return snd_device;
}
void custom_init_data(){
    change_acdb_data(SND_DEVICE_IN_VOICE_DMIC, 7);
}
