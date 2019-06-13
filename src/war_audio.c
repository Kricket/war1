typedef struct
{
    WarAudioId id;
    WarAudioType type;
} WarAudioData;

WarAudioData audioData[] =
{
    { WAR_MUSIC_00,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_01,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_02,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_03,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_04,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_05,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_06,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_07,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_08,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_09,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_10,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_11,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_12,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_13,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_14,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_15,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_16,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_17,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_18,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_19,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_20,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_21,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_22,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_23,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_24,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_25,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_26,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_27,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_28,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_29,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_30,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_31,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_32,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_33,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_34,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_35,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_36,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_37,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_38,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_39,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_40,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_41,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_42,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_43,                     WAR_AUDIO_MIDI },
    { WAR_MUSIC_44,                     WAR_AUDIO_MIDI },
    { WAR_LOGO,                         WAR_AUDIO_WAVE },
    { WAR_INTRO_DOOR,                   WAR_AUDIO_WAVE },
    { WAR_BUILDING,                     WAR_AUDIO_WAVE },
    { WAR_EXPLOSION,                    WAR_AUDIO_WAVE },
    { WAR_CATAPULT_ROCK_FIRED,          WAR_AUDIO_WAVE },
    { WAR_TREE_CHOPPING_1,              WAR_AUDIO_WAVE },
    { WAR_TREE_CHOPPING_2,              WAR_AUDIO_WAVE },
    { WAR_TREE_CHOPPING_3,              WAR_AUDIO_WAVE },
    { WAR_TREE_CHOPPING_4,              WAR_AUDIO_WAVE },
    { WAR_BUILDING_COLLAPSE_1,          WAR_AUDIO_WAVE },
    { WAR_BUILDING_COLLAPSE_2,          WAR_AUDIO_WAVE },
    { WAR_BUILDING_COLLAPSE_3,          WAR_AUDIO_WAVE },
    { WAR_UI_CHIME,                     WAR_AUDIO_WAVE },
    { WAR_UI_CLICK,                     WAR_AUDIO_WAVE },
    { WAR_UI_CANCEL,                    WAR_AUDIO_WAVE },
    { WAR_SWORD_ATTACK_1,               WAR_AUDIO_WAVE },
    { WAR_SWORD_ATTACK_2,               WAR_AUDIO_WAVE },
    { WAR_SWORD_ATTACK_3,               WAR_AUDIO_WAVE },
    { WAR_FIST_ATTACK,                  WAR_AUDIO_WAVE },
    { WAR_CATAPULT_FIRE_EXPLOSION,      WAR_AUDIO_WAVE },
    { WAR_FIREBALL,                     WAR_AUDIO_WAVE },
    { WAR_ARROW_SPEAR,                  WAR_AUDIO_WAVE },
    { WAR_ARROW_SPEAR_HIT,              WAR_AUDIO_WAVE },
    { WAR_ORC_HELP_1,                   WAR_AUDIO_WAVE },
    { WAR_ORC_HELP_2,                   WAR_AUDIO_WAVE },
    { WAR_HUMAN_HELP_2,                 WAR_AUDIO_WAVE },
    { WAR_HUMAN_HELP_1,                 WAR_AUDIO_WAVE },
    { WAR_ORC_DEAD,                     WAR_AUDIO_WAVE },
    { WAR_HUMAN_DEAD,                   WAR_AUDIO_WAVE },
    { WAR_ORC_WORK_COMPLETE,            WAR_AUDIO_WAVE },
    { WAR_HUMAN_WORK_COMPLETE,          WAR_AUDIO_WAVE },
    { WAR_ORC_HELP_3,                   WAR_AUDIO_WAVE },
    { WAR_ORC_HELP_4,                   WAR_AUDIO_WAVE },
    { WAR_HUMAN_HELP_3,                 WAR_AUDIO_WAVE },
    { WAR_HUMAN_HELP_4,                 WAR_AUDIO_WAVE },
    { WAR_ORC_READY,                    WAR_AUDIO_WAVE },
    { WAR_HUMAN_READY,                  WAR_AUDIO_WAVE },
    { WAR_ORC_ACKNOWLEDGEMENT_1,        WAR_AUDIO_WAVE },
    { WAR_ORC_ACKNOWLEDGEMENT_2,        WAR_AUDIO_WAVE },
    { WAR_ORC_ACKNOWLEDGEMENT_3,        WAR_AUDIO_WAVE },
    { WAR_ORC_ACKNOWLEDGEMENT_4,        WAR_AUDIO_WAVE },
    { WAR_HUMAN_ACKNOWLEDGEMENT_1,      WAR_AUDIO_WAVE },
    { WAR_HUMAN_ACKNOWLEDGEMENT_2,      WAR_AUDIO_WAVE },
    { WAR_ORC_SELECTED_1,               WAR_AUDIO_WAVE },
    { WAR_ORC_SELECTED_2,               WAR_AUDIO_WAVE },
    { WAR_ORC_SELECTED_3,               WAR_AUDIO_WAVE },
    { WAR_ORC_SELECTED_4,               WAR_AUDIO_WAVE },
    { WAR_ORC_SELECTED_5,               WAR_AUDIO_WAVE },
    { WAR_HUMAN_SELECTED_1,             WAR_AUDIO_WAVE },
    { WAR_HUMAN_SELECTED_2,             WAR_AUDIO_WAVE },
    { WAR_HUMAN_SELECTED_3,             WAR_AUDIO_WAVE },
    { WAR_HUMAN_SELECTED_4,             WAR_AUDIO_WAVE },
    { WAR_HUMAN_SELECTED_5,             WAR_AUDIO_WAVE },
    { WAR_ORC_ANNOYED_1,                WAR_AUDIO_WAVE },
    { WAR_ORC_ANNOYED_2,                WAR_AUDIO_WAVE },
    { WAR_ORC_ANNOYED_3,                WAR_AUDIO_WAVE },
    { WAR_HUMAN_ANNOYED_1,              WAR_AUDIO_WAVE },
    { WAR_HUMAN_ANNOYED_2,              WAR_AUDIO_WAVE },
    { WAR_HUMAN_ANNOYED_3,              WAR_AUDIO_WAVE },
    { WAR_DEAD_SPIDER_SCORPION,         WAR_AUDIO_WAVE },
    { WAR_NORMAL_SPELL,                 WAR_AUDIO_WAVE },
    { WAR_BUILD_ROAD,                   WAR_AUDIO_WAVE },
    { WAR_ORC_TEMPLE,                   WAR_AUDIO_WAVE },
    { WAR_HUMAN_CHURCH,                 WAR_AUDIO_WAVE },
    { WAR_ORC_KENNEL,                   WAR_AUDIO_WAVE },
    { WAR_HUMAN_STABLE,                 WAR_AUDIO_WAVE },
    { WAR_BLACKSMITH,                   WAR_AUDIO_WAVE },
    { WAR_FIRE_CRACKLING,               WAR_AUDIO_WAVE },
    { WAR_CANNON,                       WAR_AUDIO_WAVE },
    { WAR_CANNON2,                      WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_ENDING_1,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_ENDING_2,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_ENDING_1,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_ENDING_2,       WAR_AUDIO_WAVE },
    { WAR_INTRO_1,                      WAR_AUDIO_WAVE },
    { WAR_INTRO_2,                      WAR_AUDIO_WAVE },
    { WAR_INTRO_3,                      WAR_AUDIO_WAVE },
    { WAR_INTRO_4,                      WAR_AUDIO_WAVE },
    { WAR_INTRO_5,                      WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_01_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_02_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_03_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_04_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_05_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_06_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_07_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_08_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_09_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_10_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_11_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_HUMAN_12_INTRO,     WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_01_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_02_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_03_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_04_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_05_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_06_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_07_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_08_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_09_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_10_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_11_INTRO,       WAR_AUDIO_WAVE },
    { WAR_CAMPAIGNS_ORC_12_INTRO,       WAR_AUDIO_WAVE },
    { WAR_HUMAN_DEFEAT,                 WAR_AUDIO_WAVE },
    { WAR_ORC_DEFEAT,                   WAR_AUDIO_WAVE },
    { WAR_ORC_VICTORY_1,                WAR_AUDIO_WAVE },
    { WAR_ORC_VICTORY_2,                WAR_AUDIO_WAVE },
    { WAR_ORC_VICTORY_3,                WAR_AUDIO_WAVE },
    { WAR_HUMAN_VICTORY_1,              WAR_AUDIO_WAVE },
    { WAR_HUMAN_VICTORY_2,              WAR_AUDIO_WAVE },
    { WAR_HUMAN_VICTORY_3,              WAR_AUDIO_WAVE },
};

WarAudioData getAudioData(WarAudioId audioId)
{
    s32 index = 0;
    s32 length = arrayLength(audioData);
    while (index < length && audioData[index].id != audioId)
        index++;

    assert(index < length);
    return audioData[index];
}

bool playMidi(WarContext* context, WarEntity* entity, u32 sampleCount, s16* outputStream)
{
    WarAudioComponent* audio = &entity->audio;

    while (sampleCount)
    {
        //We progress the MIDI playback and then process TSF_RENDER_EFFECTSAMPLEBLOCK samples at once
        u32 sampleBlock = min(TSF_RENDER_EFFECTSAMPLEBLOCK, sampleCount);
        audio->playbackTime += sampleBlock * (1000.0f / PLAYBACK_FREQ);

        //Loop through all MIDI messages which need to be played up until the current playback time
        tml_message* midiMessage = audio->currentMessage;
        tml_message* prevMessage = midiMessage;
        while (midiMessage)
        {
            // end of track
            if (midiMessage->type == TML_EOT)
            {
                midiMessage = audio->loop ? audio->firstMessage : NULL;
                audio->playbackTime -= prevMessage->time;
            }

            if (!midiMessage || midiMessage->time > audio->playbackTime)
            {
                break;
            }

            switch (midiMessage->type)
            {
                // channel program (preset) change (special handling for 10th MIDI channel with drums)
                case TML_PROGRAM_CHANGE: 
                {
                    tsf_channel_set_presetnumber(context->soundFont, midiMessage->channel, midiMessage->program, (midiMessage->channel == 9));
                    break;
                }

                // play a note
                case TML_NOTE_ON:
                {
                    tsf_channel_note_on(context->soundFont, midiMessage->channel, midiMessage->key, midiMessage->velocity / 127.0f);
                    break;
                }

                // stop a note
                case TML_NOTE_OFF:
                {
                    tsf_channel_note_off(context->soundFont, midiMessage->channel, midiMessage->key);
                    break;
                }
                
                // pitch wheel modification
                case TML_PITCH_BEND:
                {
                    tsf_channel_set_pitchwheel(context->soundFont, midiMessage->channel, midiMessage->pitch_bend);
                    break;
                }
                    
                // MIDI controller messages
                case TML_CONTROL_CHANGE:
                {
                    tsf_channel_midi_control(context->soundFont, midiMessage->channel, midiMessage->control, midiMessage->control_value);
                    break;
                }
            }

            prevMessage = midiMessage;
            midiMessage = midiMessage->next;
        }

        // Render the block of audio samples in float format
        tsf_render_short(context->soundFont, outputStream, sampleBlock, TSF_TRUE);

        sampleCount -= sampleBlock;
        outputStream += sampleBlock;

        audio->currentMessage = midiMessage;
    }

    return !audio->currentMessage && !audio->loop;
}

bool playWave(WarContext* context, WarEntity* entity, u32 sampleCount, s16* outputStream)
{
    WarAudioComponent* audio = &entity->audio;

    WarResource* resource = context->resources[audio->resourceIndex];
    if (!resource)
    {
        logError("Can't play audio %d, resource: %d\n", entity->id, audio->resourceIndex);
        return false;
    }

    s32 waveLength = resource->audio.length;

    do
    {
        if (audio->sampleIndex >= waveLength)
        {
            audio->sampleIndex = 0;
            audio->playbackTime = 0;
        }

        u8* waveData = &resource->audio.data[audio->sampleIndex];

        u32 sampleBlock = min(sampleCount, waveLength - audio->sampleIndex);
        for (s32 i = 0; i < sampleBlock; i++)
        {
            s16 value = *waveData;
            value = value - 128;
            value = value << 8;
            value += *outputStream;
            *outputStream = clamp(value, INT16_MIN, INT16_MAX);

            waveData++;
            outputStream++;
        }

        audio->playbackTime += sampleBlock * (1000.0f / PLAYBACK_FREQ);
        audio->sampleIndex += sampleBlock;
        
        sampleCount -= sampleBlock;
    } while (sampleCount > 0 && audio->loop);

    return audio->sampleIndex >= waveLength && !audio->loop;
}

bool playAudio(WarContext* context, WarEntity* entity, u32 sampleCount, s16* outputStream)
{
    WarAudioComponent* audio = &entity->audio;
    if (audio->enabled)
    {
        switch (audio->type)
        {
            case WAR_AUDIO_MIDI:
            {
                return playMidi(context, entity, sampleCount, outputStream);
            }

            case WAR_AUDIO_WAVE:
            {
                return playWave(context, entity, sampleCount, outputStream);
            }

            default:
            {
                logWarning("Unkown audio type: %d\n", audio->type);
                break;
            }
        }
    }

    return false;
}

void audioDataCallback(ma_device* sfx, void* output, const void* input, u32 sampleCount)
{
    NOT_USED(input);

    WarContext* context = (WarContext*)sfx->pUserData;
    if (!context) 
    {
        return;
    }

    WarMap* map = context->map;
    if (!map)
    {
        return;
    }

    WarEntityIdList toRemove;
    WarEntityIdListInit(&toRemove, WarEntityIdListDefaultOptions);

    s16* outputStream = (s16*)output;
    for (s32 i = 0; i < map->entities.count; i++)
    {
        WarEntity* entity = map->entities.items[i];
        if (entity && entity->type == WAR_ENTITY_TYPE_AUDIO)
        {
            if (playAudio(context, entity, sampleCount, outputStream))
            {
                // if the audio finish, mark it to remove it
                WarEntityIdListAdd(&toRemove, entity->id);
            }
        }
    }

    for (s32 i = 0; i < toRemove.count; i++)
    {
        WarEntityId entityId  = toRemove.items[i];
        removeEntityById(context, entityId);
    }
    
    WarEntityIdListFree(&toRemove);
}

bool initAudio(WarContext* context)
{
    context->soundFont = tsf_load_filename("GMGeneric.SF2");
    if (!context->soundFont)
    {
        logError("Could not load SoundFont\n");
        return false;
    }

   //Initialize preset on special 10th MIDI channel to use percussion sound bank (128) if available
    tsf_channel_set_bank_preset(context->soundFont, 9, 128, 0);

    // Set the SoundFont rendering output mode
    tsf_set_output(context->soundFont, TSF_MONO, PLAYBACK_FREQ, 0.0f);

    ma_device_config sfxConfig;
    sfxConfig = ma_device_config_init(ma_device_type_playback);
    sfxConfig.playback.format = ma_format_s16;
    sfxConfig.playback.channels = 1;
    sfxConfig.sampleRate = PLAYBACK_FREQ;
    sfxConfig.dataCallback = audioDataCallback;
    sfxConfig.pUserData = context;

    if (ma_device_init(NULL, &sfxConfig, &context->sfx) != MA_SUCCESS) {
        logError("Failed to open playback device.\n");
        return false;
    }

    if (ma_device_start(&context->sfx) != MA_SUCCESS) {
        logError("Failed to start playback device.\n");
        ma_device_uninit(&context->sfx);
        return false;
    }

   return true;
}

WarEntity* createAudio(WarContext* context, WarAudioId audioId, bool loop)
{
    WarAudioData data = getAudioData(audioId);

    s32 resourceIndex = (s32)audioId;
    WarAudioType type = data.type;

    WarEntity* entity = createEntity(context, WAR_ENTITY_TYPE_AUDIO, true);
    addAudioComponent(context, entity, type, resourceIndex, loop);

    return entity;
}

WarEntity* createAudioRandom(WarContext* context, WarAudioId fromId, WarAudioId toId, bool loop)
{
    return createAudio(context, randomi(fromId, toId + 1), loop);
}

/**
 * Extension function of MemoryBuffer to read variable lengths integer values.
 */
bool mbReadUIntVar(MemoryBuffer* buffer, u32* value)
{
    u32 v = 0;
    u8 byte;
    for(s32 i = 0; i < 4; ++i)
    {
        if(!mbRead(buffer, &byte))
            return false;

        v = (v << 7) | (u32)(byte & 0x7F);
        if((byte & 0x80) == 0)
            break;
    }
    *value = v;
    return true;
}

/**
 * Extension function of MemoryBuffer to write variable lengths integer values.
 */
bool mbWriteUIntVar(MemoryBuffer* buffer, u32 value)
{
    s32 byteCount = 1;
    u32 v = value & 0x7F;
    value >>= 7;
    while (value)
    {
        v = (v << 8) | 0x80 | (value & 0x7F);
        ++byteCount;
        value >>= 7;
    }
    
    for(s32 i = 0; i < byteCount; ++i)
    {
        u8 byte = v & 0xFF;
        if(!mbWrite(buffer, byte))
            return false;
        v >>= 8;
    }
    return true;
}

typedef struct _MidiToken
{
    s32 time;
    u32 bufferLength;
    u8* buffer;
    u8  type;
    u8  data;
} MidiToken;

static s32 compareTokens(const MidiToken left, const MidiToken right)
{
    return left.time - right.time;
}

shlDeclareList(MidiTokenList, MidiToken)
shlDefineList(MidiTokenList, MidiToken)

static MidiToken* MidiTokenListAppend(MidiTokenList* list, s32 time, u8 type)
{
    MidiToken token = {0};
    token.time = time;
    token.type = type;
    MidiTokenListAdd(list, token);
    return &list->items[list->count - 1];
}

/**
 * This code is a port in C of the XMI2MID converter by Peter "Corsix" Cawley 
 * in the War1gus repository. You can find the original C++ code here: 
 * https://github.com/Wargus/war1gus/blob/master/xmi2mid.cpp.
 * 
 * To understand more about these formats see:
 * http://www.shikadi.net/moddingwiki/XMI_Format
 * http://www.shikadi.net/moddingwiki/MID_Format
 * https://github.com/colxi/midi-parser-js/wiki/MIDI-File-Format-Specifications
 */
u8* transcodeXmiToMid(u8* xmiData, size_t xmiLength, size_t* midLength)
{
    MemoryBuffer bufInput = {0};
    mbInitFromMemory(&bufInput, xmiData, xmiLength);

    MemoryBuffer bufOutput = {0};
    mbInitEmpty(&bufOutput);

    if (!mbScanTo(&bufInput, "EVNT", 4))
    {
        mbFree(&bufOutput);
        return NULL;
    }

    if (!mbSkip(&bufInput, 8))
    {
        mbFree(&bufOutput);
        return NULL;
    }

    MidiTokenListOptions options = {0};

    MidiTokenList lstTokens;
    MidiTokenListInit(&lstTokens, options);

    MidiToken* token;
    s32 tokenTime = 0;
    s32 tempo = 500000;
    bool tempoSet = false;
    bool end = false;
    u8 tokenType, extendedType;
    u32 intVar;

    while (!mbIsEOF(&bufInput) && !end)
    {
        while (true)
        {
            if (!mbRead(&bufInput, &tokenType))
            {
                mbFree(&bufOutput);
                return NULL;
            }

            if (tokenType & 0x80)
                break;

            tokenTime += (s32)tokenType * 3;
        }

        token = MidiTokenListAppend(&lstTokens, tokenTime, tokenType);
        token->buffer = bufInput._pointer + 1;

        switch (tokenType & 0xF0)
        {
            case 0xC0:
            case 0xD0:
            {
                if (!mbRead(&bufInput, &token->data))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }

                token->buffer = NULL;
                break;
            }

            case 0x80:
            case 0xA0:
            case 0xB0:
            case 0xE0:
            {
                if (!mbRead(&bufInput, &token->data))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }

                if (!mbSkip(&bufInput, 1))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }

                break;
            }

            case 0x90:
            {
                if (!mbRead(&bufInput, &extendedType))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }

                token->data = extendedType;

                if (!mbSkip(&bufInput, 1))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }

                assert(mbReadUIntVar(&bufInput, &intVar));
                token = MidiTokenListAppend(&lstTokens, tokenTime + intVar * 3, tokenType);
                token->data = extendedType;
                token->buffer = (u8*)"\0";
            
                break;
            }

            case 0xF0:
            {
                extendedType = 0;

                if (tokenType == 0xFF)
                {
                    if (!mbRead(&bufInput, &extendedType))
                    {
                        mbFree(&bufOutput);
                        return NULL;
                    }

                    if (extendedType == 0x2F)
                        end = true;
                    else if (extendedType == 0x51)
                    {
                        if (!tempoSet)
                        {
                            assert(mbSkip(&bufInput, 1));
                            assert(mbReadInt24BE(&bufInput, &tempo));
                            tempo *= 3;
                            tempoSet = true;
                            assert(mbSkip(&bufInput, -4));
                        }
                        else
                        {
                            MidiTokenListRemoveAt(&lstTokens, lstTokens.count - 1);
                            assert(mbReadUIntVar(&bufInput, &intVar));
                            if (!mbSkip(&bufInput, intVar))
                            {
                                mbFree(&bufOutput);
                                return NULL;
                            }
                            break;
                        }
                    }
                }

                token->data = extendedType;
                assert(mbReadUIntVar(&bufInput, &token->bufferLength));
                token->buffer = bufInput._pointer;

                if (!mbSkip(&bufInput, token->bufferLength))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }

                break;
            }
        }
    }

    if (lstTokens.count == 0)
    {
        mbFree(&bufOutput);
        return NULL;
    }
    if (!mbWriteString(&bufOutput, "MThd\0\0\0\x06\0\0\0\x01", 12))
    {
        mbFree(&bufOutput);
        return NULL;
    }
    if (!mbWriteUInt16BE(&bufOutput, (tempo * 3) / 25000))
    {
        mbFree(&bufOutput);
        return NULL;
    }
    if (!mbWriteString(&bufOutput, "MTrk\xBA\xAD\xF0\x0D", 8))
    {
        mbFree(&bufOutput);
        return NULL;
    }

    MidiTokenListSort(&lstTokens, compareTokens);

    tokenTime = 0;
    tokenType = 0;
    end = false;

    for (s32 i = 0; i < lstTokens.count && !end; i++)
    {
        MidiToken t = lstTokens.items[i];

        if (!mbWriteUIntVar(&bufOutput, t.time - tokenTime))
        {
            mbFree(&bufOutput);
            return NULL;
        }

        tokenTime = t.time;

        if (t.type >= 0xF0)
        {
            tokenType = t.type;
            if (!mbWrite(&bufOutput, tokenType))
            {
                mbFree(&bufOutput);
                return NULL;
            }

            if (tokenType == 0xFF)
            {
                if (!mbWrite(&bufOutput, t.data))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }

                if (t.data == 0x2F)
                    end = true;
            }

            if (!mbWriteUIntVar(&bufOutput, t.bufferLength))
            {
                mbFree(&bufOutput);
                return NULL;
            }
            if (!mbWriteBytes(&bufOutput, t.buffer, t.bufferLength))
            {
                mbFree(&bufOutput);
                return NULL;
            }
        }
        else
        {
            if (t.type != tokenType)
            {
                tokenType = t.type;

                if (!mbWrite(&bufOutput, tokenType))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }
            }

            if (!mbWrite(&bufOutput, t.data))
            {
                mbFree(&bufOutput);
                return NULL;
            }

            if (t.buffer)
            {
                if (!mbWriteBytes(&bufOutput, t.buffer, 1))
                {
                    mbFree(&bufOutput);
                    return NULL;
                }
            }
        }
    }

    size32 length = mbPosition(&bufOutput) - 22;
    assert(mbSeek(&bufOutput, 18));
    assert(mbWriteUInt32BE(&bufOutput, length));

    u8* midData = mbGetData(&bufOutput, midLength);

    mbFree(&bufOutput);
    return midData;
}

u8* changeSampleRate(u8* samplesIn, s32 length, s32 factor)
{
    assert(factor >= 1);

    s32 newLength = length * factor;
    u8* samplesOut = (u8*)xmalloc(newLength);

    samplesOut[0] = samplesIn[0];

    for (s32 i = 1, j = 0; i < length; i++)
    {
        u8 a = samplesIn[i - 1];
        u8 b = samplesIn[i];

        f32 dt = 1 / factor;
        for (s32 k = 0; k < factor - 1; k++)
        {
            // linear interpolation: a + (b - a) * t
            f32 t = dt * (k + 1);
            samplesOut[j++] = (u8)(a + (b - a) * t);
        }

        samplesOut[j++] = b;
    }

    return samplesOut;
}
