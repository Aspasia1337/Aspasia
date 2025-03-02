#include "Visual.h"

void* Visual::LightningModulation::hLightningModulation (__int64 a1, CAggregateSceneObject *a2, __int64 a3) {

	return oLightningModulation (a1, a2, a3);
}

void Visual::SmokeRender::hRenderSmoke (__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6) {

	if (Globals::RenderSmokeHook) {
		return;
	}
	return oRenderSmokeParticles (a1, a2, a3, a4, a5, a6);
}	

void Visual::FlashEffect::hFlashEffect (__int64 a1, __int64 a2, float *a3) {

	if (Globals::RenderFlashHook) {
		return;
	}
	return oFlashEffect (a1, a2, a3);
}

void Visual::DrawObjectClass::hDrawObject (void *a1, void *a2, void *a3, int a4, void *a5, void *a6, void *a7, void *a8) {

	return oDrawObject (a1, a2, a3, a4, a5, a6, a7, a8);
}