#pragma once

#include "../../globals.h"
#include "../../Classes/Classes.h"
#include <algorithm>

class Visual
{
public:


	static std::vector<C_EnvSky *> CEnvSkyVector;


	Visual ( );

	static ByteColor ToByteColor (float *floatColor);

	class LightningModulation {
	public:
		typedef void *(__fastcall *LightningModulationFunction)(__int64 a1, CAggregateSceneObject *a2, __int64 a3);
		static LightningModulationFunction oLightningModulation;
		static void *__fastcall hLightningModulation (__int64 a1, CAggregateSceneObject *a2, __int64 a3);
	};
	LightningModulation mLightningModulation;

	class SmokeRender {
	public:
		typedef void (__fastcall *RenderSmokeParticlesFunction)(__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6);
		static RenderSmokeParticlesFunction oRenderSmokeParticles;
		static void __fastcall hRenderSmoke (__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6);
	};
	SmokeRender m_SmokeEffect;

	class FlashEffect {
	public:
		typedef void (__fastcall *FlashEffectFunction)(__int64 a1, __int64 a2, float *a3);
		static FlashEffectFunction oFlashEffect;
		static void __fastcall hFlashEffect (__int64 a1, __int64 a2, float *a3);
	};
	FlashEffect m_FlashEffect;

	class DrawObjectClass {
	public:
		typedef void (__fastcall *DrawObjectFunction)(void *a1, void *a2, CMeshData *a3, int a4, void *a5, void *a6, void *a7, void *a8);
		static DrawObjectFunction oDrawObject;
		static void __fastcall hDrawObject (void *a1, void *a2, CMeshData *a3, int a4, void *a5, void *a6, void *a7, void *a8);
	};
	DrawObjectClass m_DrawObject;

	class CreateMaterial {
	public:

		int (__fastcall *CreateMaterialFunction)(void *, void *, const char *, void *, unsigned int, unsigned int);

	};
	CreateMaterial m_CreateMaterial;


	class SetTypeKV3 {
	public:

		int (__fastcall *SetTypeKV3)(void *, unsigned int, unsigned int);

	};
	SetTypeKV3 m_SetTypeKV3;

	class UpdateSkybox {
	private:
		static ByteColor ColorCache;

	public:
		ByteColor ToByteColor (float *floatColor);
		void (__fastcall *UpdateSkyboxFunction)(C_EnvSky *);
		void ChangeSkybox ( );
	};
	UpdateSkybox m_UpdateSkybox;

	class WorldModulation
	{
	private:
		static ByteColor ColorCache;

	public:
		typedef void *(__fastcall *ModulateWorldColorFn)(CAggregateSceneObjectWorld *, void *);
		static ModulateWorldColorFn oModulateWorldColor;
		static void* __fastcall hModulateWorldColor (CAggregateSceneObjectWorld *pAggregateSceneObject, void *a2);
	};

	WorldModulation m_WorldModulation;

};

inline Visual *iVisual = new Visual ( );

