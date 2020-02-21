#pragma once

#define CHEAT_GOLD_INCREASE 10000
#define CHEAT_WOOD_INCREASE 5000
#define CHEAT_SPEED_UP_FACTOR 100.0f

typedef void (*WarCheatFunc)(WarContext* context, const char* argument);

typedef struct
{
    WarCheat cheat;
    char text[CHEAT_TEXT_MAX_LENGTH];
    bool argument;
    WarCheatFunc cheatFunc;
} WarCheatDescriptor;

void applyCheat(WarContext* context, const char* text);

// original cheats
void applyGoldCheat(WarContext* context, const char* argument);
void applySpellsCheat(WarContext* context, const char* argument);
void applyUpgradesCheat(WarContext* context, const char* argument);
void applyEndCheat(WarContext* context, const char* argument);
void applyEnableCheat(WarContext* context, const char* argument);
void applyGodModeCheat(WarContext* context, const char* argument);
void applyWinCheat(WarContext* context, const char* argument);
void applyLossCheat(WarContext* context, const char* argument);
void applyFogOfWarCheat(WarContext* context, const char* argument);
void applySkipHumanCheat(WarContext* context, const char* argument);
void applySkipOrcCheat(WarContext* context, const char* argument);
void applySpeedCheat(WarContext* context, const char* argument);

// custom cheats
void applyMusicCheat(WarContext* context, const char* argument);
void applySoundCheat(WarContext* context, const char* argument);
void applyMusicVolCheat(WarContext* context, const char* argument);
void applySoundVolCheat(WarContext* context, const char* argument);
void applyGlobalScaleCheat(WarContext* context, const char* argument);
void applyGlobalSpeedCheat(WarContext* context, const char* argument);
void applyEditTreesCheat(WarContext* context, const char* argument);
void applyEditRoadsCheat(WarContext* context, const char* argument);
void applyEditRuinsCheat(WarContext* context, const char* argument);
void applyEditWallsCheat(WarContext* context, const char* argument);
void applyRainOfFireCheat(WarContext* context, const char* argument);
