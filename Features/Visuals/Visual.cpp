#include "Visual.h"

ByteColor Visual::WorldModulation::ColorCache;
ByteColor Visual::UpdateSkybox::ColorCache;


void *Visual::LightningModulation::hLightningModulation (__int64 a1, CAggregateSceneObject *CAgregateSceneObject, __int64 a3) {

	CAgregateSceneObject->RedColor = Globals::lightModulationColor[0] * Globals::lightIntensity;
	CAgregateSceneObject->GreenColor = Globals::lightModulationColor[1] * Globals::lightIntensity;
	CAgregateSceneObject->BlueColor = Globals::lightModulationColor[2] * Globals::lightIntensity;

	return oLightningModulation (a1, CAgregateSceneObject, a3);
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



void Visual::DrawObjectClass::hDrawObject (void *a1, void *a2, CMeshData *arrayMeshData, int a4, void *a5, void *a6, void *a7, void *a8) {

	//CBaseHandle hOwner = arrayMeshData->SceneAnimatableObject->hOwner;

	//if (hOwner.nIndex == 0xFFFFFFFF)
	//	return;

	//auto pEntity = iGameEntitySystem->GetEntityByIndexFunction (hOwner.nIndex);
	//
	//if (pEntity == nullptr) {
	//	return;
	//}

	//auto schemaName = iGameEntitySystem->GetSchemaName (pEntity);
	//
	//if (std::strcmp (schemaName.c_str ( ), std::string ("C_CSPlayerPawn").c_str( )) == 0) {
	//	return;
	//}

	//iHelper->m_Console.printMessage (WARNING, "a");

	return oDrawObject (a1, a2, arrayMeshData, a4, a5, a6, a7, a8);
}


void OverrideMaterial (void *pAnimatableSceneObjectDesc, void *pDx11, CMeshData *arrMeshDraw, int nDataCount, void *pSceneView, void *pSceneLayer, void *pUnk, void *pUnk2) {



}

Visual::Visual ( )
{
}

ByteColor Visual::ToByteColor (float *floatColor) {
	return ByteColor (
		static_cast<unsigned char>(std::clamp (floatColor[0], 0.0f, 1.0f) * 255.0f + 0.5f),
		static_cast<unsigned char>(std::clamp (floatColor[1], 0.0f, 1.0f) * 255.0f + 0.5f),
		static_cast<unsigned char>(std::clamp (floatColor[2], 0.0f, 1.0f) * 255.0f + 0.5f)
	);
}




void Visual::UpdateSkybox::ChangeSkybox ( )
{
	ByteColor Color = Visual::ToByteColor (Globals::SkyTintColor);

	if (Globals::ChangeSkyColor && Color != ColorCache) {
		for (auto &sky : CEnvSkyVector) {

			unsigned char *p = reinterpret_cast<unsigned char *>(&sky->m_vTintColor);

			p[0] = Color.r;
			p[1] = Color.g;
			p[2] = Color.b;
			p[3] = 255;

			sky->m_flBrightnessScale = 1.f;
			iVisual->m_UpdateSkybox.UpdateSkyboxFunction (sky);
		}
		ColorCache = Color;
	}
}

void *Visual::WorldModulation::hModulateWorldColor (CAggregateSceneObjectWorld *pAggregateSceneObject, void *a2) {

	ByteColor Color = ToByteColor (Globals::worldModulationColor);

	if (Globals::worldModulation && Color != ColorCache) {
		int count = pAggregateSceneObject->count;
		for (int i = 0; i < count; i++) {
			CAggregateSceneObjectDataWorld *pAggregateSceneObjectData = &pAggregateSceneObject->array[i];

			pAggregateSceneObjectData->r = Color.r;
			pAggregateSceneObjectData->g = Color.g;
			pAggregateSceneObjectData->b = Color.b;

			ColorCache = Color;
		}
	}
	return oModulateWorldColor (pAggregateSceneObject, a2);
}