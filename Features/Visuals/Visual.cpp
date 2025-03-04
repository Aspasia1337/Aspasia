#include "Visual.h"

void* Visual::LightningModulation::hLightningModulation (__int64 a1, CAggregateSceneObject *CAgregateSceneObject, __int64 a3) {

	CAgregateSceneObject->RedColor = Globals::lightModulationColor[0] * Globals::lightIntensity ;
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



void Visual::DrawObjectClass::hDrawObject (void *a1, void *a2, CMeshData * arrayMeshData, int a4, void *a5, void *a6, void *a7, void *a8) {


	

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
