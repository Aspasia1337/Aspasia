#pragma once

#include "../../globals.h"
#include "../../Classes/Classes.h"
#include "../../Interfaces/iManager.h"

#include "Materials.h"

#include <algorithm>
#include <unordered_map>

class CUtilsBuff
{
public:
	char pad[0x80];

	CUtilsBuff(int a1, int nSize, int a3);

	void PutString(const char *szString);
};

class Visual
{
public:
	static std::vector<C_EnvSky *> CEnvSkyVector;

	static ByteColor ToByteColor(float *floatColor);

	class LightningModulation
	{
	public:
		typedef void *(__fastcall *LightningModulationFunction)(__int64 a1, CAggregateSceneObject *a2, __int64 a3);
		static LightningModulationFunction oLightningModulation;
		static void *__fastcall hLightningModulation(__int64 a1, CAggregateSceneObject *a2, __int64 a3);
	};
	LightningModulation mLightningModulation;

	class SmokeRender
	{
	public:
		typedef void(__fastcall *RenderSmokeParticlesFunction)(__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6);
		static RenderSmokeParticlesFunction oRenderSmokeParticles;
		static void __fastcall hRenderSmoke(__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6);
	};
	SmokeRender m_SmokeEffect;

	class FlashEffect
	{
	public:
		typedef void(__fastcall *FlashEffectFunction)(__int64 a1, __int64 a2, float *a3);
		static FlashEffectFunction oFlashEffect;
		static void __fastcall hFlashEffect(__int64 a1, __int64 a2, float *a3);
	};
	FlashEffect m_FlashEffect;

	class DrawObjectClass
	{
	public:
		typedef void(__fastcall *DrawObjectFunction)(void *a1, void *a2, CMeshData *a3, int a4, void *a5, void *a6, void *a7, void *a8);
		static DrawObjectFunction oDrawObject;
		static void __fastcall hDrawObject(void *a1, void *a2, CMeshData *a3, int a4, void *a5, void *a6, void *a7, void *a8);
	};
	DrawObjectClass m_DrawObject;

	class UpdateSkybox
	{
	private:
		static ByteColor ColorCache;

	public:
		ByteColor ToByteColor(float *floatColor);
		void(__fastcall *UpdateSkyboxFunction)(C_EnvSky *);
		void ChangeSkybox();
	};
	UpdateSkybox m_UpdateSkybox;

	class WorldModulation
	{
	private:
		static ByteColor ColorCache;

	public:
		typedef void *(__fastcall *ModulateWorldColorFn)(CAggregateSceneObjectWorld *, void *);
		static ModulateWorldColorFn oModulateWorldColor;
		static void *__fastcall hModulateWorldColor(CAggregateSceneObjectWorld *pAggregateSceneObject, void *a2);
	};
	WorldModulation m_WorldModulation;

	class Chams
	{
	public:
		std::unordered_map<const char *, CustomMaterial_t> CustomMaterialMap;
		std::vector<const char *> MaterialInfo = {
			szVMatBufferWhiteVisible, szVMatBufferWhiteInvisible,
			szVMatBufferIlluminateVisible, szVMatBufferIlluminateInvisible,
			szVMatBufferIlatex, szVMatBufferIlatexInvisible,
			szVMatBufferMetalic, szVMatBufferMetalicInvisible,
			szVMatBufferGlowVisible, szVMatBufferGlowInvisible,
			szBloomVmatBuffer, szInvisBloomVmatBuffer};

		void InitChams();
		CMaterial2 *CreateMaterial(const char *szMaterialName, const char szVmatBuffer[]);
		static bool LoadKV3(CUtilsBuff *buff, CKeyValues3 *CkeyVal);
		static CKeyValues3 *CreateMaterialResource();

		static CKeyValues3 *(__fastcall *SetTypeKV3)(CKeyValues3 *, int, unsigned int);
		static bool(__fastcall *LoadKeyValues)(CKeyValues3 *, void *, CUtilsBuff *buffer, KV3ID_t *, void *, void *, void *, void *, const char *);
		static int64_t(__fastcall *CreateMaterialFunction)(void *, void *, const char *, void *, unsigned int, unsigned int);

		static CUtilsBuff *(__fastcall *BuffInit)(CUtilsBuff *, int a1, int nSize, int a3);
		static void(__fastcall *BuffPutString)(CUtilsBuff *, const char *);
	};
	Chams m_Chams;

	class OverlayRender
	{
	public:
		static void RenderHealth(C_PlayerPawn *Player);

		static void RenderPlayerBones(C_PlayerPawn *Player);

		static void RenderSkeleton(C_PlayerPawn *Player);

		void RenderAllPlayerVisuals(C_PlayerPawn *Player);
	};

	OverlayRender m_OverlayRender;
};

inline Visual *iVisual = new Visual();
