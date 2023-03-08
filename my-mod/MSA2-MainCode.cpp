#include "pch.h"
#include "SA2ModLoader.h"
#include "FootstepAudio.h"
#include "Trampoline.h"

#define TRAMPOLINE(name) ((decltype(name##_r)*)name##_t->Target())

void InitFootStep();
void PlayFootSound(EntityData1* data, CharObj2Base* co2, int character, int soundL, int soundR, int animation, float frameL, float frameR);
void PlayFootSound(EntityData1* data, CharObj2Base* co2, int character, int sound, int animation, float frameL, float frameR);
void __cdecl Knuckles_Main_r(ObjectMaster* obj);
void __cdecl Sonic_Main_r(ObjectMaster* obj);



int lightV1; bool smokeGone; bool rocketOn; bool startCheck;
float getRotX; float getRotZ; float getRotYX; float getRotYZ;
float convYX;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Animation List Format : Anim.ID | Model ID | Interpolation Type | Next Anim. | Transition Speed | Anim.Speed
// Interpolation Properties: 
0	Load first frame
1	Load first frame
2	No animation
3	Play animation, loop
4	Play animation, hard cut to NextAnimation
5	Play animation, hard cut to NextAnimation
6	Play animation, stop on last frame
7	Play animation in reverse, hard cut to NextAnimation
8	Play animation in reverse, stop on last frame
9	Play animation, smooth transition to NextAnimation
10	Play animation with directional input, slow
11	Play animation with directional input, normal
12	Play animation with directional input, fast
13	Animation playback is handled by code, otherwise uses first frame */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AnimationInfo SonicAnimationList[] = {
	{ 0, 0, 3, 0, 0.0625f, 0.1f },
	{ 1, 0, 3, 1, 0.25f, 0.1f },
	{ 2, 0, 3, 2, 0.25f, 0.1f },
	{ 3, 0, 4, 0, 0.25f, 1 },
	{ 4, 0, 3, 4, 0.03125f, 0.1f },
	{ 5, 0, 3, 5, 0.03125f, 0.1f },
	{ 6, 0, 11, 6, 0.25f, 3.4f },
	{ 7, 0, 11, 7, 0.25f, 1.4f },
	{ 8, 0, 11, 8, 0.25f, 0.8f },
	{ 9, 0, 11, 9, 0.25f, 0.6f },
	{ 10, 0, 10, 10, 0.5f, 0.4f },
	{ 11, 0, 11, 11, 0.25f, 0.2f },
	{ 12, 0, 10, 12, 0.25f, 0.7f },
	{ 13, 0, 3, 13, 0.25f, 0.4f },
	{ 14, 0, 4, 15, 0.125f, 0.3f },
	{ 15, 0, 3, 15, 0.125f, 0.4f },
	{ 16, 0, 4, 0, 1, 0.2f },
	{ 17, 0, 4, 0, 1, 0.4f },
	{ 18, 0, 9, 0, 0.25f, 0.3f },
	{ 19, 0, 4, 20, 0.125f, 0.2f },
	{ 20, 0, 3, 20, 0.25f, 0.6f },
	{ 21, 0, 6, 21, 1, 0.1f },
	{ 22, 0, 4, 0, 1, 1 },
	{ 23, 0, 3, 23, 0.0625f, 1 },
	{ 24, 0, 4, 25, 0.5f, 0.4f },
	{ 25, 0, 4, 0, 0.0625f, 0.3f },
	{ 26, 0, 4, 1, 0.0625f, 1 },
	{ 27, 0, 4, 0, 1, 0.5f },
	{ 28, 0, 3, 28, 0.0625f, 0.2f },
	{ 29, 0, 3, 29, 0.0625f, 0.2f },
	{ 30, 6, 3, 30, 0.5f, 1 },
	{ 31, 0, 9, 35, 0.125f, 0.2f },
	{ 32, 0, 11, 32, 0.5f, 0.4f },
	{ 33, 0, 6, 33, 0.25f, 0.2f },
	{ 34, 0, 4, 35, 0.5f, 1 },
	{ 35, 0, 3, 35, 0.125f, 0.1f },
	{ 36, 0, 3, 36, 0.03125f, 0.1f },
	{ 37, 0, 3, 37, 0.03125f, 0.1f },
	{ 38, 0, 11, 38, 0.5f, 0.5f },
	{ 39, 0, 4, 6, 0.5f, 0.5f },
	{ 40, 0, 3, 40, 0.5f, 0.6f },
	{ 41, 0, 3, 41, 0.5f, 0.4f },
	{ 42, 0, 3, 42, 0.125f, 0.3f },
	{ 43, 0, 4, 44, 0.5f, 0.2f },
	{ 44, 0, 3, 44, 0.5f, 0.1f },
	{ 45, 0, 3, 45, 0.5f, 0.1f },
	{ 46, 0, 4, 47, 0.5f, 0.1f },
	{ 47, 0, 3, 47, 0.5f, 0.3f },
	{ 48, 0, 4, 35, 1, 0.04f },
	{ 49, 0, 4, 0, 0.5f, 0.3f },
	{ 50, 0, 4, 0, 0.5f, 0.3f },
	{ 51, 0, 4, 15, 0.5f, 0.3f },
	{ 52, 0, 4, 15, 0.5f, 0.3f },
	{ 53, 0, 7, 0, 1, 0.2f },
	{ 54, 0, 6, 54, 1, 0.5f },
	{ 55, 0, 3, 55, 0.125f, 0.3f },
	{ 56, 0, 4, 15, 1, 0.25f },
	{ 57, 0, 4, 58, 0.125f, 0.1f },
	{ 58, 0, 3, 58, 1, 0.1f },
	{ 59, 0, 6, 59, 1, 0.2f },
	{ 60, 0, 6, 60, 0.25f, 1 },
	{ 61, 0, 3, 61, 0.25f, 0.5f },
	{ 62, 0, 6, 62, 0.25f, 0.1f },
	{ 63, 0, 4, 0, 0.25f, 0.3f },
	{ 64, 0, 4, 0, 0.25f, 0.3f },
	{ 65, 0, 4, 66, 0.25f, 1.4f },
	{ 66, 0, 3, 66, 0.25f, 1 },
	{ 67, 0, 4, 68, 0.125f, 0.4f },
	{ 68, 0, 3, 68, 0.125f, 0.4f },
	{ 69, 0, 4, 70, 0.25f, 0.1f },
	{ 70, 0, 3, 70, 1, 0.2f },
	{ 71, 0, 9, 0, 0.125f, 0.2f },
	{ 72, 0, 6, 72, 0.25f, 0.2f },
	{ 73, 0, 4, 0, 0.25f, 0.3f },
	{ 74, 0, 3, 74, 0.25f, 0.4f },
	{ 75, 0, 3, 75, 0.25f, 0.1f },
	{ 76, 0, 6, 76, 0.25f, 0.3f },
	{ 77, 0, 3, 77, 0.25f, 0.8f },
	{ 78, 0, 3, 78, 0.125f, 0.2f },
	{ 79, 0, 9, 35, 0.25f, 0.5f },
	{ 80, 0, 5, 81, 0.25f, 0.5f },
	{ 81, 0, 4, 0, 0.25f, 0.5f },
	{ 82, 0, 9, 0, 0.25f, 0.5f },
	{ 83, 0, 4, 0, 0.0625f, 0.15f },
	{ 84, 0, 4, 0, 0.0625f, 0.15f },
	{ 85, 0, 6, 85, 0.25f, 0.5f },
	{ 86, 0, 3, 86, 1, 0.2f },
	{ 87, 0, 3, 87, 1, 0.5f },
	{ 88, 0, 3, 88, 0.03125f, 0.3f },
	{ 89, 0, 3, 89, 0.03125f, 0.1f },
	{ 90, 0, 9, 0, 0.03125f, 0.25f },
	{ 91, 0, 4, 92, 0.25f, 1 },
	{ 92, 0, 4, 93, 0.25f, 1 },
	{ 93, 0, 4, 16, 0.25f, 0.2f },
	{ 94, 0, 9, 0, 0.25f, 0.3f },
	{ 95, 0, 9, 0, 0.25f, 0.3f },
	{ 96, 0, 4, 97, 0.25f, 1 },
	{ 97, 0, 4, 98, 0.25f, 1 },
	{ 98, 0, 4, 99, 0.25f, 0.2f },
	{ 99, 0, 4, 16, 0.25f, 1 },
	{ 66, 0, 3, 100, 1, 3 },
	{ 101, 0, 6, 67, 0.25f, 0.3f },
	{ 102, 0, 6, 67, 0.25f, 0.3f },
	{ 103, 0, 6, 67, 0.25f, 0.3f },
	{ 104, 0, 13, 104, 0.25f, 0.3f },
	{ 105, 0, 13, 105, 0.25f, 0.3f },
	{ 106, 0, 13, 106, 0.25f, 0.3f },
	{ 107, 0, 13, 107, 0.25f, 0.3f },
	{ 108, 0, 13, 108, 0.25f, 0.3f },
	{ 109, 0, 13, 109, 0.25f, 0.3f },
	{ 110, 0, 13, 110, 0.25f, 0.3f },
	{ 111, 0, 13, 111, 0.25f, 0.3f },
	{ 112, 0, 3, 112, 0.25f, 0.3f },
	{ 113, 0, 3, 113, 0.25f, 0.3f },
	{ 114, 0, 9, 15, 0.0625f, 0.2f },
	{ 115, 0, 4, 15, 0.0625f, 0.35f },
	{ 116, 0, 4, 15, 0.0625f, 0.35f },
	{ 117, 0, 4, 15, 0.125f, 1 },
	{ 118, 0, 6, 118, 1, 1 },
	{ 119, 0, 6, 119, 1, 1 },
	{ 120, 0, 3, 120, 1, 0.1f },
	{ 121, 0, 13, 121, 0.25f, 0.3f },
	{ 122, 0, 13, 122, 0.25f, 0.3f },
	{ 123, 0, 13, 123, 0.25f, 0.3f },
	{ 124, 0, 13, 124, 0.25f, 0.3f },
	{ 125, 0, 3, 125, 0.25f, 0.3f },
	{ 126, 0, 3, 126, 0.25f, 0.3f },
	{ 127, 0, 6, 127, 0.25f, 0.4f },
	{ 128, 0, 4, 129, 0.125f, 0.3f },
	{ 129, 0, 3, 129, 0.125f, 0.4f },
	{ 130, 0, 4, 121, 1, 0.3f },
	{ 132, 0, 4, 121, 0.25f, 0.3f },
	{ 131, 0, 4, 122, 0.25f, 0.3f },
	{ 133, 0, 4, 130, 1, 1 },
	{ 134, 0, 4, 130, 1, 1 },
	{ 135, 0, 4, 130, 0.0625f, 1.25f },
	{ 136, 0, 4, 130, 0.0625f, 1 },
	{ 137, 0, 4, 130, 1, 1 },
	{ 138, 0, 4, 130, 0.0625f, 1.25f },
	{ 139, 0, 4, 130, 1, 1 },
	{ 140, 0, 4, 130, 1, 1 },
	{ 141, 0, 4, 130, 0.0625f, 1.25f },
	{ 142, 0, 4, 130, 0.0625f, 1.25f },
	{ 143, 0, 4, 130, 1, 1 },
	{ 144, 0, 4, 130, 0.0625f, 1 },
	{ 145, 0, 13, 121, 0.25f, 0.3f },
	{ 146, 0, 13, 122, 0.25f, 0.3f },
	{ 147, 0, 13, 123, 0.25f, 0.3f },
	{ 148, 0, 13, 124, 0.25f, 0.3f },
	{ 149, 0, 3, 125, 0.25f, 0.3f },
	{ 150, 0, 3, 126, 0.25f, 0.3f },
	{ 151, 0, 6, 127, 0.25f, 0.4f },
	{ 152, 0, 4, 129, 0.125f, 0.3f },
	{ 153, 0, 3, 129, 0.125f, 0.4f },
	{ 154, 0, 4, 121, 0.125f, 0.4f },
	{ 156, 0, 4, 121, 0.25f, 0.3f },
	{ 155, 0, 4, 122, 0.25f, 0.3f },
	{ 157, 0, 9, 129, 0.0625f, 1 },
	{ 158, 0, 9, 129, 0.0625f, 1 },
	{ 159, 0, 9, 129, 0.0625f, 1 },
	{ 160, 0, 9, 129, 0.0625f, 1 },
	{ 161, 0, 9, 129, 0.0625f, 1 },
	{ 162, 0, 9, 129, 0.0625f, 1 },
	{ 163, 0, 9, 129, 0.0625f, 1 },
	{ 164, 0, 9, 129, 0.0625f, 1 },
	{ 165, 0, 9, 129, 0.0625f, 1 },
	{ 166, 0, 9, 129, 0.0625f, 1 },
	{ 167, 0, 9, 129, 0.0625f, 1 },
	{ 168, 0, 9, 129, 0.0625f, 1 },
	{ 169, 0, 3, 169, 0.0625f, 0.1f },
	{ 170, 0, 3, 170, 0.25f, 0.1f },
	{ 171, 0, 11, 171, 0.25f, 1.4f },
	{ 172, 0, 11, 172, 0.25f, 1.4f },
	{ 173, 0, 11, 173, 0.25f, 1.4f },
	{ 174, 0, 11, 174, 0.25f, 1.4f },
	{ 175, 0, 4, 169, 0.25f, 1.4f },
	{ 176, 0, 3, 176, 0.03125f, 0.1f },
	{ 177, 0, 3, 177, 0.03125f, 0.1f },
	{ 178, 0, 9, 0, 0.03125f, 0.25f },
	{ 179, 0, 4, 169, 0.25f, 1.4f },
	{ 180, 0, 6, 180, 0.25f, 1.4f },
	{ 181, 0, 6, 181, 0.25f, 1.4f },
	{ 182, 0, 6, 182, 1, 1 },
	{ 183, 0, 6, 183, 1, 1 },
	{ 184, 0, 3, 184, 1, 0.5f },
	{ 185, 0, 4, 186, 0.25f, 0.3f },
	{ 186, 0, 3, 186, 0.25f, 0.5f },
	{ 187, 0, 3, 187, 0.25f, 0.8f },
	{ 188, 0, 6, 188, 0.25f, 0.3f },
	{ 189, 0, 4, 189, 0.25f, 0.2f },
	{ 190, 0, 3, 190, 0.25f, 0.3f },
	{ 191, 0, 3, 191, 0.25f, 0.2f },
	{ 192, 0, 11, 192, 0.25f, 0.2f },
	{ 193, 0, 11, 192, 0.25f, 0.2f },
	{ 194, 0, 3, 194, 0.0625f, 0.2f },
	{ 195, 0, 3, 195, 0.0625f, 0.2f },
	{ 196, 0, 3, 196, 0.0625f, 0.2f },
	{ 197, 0, 3, 197, 0.0625f, 0.2f },
	{ 198, 0, 3, 198, 0.0625f, 0.2f }, // TS = 0.125f vanilla
	{ 199, 0, 3, 199, 0.25f, 0.2f },
	{ 200, 0, 3, 200, 0.25f, 0.2f },
	{ 201, 0, 6, 201, 0.125f, 0.8f },
	{ 202, 0, 3, 202, 0.125f, 0.5f }
};

AnimationInfo RougeAnimationList[] = {
	{ 168, 171, 3, 0, 0.0625f, 0.1f },
	{ 1, 171, 3, 1, 0.25f, 0.1f },
	{ 2, 171, 3, 2, 0.25f, 0.1f },
	{ 3, 171, 4, 0, 0.25f, 1 },
	{ 4, 171, 3, 4, 0.03125f, 0.1f },
	{ 5, 171, 3, 5, 0.03125f, 0.1f },
	{ 150, 171, 11, 6, 0.25f, 3.4f },
	{ 151, 171, 11, 7, 0.20f, 1.5f },
	{ 152, 171, 11, 8, 0.25f, 0.8f },
	{ 153, 171, 11, 9, 0.25f, 1.2f },
	{ 154, 171, 10, 10, 0.25f, 1.3f },
	{ 11, 171, 11, 11, 0.25f, 0.2f },
	{ 12, 171, 10, 12, 0.25f, 0.7f },
	{ 13, 171, 3, 13, 0.25f, 0.5 },
	{ 14, 171, 4, 15, 0.13f, 0.725 },
	{ 15, 171, 3, 15, 0.125f, 0.3f },
	{ 16, 171, 4, 0, 1, 0.2f },
	{ 173, 171, 4, 0, 1, 0.25f },
	{ 18, 171, 9, 0, 0.25f, 0.3f },
	{ 19, 171, 4, 20, 0.125f, 0.2f },
	{ 20, 171, 3, 20, 0.25f, 0.6f },
	{ 21, 171, 6, 21, 1, 0.1f },
	{ 22, 171, 4, 0, 1, 1 },
	{ 23, 171, 3, 23, 0.0625f, 1 },
	{ 24, 171, 4, 25, 0.25f, 0.4f },
	{ 25, 171, 4, 0, 0.0625f, 0.3f },
	{ 26, 171, 4, 1, 0.0625f, 1 },
	{ 27, 171, 4, 0, 1, 0.5f },
	{ 28, 171, 3, 28, 0.0625f, 0.2f },
	{ 29, 171, 3, 29, 0.0625f, 0.2f },
	{ 30, 171, 3, 30, 0.5f, 1 },
	{ 169, 171, 4, 35, 0.5f, 0.5f },
	{ 200, 171, 11, 32, 0.5f, 0.4f },
	{ 33, 171, 4, 35, 0.5f, 1 },
	{ 34, 171, 4, 35, 0.5f, 1 },
	{ 171, 171, 3, 35, 0.5f, 0.1f },
	{ 36, 171, 3, 36, 0.03125f, 0.1f },
	{ 37, 171, 3, 37, 0.03125f, 0.1f },
	{ 199, 171, 11, 38, 0.5f, 0.5f },
	{ 39, 171, 4, 6, 0.5f, 0.5f },
	{ 40, 171, 3, 40, 0.5f, 0.6f },
	{ 41, 171, 3, 41, 0.5f, 0.4f },
	{ 201, 171, 3, 42, 0.5f, 0.3f },
	{ 43, 171, 4, 44, 0.5f, 0.2f },
	{ 44, 171, 3, 44, 0.5f, 0.1f },
	{ 202, 171, 3, 45, 0.5f, 0.1f },
	{ 203, 171, 4, 47, 0.5f, 0.1f },
	{ 204, 171, 3, 47, 0.5f, 0.3f },
	{ 48, 171, 4, 35, 1, 0.04f },
	{ 49, 171, 4, 0, 0.5f, 0.3f },
	{ 50, 171, 4, 0, 0.5f, 0.3f },
	{ 51, 171, 4, 15, 0.5f, 0.3f },
	{ 52, 171, 4, 15, 0.5f, 0.3f },
	{ 170, 171, 7, 0, 1, 0.2f },
	{ 174, 171, 6, 54, 1, 0.25f },
	{ 55, 171, 3, 55, 0.125f, 0.3f },
	{ 56, 171, 4, 15, 1, 0.25f },
	{ 57, 171, 4, 58, 0.125f, 0.1f },
	{ 58, 171, 3, 58, 1, 0.1f },
	{ 59, 171, 6, 59, 1, 0.2f },
	{ 60, 171, 3, 60, 0.25f, 1 },
	{ 61, 171, 3, 61, 0.25f, 0.5f },
	{ 62, 171, 6, 62, 0.25f, 0.1f },
	{ 63, 171, 4, 0, 0.25f, 0.3f },
	{ 64, 171, 4, 0, 0.25f, 0.3f },
	{ 65, 171, 4, 66, 0.25f, 1.4f },
	{ 66, 171, 3, 66, 0.25f, 0.82 },
	{ 67, 171, 4, 15, 0.065f, 0.2f },
	{ 68, 171, 3, 15, 0.12f, 0.4f },
	{ 69, 171, 4, 70, 0.25f, 0.1f },
	{ 172, 171, 3, 70, 1, 0.2f },
	{ 71, 171, 9, 0, 0.125f, 0.2f },
	{ 188, 171, 6, 72, 0.25f, 0.5f },
	{ 73, 171, 4, 0, 0.25f, 0.3f },
	{ 74, 171, 3, 74, 0.25f, 0.4f },
	{ 198, 171, 3, 75, 0.25f, 0.1f },
	{ 76, 171, 6, 76, 0.25f, 0.3f },
	{ 77, 171, 3, 77, 0.25f, 0.8f },
	{ 78, 171, 3, 78, 0.125f, 0.2f },
	{ 79, 171, 4, 0, 0.25f, 0.3f },
	{ 80, 171, 5, 81, 0.25f, 0.5f },
	{ 81, 171, 4, 0, 0.25f, 0.5f },
	{ 82, 171, 9, 0, 0.25f, 0.5f },
	{ 83, 171, 4, 0, 0.0625f, 0.15f },
	{ 84, 171, 4, 0, 0.0625f, 0.15f },
	{ 175, 171, 6, 85, 1, 0.25f },
	{ 86, 171, 3, 86, 1, 0.2f },
	{ 87, 171, 3, 87, 0.03125f, 0.05f },
	{ 88, 171, 3, 88, 0.03125f, 0.1f },
	{ 89, 171, 9, 0, 0.03125f, 0.25f },
	{ 164, 171, 4, 91, 0.125f, 0.5f },
	{ 165, 171, 3, 91, 0.25f, 0.5f },
	{ 189, 171, 3, 92, 0.25f, 0.3f },
	{ 190, 171, 4, 68, 0.25f, 0.3f },
	{ 191, 171, 9, 0, 0.25f, 0.2f },
	{ 166, 171, 4, 96, 0.125f, 0.5f },
	{ 167, 171, 3, 96, 0.25f, 0.5f },
	{ 192, 171, 3, 97, 0.25f, 0.3f },
	{ 193, 171, 4, 103, 0.25f, 0.5f },
	{ 194, 171, 9, 0, 0.25f, 0.2f },
	{ 159, 171, 10, 100, 0.25f, 1.6f },
	{ 160, 171, 11, 101, 0.25f, 1.6f },
	{ 161, 171, 11, 102, 0.25f, 1.6f },
	{ 162, 171, 3, 103, 0.25f, 0.3f },
	{ 195, 171, 4, 15, 0.25f, 0.5f },
	{ 155, 171, 3, 105, 0.15f, 0.6f },
	{ 156, 171, 4, 103, 1, 0.4f },
	{ 157, 171, 4, 0, 0.25f, 0.4f },
	{ 158, 171, 4, 68, 0.25f, 0.2f },
	{ 144, 171, 4, 110, 1, 0.5f },
	{ 145, 171, 4, 111, 1, 0.5f },
	{ 146, 171, 6, 15, 1, 0.6f },
	{ 147, 171, 4, 113, 1, 0.5f },
	{ 148, 171, 4, 114, 1, 0.5f },
	{ 149, 171, 3, 114, 0.25f, 1 },
	{ 163, 171, 3, 115, 0.125f, 1 },
	{ 116, 171, 4, 0, 1, 1 },
	{ 182, 171, 3, 117, 0.0625f, 0.5f },
	{ 183, 171, 4, 119, 0.125f, 0.3f },
	{ 184, 171, 3, 119, 1, 0.1f },
	{ 185, 171, 4, 121, 0.03125f, 0.3f },
	{ 186, 171, 3, 121, 1, 0.2f },
	{ 187, 171, 3, 122, 0.0625f, 0.1f },
	{ 196, 171, 4, 124, 0.25f, 0.5f },
	{ 197, 171, 9, 0, 0.0625f, 0.25f },
	{ 125, 171, 11, 125, 0.25f, 0.3f },
	{ 126, 171, 10, 126, 0.25f, 1.5f },
	{ 181, 171, 3, 127, 0.5f, 0.5f },
	{ 128, 171, 3, 128, 0.0625f, 0.1f },
	{ 129, 171, 6, 129, 0.0625f, 0.1f },
	{ 130, 171, 6, 129, 0.0625f, 0.1f },
	{ 131, 171, 6, 129, 0.0625f, 0.1f },
	{ 132, 171, 6, 129, 0.0625f, 0.1f },
	{ 133, 171, 6, 129, 0.0625f, 0.1f },
	{ 134, 171, 6, 134, 0.0625f, 0.5f },
	{ 135, 171, 9, 0, 0.125f, 0.2f },
	{ 136, 171, 6, 136, 1, 0.5f },
	{ 137, 171, 3, 137, 1, 0.1f },
	{ 138, 171, 9, 0, 0.125f, 0.5f },
	{ 139, 171, 11, 38, 0.5f, 0.5f },
	{ 140, 171, 4, 15, 0.5f, 0.3f },
	{ 141, 171, 3, 141, 0.03125f, 0.1f },
	{ 142, 171, 4, 168, 0.03125f, 0.15f },
	{ 143, 171, 9, 0, 0.03125f, 0.25f },
	{ 144, 171, 4, 110, 1, 0.5f },
	{ 145, 171, 4, 111, 1, 0.5f },
	{ 146, 171, 6, 15, 1, 0.6f },
	{ 147, 171, 4, 110, 1, 0.5f },
	{ 148, 171, 4, 111, 1, 0.5f },
	{ 149, 171, 6, 15, 1, 0.6f },
	{ 150, 171, 10, 6, 0.1f, 0.3f },
	{ 151, 171, 10, 7, 0.1f, 0.3f },
	{ 152, 171, 10, 8, 0.1f, 0.3f },
	{ 153, 171, 11, 9, 0.2f, 0.3f },
	{ 154, 171, 11, 10, 0.2f, 0.15f },
	{ 155, 171, 3, 105, 0.25f, 0.6f },
	{ 156, 171, 4, 103, 1, 0.4f },
	{ 157, 171, 4, 0, 0.25f, 0.4f },
	{ 158, 171, 4, 68, 0.25f, 0.2f },
	{ 159, 171, 10, 100, 0.25f, 1.6f },
	{ 160, 171, 11, 101, 0.25f, 1.6f },
	{ 161, 171, 11, 102, 0.25f, 1.6f },
	{ 162, 171, 3, 103, 0.25f, 0.3f },
	{ 163, 171, 3, 163, 0.125f, 1 },
	{ 164, 171, 4, 165, 0.125f, 0.5f },
	{ 165, 171, 3, 165, 0.25f, 0.5f },
	{ 166, 171, 4, 167, 0.125f, 0.5f },
	{ 167, 171, 3, 167, 0.25f, 0.5f },
	{ 168, 171, 3, 168, 0.2f, 0.5f },
	{ 169, 171, 4, 171, 0.5f, 0.2f },
	{ 170, 171, 7, 0, 1, 0.2f },
	{ 171, 171, 3, 171, 0.5f, 0.1f },
	{ 172, 171, 3, 172, 1, 0.2f },
	{ 173, 171, 3, 172, 1, 0.2f },
	{ 174, 171, 6, 174, 0.0625f, 0.05f },
	{ 175, 171, 6, 175, 0.0625f, 0.05f },
	{ 176, 171, 6, 176, 1, 0.5f },
	{ 177, 171, 9, 0, 0.125f, 0.1f },
	{ 178, 171, 9, 0, 0.125f, 0.5f },
	{ 179, 171, 6, 179, 1, 0.5f },
	{ 180, 171, 3, 180, 1, 0.1f },
	{ 181, 171, 3, 181, 0.5f, 0.5f },
	{ 182, 171, 3, 182, 0.0625f, 0.5f },
	{ 183, 171, 4, 184, 0.125f, 0.3f },
	{ 184, 171, 3, 184, 1, 0.1f },
	{ 185, 171, 4, 186, 0.03125f, 0.3f },
	{ 186, 171, 3, 186, 1, 0.2f },
	{ 187, 171, 3, 187, 0.0625f, 0.1f },
	{ 188, 171, 6, 188, 0.25f, 0.2f },
	{ 189, 171, 3, 92, 0.25f, 0.3f },
	{ 190, 171, 4, 68, 0.25f, 0.3f },
	{ 191, 171, 9, 0, 0.25f, 0.2f },
	{ 192, 171, 3, 97, 0.25f, 0.3f },
	{ 193, 171, 4, 103, 0.25f, 0.5f },
	{ 194, 171, 9, 0, 0.25f, 0.2f },
	{ 195, 171, 4, 15, 0.25f, 0.5f },
	{ 196, 171, 4, 124, 0.25f, 0.25f },
	{ 197, 171, 9, 0, 0.0625f, 0.5f },
	{ 198, 171, 3, 75, 0.25f, 0.1f },
	{ 199, 171, 3, 38, 0.25f, 0.1f },
	{ 200, 171, 3, 32, 0.25f, 0.1f },
	{ 201, 171, 3, 42, 0.25f, 0.1f },
	{ 202, 171, 3, 45, 0.5f, 0.1f },
	{ 203, 171, 4, 47, 0.5f, 0.1f },
	{ 204, 171, 3, 47, 0.5f, 0.3f },
	{ 205, 190, 3, 205, 0.25f, 0.5f },
	{ 206, 190, 3, 206, 0.25f, 0.5f },
	{ 207, 190, 3, 207, 0.25f, 0.5f },
	{ 208, 190, 3, 208, 0.25f, 0.5f },
	{ 209, 190, 3, 209, 0.25f, 0.5f },
	{ 210, 190, 3, 210, 0.25f, 0.5f },
	{ 211, 171, 3, 211, 0.0625f, 0.1f },
	{ 212, 171, 3, 212, 0.0625f, 0.4f },
	{ 213, 171, 3, 213, 0.125f, 0.5f },
	{ 214, 171, 3, 214, 0.125f, 0.5f },
	{ 215, 171, 4, 211, 0.2f, 0.3f }, // Rouge - Fall to land on turtle [DRY LAGOON]
	{ 216, 171, 3, 216, 0.0625f, 0.2f }
};

PointerInfo pointers[] = {
	 ptrdecl(0x72856F, &RougeAnimationList), (0x716F0A, &SonicAnimationList),
};

////////////////////////////////////////////////////////////////////////////////////
// ROCKET MOMENTUM
////////////////////////////////////////////////////////////////////////////////////
void RocketShot()
{
	if (MainCharObj1[0]->Action == 0x12) // rocket, translate to X speed launch 'cause reasons
	{
		rocketOn = 1;
	}
	if ((rocketOn == 1) && (MainCharObj1[0]->Action != 0x12))
	{
		MainCharObj2[0]->Speed.x = 12; // set an custom value here up-to 20
		rocketOn = 0;
	}
}

///////////////////////////////////////////////////////
//	Dark Character Results Jingle, by "Kell", 2021	///
///////////////////////////////////////////////////////

Trampoline* PlayMusicOnce_t = nullptr;

void __cdecl PlayMusicOnce_original(const char* name)
{
	const auto PlayMusicOnce_ptr = PlayMusicOnce_t->Target();

	__asm
	{
		mov edi, name
		call PlayMusicOnce_ptr
	}
}

void __cdecl PlayMusicOnce_r(const char* name)
{
	if (TwoPlayerMode == 0 && (CurrentCharacter == Characters_Shadow || CurrentCharacter == Characters_Rouge || CurrentCharacter == Characters_MechEggman ||
		CurrentCharacter == Characters_Eggman || CurrentCharacter == Characters_DarkChaoWalker) && !strcmp(name, "rndclear.adx"))
	{
		PlayMusicOnce_original("rndclear_dark.adx");
	}
	else
	{
		PlayMusicOnce_original(name);
	}
}

static void __declspec(naked) PlayMusicOnce_asm()
{
	__asm
	{
		push edi
		call PlayMusicOnce_r
		pop edi
		retn
	}
}


//////////////////////////////////////
//	Footstep Audio by "Kell", 2022	//
//////////////////////////////////////

static Trampoline* Sonic_Main_t = nullptr;
static Trampoline* Knuckles_Main_t = nullptr;

static bool SoundPlayed[8] = { };

static MLTSoundEntry NewHuntersSndEntries[]{
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
	{ 15, 4, 0, 0, 0, 0 }, // Knuckles Walk Steps
	{ 15, 8, 0, 0, 0, 0 }, // Rouge Walk Footsteps
	{ 15, 9, 0, 0, 0, 0 }, // Rouge Run Footsteps
	{ 15, 11, 0, 0, 0, 0 }, // Rouge Big Land
	{ 2, 24, 0, 0, 0, 0 }, // Rouge Wing Flaps
};

static MLTSoundList NewHuntersSndList = { arraylengthandptrT(NewHuntersSndEntries, int) };

static MLTSoundEntry NewSpeedsSndEntries[]{
	{ 2, 0, 0, 0, 0, 0 },
	{ 2, 1, 0, 0, 0, 0 },
	{ 14, 15, 0, 0, 0, 0 }, // Brake
	{ 14, 14, 0, 0, 0, 0 }, // Land
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
	{ 14, 12, 0, 0, 0, 0 }, // Sonic/Shadow Walk Footsteps
	{ 14, 8, 0, 0, 0, 0 }, // Shadow JetShoe R
	{ 14, 9, 0, 0, 0, 0 }, // Shadow JetShoe L
	{ 14, 13, 0, 0, 0, 0,}, // Sonic/Shadow Run Footsteps
	{ 14, 11, 0, 0, 0, 0,}, // Shadow JetBrake (this plays with the regular brake sound)
};

static MLTSoundList NewSpeedsSndList = { arraylengthandptrT(NewSpeedsSndEntries, int) };

enum NewHuntersSounds
{
	HSnd_HuntersStep = 0x201E,
	HSnd_RougeWalkStep,
	HSnd_RougeRunStep,
	HSnd_RougeBigLand,
	Hsnd_RougeWingFlap,
};

enum NewSpeedsSounds
{
	SSnd_SpeedsStep = 0x201D,
	SSnd_ShadowAirR,
	SSnd_ShadowAirL,
	SSnd_RunStep,
	SSnd_ShadowJetBrake,
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

	PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeWalkStep, 6, 3.0f, 24.0f);
	PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeWalkStep, 7, 3.0f, 24.0f);
	PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeRunStep, 8, 11.0f, 36.0f);
	PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeRunStep, 9, 19.0f, 61.0f);
	PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeRunStep, 10, 19.0f, 611.0f);
	PlayFootSound(data, co2, Characters_Rouge, Hsnd_RougeWingFlap, 105, 5.0f, 18.0f);
	PlayFootSound(data, co2, Characters_Rouge, HSnd_RougeBigLand, 17, 1.0f, 2.0f);
}

void __cdecl Sonic_Main_r(ObjectMaster* obj)
{
	TRAMPOLINE(Sonic_Main)(obj);

	auto data = obj->Data1.Entity;
	auto co2 = obj->Data2.Character;

	PlayFootSound(data, co2, Characters_Sonic, SSnd_SpeedsStep, 6, 16.0f, 35.0f);
	PlayFootSound(data, co2, Characters_Sonic, SSnd_SpeedsStep, 7, 22.0f, 50.0f);
	PlayFootSound(data, co2, Characters_Sonic, SSnd_RunStep, 8, 23.0f, 61.0f);
	PlayFootSound(data, co2, Characters_Sonic, SSnd_RunStep, 9, 17.0f, 50.0f);
	PlayFootSound(data, co2, Characters_Sonic, SSnd_RunStep, 10, 15.0f, 41.0f);

	PlayFootSound(data, co2, Characters_Shadow, SSnd_SpeedsStep, 6, 16.0f, 35.0f);
	PlayFootSound(data, co2, Characters_Shadow, SSnd_RunStep, 7, 22.0f, 50.0f);
	PlayFootSound(data, co2, Characters_Shadow, SSnd_ShadowAirL, SSnd_ShadowAirR, 8, 1.0f, 6.0f);
	PlayFootSound(data, co2, Characters_Shadow, SSnd_ShadowAirL, SSnd_ShadowAirR, 9, 5.0f, 39.0f);
	PlayFootSound(data, co2, Characters_Shadow, SSnd_ShadowAirL, SSnd_ShadowAirR, 10, 14.0f, 52.0f);
	PlayFootSound(data, co2, Characters_Shadow, SSnd_ShadowJetBrake, 18, 1.0f, 2.0f);
}

///////////////////////////////////////////////////
///	Remove Character Jiggle, by "Prahaha" 2021	///
///////////////////////////////////////////////////

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
	HelperFunctions helperFunctionsG;
	int currentFile = 0;

	__declspec(dllexport) PointerList Pointers = { arrayptrandlengthT(pointers, int) };
	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
	__declspec(dllexport) void _cdecl Init(const char* path, const HelperFunctions& helperFunctions)

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

		//Trampoline* PlayMusicOnce_t = nullptr;
		PlayMusicOnce_t = new Trampoline(0x442EF0, 0x442EF7, PlayMusicOnce_asm, false);

	
		helperFunctionsG = helperFunctions;
		helperFunctionsG.ReplaceFile("resource\\gd_PC\\plcommtn_other.prs", "resource\\gd_PC\\plcommtn.prs");
	}

	__declspec(dllexport) void OnFrame()

	{	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	Individual Player_Common (plcommtn) animation implmentation by "Speeps" back in 2021
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//	This code allows each character to have their own common animations. Currently only works in 1P Mode and if each character is in their intended stages.
		//	E.g: Rouge in Dry Lagoon will work. Rouge in City Escape will make her use Sonic's common animations. This has yet to be resolved.
		//	This code also allows us to redirect the file locations of the MTN files. Useful for keeping things nice and tidy.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		switch (CurrentCharacter) {
		case Characters_Sonic:
			if (currentFile != 1) {
				helperFunctionsG.ReplaceFile("resource\\gd_PC\\plcommtn.prs", "resource\\gd_PC\\Animations\\Sonic_Common.prs");
				currentFile = 1;
			}
			break;
		case Characters_Shadow:
			if (currentFile != 2) {
				helperFunctionsG.ReplaceFile("resource\\gd_PC\\plcommtn.prs", "resource\\gd_PC\\Animations\\Shadow_Common.prs");
				currentFile = 2;
			}
			break;
		default:
			if (currentFile != 0) {
				helperFunctionsG.ReplaceFile("resource\\gd_PC\\plcommtn.prs", "resource\\gd_PC\\Animations\\Other_Common.prs");
				currentFile = 0;
			}
			break;
		case Characters_Rouge:
			if (currentFile != 3) {
				helperFunctionsG.ReplaceFile("resource\\gd_PC\\plcommtn.prs", "resource\\gd_PC\\Animations\\Rouge_Common.prs");
				currentFile = 3;
			}
		}
	}
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
;