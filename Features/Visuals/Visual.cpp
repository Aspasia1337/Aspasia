#include "Visual.h"
#include "Materials.h"
#include "../../EntityManager/EntityManager.h"

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



void Visual::DrawObjectClass::hDrawObject (void *pAnimatableSceneObjectDesc, void *pDx11, CMeshData *arrMeshDraw, int nDataCount, void *pSceneView, void *pSceneLayer, void *pUnk, void *pUnk2)
{
	if (Globals::Chams) {
		if (!arrMeshDraw) {
			return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		// Crear una copia local del puntero a SceneAnimatableObject para evitar cambios concurrentes
		auto *sceneObject = arrMeshDraw->SceneAnimatableObject;
		if (!sceneObject) {
			return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		auto *object = &arrMeshDraw->SceneAnimatableObject->hOwner;

		if (object == nullptr) {
			return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		auto *index = &arrMeshDraw->SceneAnimatableObject->hOwner.nIndex;

		if (index == nullptr) {
			return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		CBaseHandle hOwner = sceneObject->hOwner;

		auto pEntity = iGameEntitySystem->GetEntityByIndexFunction (hOwner.nIndex & 0x7FFF);

		if (pEntity == nullptr) {
			return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}
		auto schemaName = iGameEntitySystem->GetSchemaName (pEntity);

		//iHelper->m_Console.printMessage (WARNING, schemaName);

		if (std::strcmp (schemaName.c_str ( ), std::string ("C_CSPlayerPawnBase").c_str ( )) == 0 && Globals::PlayersChams) {
			ByteColor Color = ToByteColor (Globals::ChamsColor);
			//if (pEntity == iGameEntitySystem->GetPlayerPawn ( )) {
				//if (strcmp (arrMeshDraw->CMaterial->GetName ( ), "characters/models/shared/arms/glove_hardknuckle/materials/glove_hardknuckle_black.vmat") == 0)
				//{
			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at (Globals::MaterialNames[Globals::MaterialSelected]).pMaterial;
					
					*(byte*)((uintptr_t)arrMeshDraw+ 0x50) = Color.r;
					*(byte*)((uintptr_t)arrMeshDraw+ 0x51) = Color.g;
					*(byte*)((uintptr_t)arrMeshDraw+ 0x52) = Color.b;
					*(byte*)((uintptr_t)arrMeshDraw+ 0x53) = (byte)255;

			//}
		}
		if (std::strcmp (schemaName.c_str ( ), std::string ("C_PredictedViewModel").c_str ( )) == 0 && Globals::WeaponChams) {

			ByteColor Color = ToByteColor (Globals::ChamsColorGun);
			//if (pEntity == iGameEntitySystem->GetPlayerPawn ( )) {
				//if (strcmp (arrMeshDraw->CMaterial->GetName ( ), "characters/models/shared/arms/glove_hardknuckle/materials/glove_hardknuckle_black.vmat") == 0)
				//{
			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at (Globals::MaterialNames[Globals::MaterialGunSelected]).pMaterial;

			*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
			*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
			*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
			*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;

		}

	}
	return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
}



void OverrideMaterial (void *pAnimatableSceneObjectDesc, void *pDx11, CMeshData *arrMeshDraw, int nDataCount, void *pSceneView, void *pSceneLayer, void *pUnk, void *pUnk2) {



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

	ByteColor Color = Visual::ToByteColor (Globals::worldModulationColor);

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






void Visual::Chams::InitChams ( )
{
	const char *MaterialNames [5] = {"WhiteMaterial", "Illuminate", "Latex", "Metalic" , "Glow"};

	int i = 0;
	for (auto &MaterialName : MaterialNames) {
		CustomMaterialMap.emplace (MaterialName, CustomMaterial_t (CreateMaterial ("materials/dev/glowproperty.vmat", MaterialInfo[i]),
			CreateMaterial ("materials/dev/glowproperty.vmat", MaterialInfo[i+1])));
		i += 2;
	}

}

CMaterial2 *Visual::Chams::CreateMaterial (const char *szMaterialName, const char szVmatBuffer[])
{
	CKeyValues3 *pKeyValue = CreateMaterialResource ( );
	if (!pKeyValue || !szVmatBuffer) return nullptr;  

	CUtilsBuff buffer (0, strlen (szVmatBuffer) + 10, 1);

	buffer.PutString (szVmatBuffer);

	LoadKV3 (&buffer,pKeyValue);

	CMaterial2 *pCustomMate = {};

	CreateMaterialFunction (nullptr, &pCustomMate, szMaterialName, pKeyValue, 0, 1);

	return pCustomMate;
}

CKeyValues3 *Visual::Chams::CreateMaterialResource ( )
{
	CKeyValues3 *pKeyValue = new CKeyValues3[0x10];
	return SetTypeKV3 (pKeyValue, 1U, 6U);
}

bool Visual::Chams::LoadKV3 (CUtilsBuff *buff, CKeyValues3 *CkeyVal) 
{

	KV3ID_t kv3ID = KV3ID_t ("generic", 0x41B818518343427E, 0xB5F447C23C0CDF8C);

	return LoadKeyValues (CkeyVal, nullptr, buff, &kv3ID, nullptr, nullptr, nullptr, nullptr,"");
}

CUtilsBuff::CUtilsBuff (int a1, int nSize, int a3)
{
	iVisual->m_Chams.BuffInit (this, a1, nSize, a3);
}

void CUtilsBuff::PutString (const char *szString)
{
	iVisual->m_Chams.BuffPutString (this, szString);
}
