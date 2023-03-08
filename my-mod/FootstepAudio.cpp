#include "pch.h"
#include "FootstepAudio.h"

#define TRAMPOLINE(name) ((decltype(name##_r)*)name##_t->Target())

/*static Trampoline* Sonic_Main_t = nullptr;
static Trampoline* Knuckles_Main_t = nullptr;

static bool SoundPlayed[8] = { };

static MLTSoundEntry NewHuntersSndEntries[] {
    { 2, 0, 0, 0, 0, 0 },
    { 2, 1, 0, 0, 0, 0 },
    { 2, 2, 0, 0, 0, 0 },
    { 2, 3, 0, 0, 0, 0 },
    { 2, 4, 0, 0, 0, 0 },
    { 2, 5, 0, 0, 0, 0 },
    { 2, 6, 0, 0, 0, 0 },
    { 2, 7, 0, 0, 0, 0 },
    { 2, 8, 0, 0, 0, 0 },
    { 2, 9, 0, 0, 0, 0 },
    { 2, 10, 0, 0, 0, 0 },
    { 2, 11, 0, 0, 0, 0 },
    { 2, 12, 0, 0, 0, 0 },
    { 2, 13, 0, 0, 0, 0 },
    { 2, 14, 0, 0, 0, 0 },
    { 2, 15, 0, 0, 0, 0 },
    { 2, 16, 0, 0, 0, 0 },
    { 2, 50, 0, 0, 0, 0 },
    { 2, 51, 0, 0, 0, 0 },
    { 2, 52, 2, 0, 0, 0 },
    { 2, 53, 2, 0, 0, 0 },
    { 2, 17, 2, 0, 0, 0 },
    { 2, 18, 0, 0, 0, 0 },
    { 2, 30, 0, 0, 0, 0 },
    { 2, 31, 0, 0, 0, 0 },
    { 2, 32, 0, 0, 0, 0 },
    { 2, 33, 0, 0, 0, 0 },
    { 2, 34, 0, 0, 0, 0 },
    { 2, 34, 0, 0, 0, 0 },
    { 2, 36, 0, 0, 0, 0 },
    { 2, 23, 0, 0, 0, 0 }, // Steps
    { 2, 24, 0, 0, 0, 0 }, // Rouge
};

static MLTSoundList NewHuntersSndList = { arraylengthandptrT(NewHuntersSndEntries, int) };

static MLTSoundEntry NewSpeedsSndEntries[] {
    { 2, 0, 0, 0, 0, 0 },
    { 2, 1, 0, 0, 0, 0 },
    { 2, 2, 0, 0, 0, 0 },
    { 2, 3, 0, 0, 0, 0 },
    { 2, 4, 0, 0, 0, 0 },
    { 2, 5, 0, 0, 0, 0 },
    { 2, 6, 0, 0, 0, 0 },
    { 2, 7, 0, 0, 0, 0 },
    { 2, 8, 0, 0, 0, 0 },
    { 2, 9, 0, 0, 0, 0 },
    { 2, 10, 0, 0, 0, 0 },
    { 2, 11, 0, 0, 0, 0 },
    { 2, 12, 0, 0, 0, 0 },
    { 2, 13, 0, 0, 0, 0 },
    { 2, 14, 0, 0, 0, 0 },
    { 2, 51, 0, 0, 0, 0 },
    { 2, 52, 0, 0, 0, 0 },
    { 2, 15, 0, 0, 0, 0 },
    { 2, 16, 0, 0, 0, 0 },
    { 2, 17, 0, 0, 0, 0 },
    { 2, 30, 0, 0, 0, 0 },
    { 2, 31, 0, 0, 0, 0 },
    { 2, 32, 0, 0, 0, 0 },
    { 2, 33, 0, 0, 0, 0 },
    { 2, 34, 0, 0, 0, 0 },
    { 2, 35, 0, 0, 0, 0 },
    { 2, 36, 0, 0, 0, 0 },
    { 2, 37, 0, 0, 0, 0 },
    { 2, 38, 0, 0, 0, 0 },
    { 2, 21, 0, 0, 0, 0 }, // Steps
    { 2, 22, 0, 0, 0, 0 }, // Shadow R
    { 2, 23, 0, 0, 0, 0 }, // Shadow L
};

static MLTSoundList NewSpeedsSndList = { arraylengthandptrT(NewSpeedsSndEntries, int) };

enum NewHuntersSounds
{
    HSnd_HuntersStep = 0x201E,
    HSnd_RougeStep
};

enum NewSpeedsSounds
{
    SSnd_SpeedsStep = 0x201D,
    SSnd_ShadowAirR,
    SSnd_ShadowAirL
};

void PlayFootSound(EntityData1* data, CharObj2Base* co2, int character, int soundL, int soundR, int animation, float frameL, float frameR)
{
    if (co2->CharID2 == character)
    {
        if (co2->AnimInfo.Current == animation)
        {
            if (co2->AnimInfo.nframe > frameL && co2->AnimInfo.nframe < frameR)
            {
                if (SoundPlayed[co2->PlayerNum] != true)
                {
                    Play3DSound_Pos(soundL, &data->Position, (int)data, 0, 0);
                    SoundPlayed[co2->PlayerNum] = true;
                }
            }
            if (co2->AnimInfo.nframe < frameL || co2->AnimInfo.nframe > frameR)
            {
                if (SoundPlayed[co2->PlayerNum] != false)
                {
                    Play3DSound_Pos(soundR, &data->Position, (int)data, 0, 0);
                    SoundPlayed[co2->PlayerNum] = false;
                }
            }
        }
    }
}

void PlayFootSound(EntityData1* data, CharObj2Base* co2, int character, int sound, int animation, float frameL, float frameR)
{
    PlayFootSound(data, co2, character, sound, sound, animation, frameL, frameR);
}

void __cdecl Knuckles_Main_r(ObjectMaster* obj)
{
    TRAMPOLINE(Knuckles_Main)(obj);

    auto data = obj->Data1.Entity;
    auto co2 = obj->Data2.Character;

    PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeStep, 6, 10.0f, 38.0f);
    PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeStep, 7, 7.0f, 25.0f);
    PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeStep, 8, 11.0f, 39.0f);
    PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeStep, 9, 27.0f, 66.0f);
}

void __cdecl Sonic_Main_r(ObjectMaster* obj)
{
    TRAMPOLINE(Sonic_Main)(obj);

    auto data = obj->Data1.Entity;
    auto co2 = obj->Data2.Character;

    PlayFootSound(data, co2, Characters_Sonic, SSnd_SpeedsStep, 6, 16.0f, 35.0f);
    PlayFootSound(data, co2, Characters_Sonic, SSnd_SpeedsStep, 7, 22.0f, 50.0f);
    PlayFootSound(data, co2, Characters_Sonic, SSnd_SpeedsStep, 8, 30.0f, 69.0f);
    PlayFootSound(data, co2, Characters_Sonic, SSnd_SpeedsStep, 9, 22.0f, 55.0f);
    PlayFootSound(data, co2, Characters_Sonic, SSnd_SpeedsStep, 10, 22.0f, 54.0f);

    PlayFootSound(data, co2, Characters_Shadow, SSnd_SpeedsStep, 6, 16.0f, 35.0f);
    PlayFootSound(data, co2, Characters_Shadow, SSnd_SpeedsStep, 7, 30.0f, 69.0f);
    PlayFootSound(data, co2, Characters_Shadow, SSnd_ShadowAirL, SSnd_ShadowAirR, 8, 4.0f, 8.0f);
    PlayFootSound(data, co2, Characters_Shadow, SSnd_ShadowAirL, SSnd_ShadowAirR, 9, 10.0f, 45.0f);
    PlayFootSound(data, co2, Characters_Shadow, SSnd_ShadowAirL, SSnd_ShadowAirR, 10, 14.0f, 52.0f);
}

const void* const loc_71729f = (const void*)0x71729f;
__declspec(naked) void JumpJiggleAmy()
{
    __asm
    {

        sub esp, 4
        jmp loc_71729f
    }
}


const void* const loc_717028 = (const void*)0x717028;
__declspec(naked) void JumpJiggleSonic()
{
    __asm
    {
        mov esi, [esp + 0Ch]
        sub esp, 4
        jmp loc_717028
    }
}
const void* const loc_7287c0 = (const void*)0x7287c0;
__declspec(naked) void JumpJiggleRouge()
{
    __asm
    {
        mov esi, [esp + 0Ch]
        sub esp, 4
        jmp loc_7287c0
    }
}
const void* const loc_717588 = (const void*)0x717588;
__declspec(naked) void JumpJiggleShadow()
{
    __asm
    {
        mov esi, [esp + 0Ch]
        sub esp, 4
        jmp loc_717588
    }
}

const void* const loc_728399 = (const void*)0x728399;
__declspec(naked) void JumpJiggleKnux()
{
    __asm
    {
        mov esi, [esp + 0Ch]
        sub esp, 4
        jmp loc_728399
    }
}

const void* const loc_728a8a = (const void*)0x728a8a;
__declspec(naked) void JumpJiggleTikal() {
    __asm
    {
        sub esp, 4
        jmp loc_728a8a
    }
}
const void* const loc_49ab13 = (const void*)0x49ab13;
__declspec(naked) void JumpJiggleSuperSonic()
{
    __asm
    {
        sub esp, 4
        jmp loc_49ab13
    }
}

const void* const loc_49ad77 = (const void*)0x49ad77;
__declspec(naked) void JumpJiggleSuperShadow()
{
    __asm
    {

        sub esp, 4
        jmp loc_49ad77
    }
}

extern "C"
{
    __declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
    {
        CharacterSoundBanks1P[0].Name = (char*)0x8B93F4; // Sonic, Metal Sonic
        CharacterSoundBanks1P[0].SoundList = &NewSpeedsSndList;
        CharacterSoundBanks1P[1].Name = (char*)0x8B93F4; // Shadow, Amy
        CharacterSoundBanks1P[1].SoundList = &NewSpeedsSndList;
        CharacterSoundBanks1P[4].Name = (char*)0x8B93CC; // Knuckles, Tikal
        CharacterSoundBanks1P[4].SoundList = &NewHuntersSndList;
        CharacterSoundBanks1P[5].Name = (char*)0x8B93CC; // Rouge, Chaos
        CharacterSoundBanks1P[5].SoundList = &NewHuntersSndList;
        CharacterSoundBanks2P[0].SoundList = &NewSpeedsSndList;
        CharacterSoundBanks2P[1].SoundList = &NewSpeedsSndList;
        CharacterSoundBanks2P[4].SoundList = &NewHuntersSndList;
        CharacterSoundBanks2P[5].SoundList = &NewHuntersSndList;

        Knuckles_Main_t = new Trampoline(0x728D70, 0x728D76, Knuckles_Main_r, false);
        Sonic_Main_t = new Trampoline(0x717840, 0x717846, Sonic_Main_r, false);

        WriteJump((void*)0x716FDD, JumpJiggleSonic);
        WriteJump((void*)0x71753d, JumpJiggleShadow);
        WriteJump((void*)0x728346, JumpJiggleKnux);
        WriteJump((void*)0x728696, JumpJiggleRouge);
        WriteJump((void*)0x717257, JumpJiggleAmy);
        WriteJump((void*)0x49aacb, JumpJiggleSuperSonic);
        WriteJump((void*)0x49ad2f, JumpJiggleSuperShadow);
        WriteJump((void*)0x728a3c, JumpJiggleTikal);
        WriteData<3>((void*)0x728a8e, 0x90); // tikal jiggle
    }

    __declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
; */