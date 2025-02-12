#pragma once


class HooksManager
{
public:

	
	HooksManager ( ) {
		initHook ( );
	}

	bool initHook ( );



	bool CreateHook (uint8_t *targetAddress, void *hookFunction, void **originalFunction, const char *hookName);

	// void __fastcall RenderSmoke(__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6)
	class SmokeEffect {
	public:
		typedef void (__fastcall *RenderSmokeParticlesFunction)(__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6);
		static RenderSmokeParticlesFunction oRenderSmokeParticles;
		static void __fastcall hRenderSmoke (__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6);
	};
	SmokeEffect m_SmokeEffect;

	class FlashEffect {
	public:
		typedef void (__fastcall *FlashEffectFunction)(__int64 a1, __int64 a2, float* a3);
		static FlashEffectFunction oFlashEffect;
		static void __fastcall hFlashEffect (__int64 a1, __int64 a2, float *a3);
	};
	FlashEffect m_FlashEffect;

	//void __fastcall createMove(__int64 *a1, int a2, char a3)
	class CreateMove {

	public:
		typedef void (__fastcall *CreateMoveFunction)(__int64 *a1, __int64 a2, float *a3);
		static CreateMoveFunction oCreateMove;
		static void __fastcall hCreateMove (__int64 *a1, __int64 a2, float *a3);

	};
	CreateMove m_CreateMove;

};

inline HooksManager *hookMgr = new HooksManager ( );
