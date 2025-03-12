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
void* Visual::DrawObjectClass::hDrawObject (void *pAnimatableSceneObjectDesc, void *pDx11, CMeshData *arrMeshDraw, int nDataCount, void *pSceneView, void *pSceneLayer, void *pUnk, void *pUnk2)
{

	if (!arrMeshDraw) {
		return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
	}

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

	// Obtener una copia segura del propietario antes de continuar
	CBaseHandle hOwner = sceneObject->hOwner;

	auto pEntity = iGameEntitySystem->GetEntityByIndexFunction (hOwner.nIndex & 0x7FFF);
	
	if (pEntity == nullptr) {
		return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
	}
	auto schemaName = iGameEntitySystem->GetSchemaName (pEntity);

	
	if (std::strcmp (schemaName.c_str ( ), std::string ("C_CSPlayerPawnBase").c_str( )) == 0) {

		if (pEntity == iGameEntitySystem->GetPlayerPawn()) {
			if (strcmp(arrMeshDraw->CMaterial->GetName ( ) , "characters/models/shared/arms/glove_hardknuckle/materials/glove_hardknuckle_black.vmat")==0)
			{
				arrMeshDraw->CMaterial = iVisual->m_Chams.Material.pMaterial;
			}


			//byte *p = &arrMeshDraw->colVal;
			return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);

		}


	}

	return oDrawObject (pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
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


static constexpr char szVMatBufferGlow2Visible[] =
R"(<!-- kv3 encoding:text:version{e21c7f3c-8a33-41c5-9977-a76d3a32aa0d}
            format:generic:version{7412167c-06e9-4698-aff2-e63eb59037e7} -->
            {
                shader = "csgo_effects.vfx"
                g_tColor = resource:"materials/dev/primary_white_color_tga_21186c76.vtex"
                g_tNormal = resource:"materials/default/default_normal_tga_7652cb.vtex"
                g_tMask1 = resource:"materials/default/default_mask_tga_344101f8.vtex"
                g_tMask2 = resource:"materials/default/default_mask_tga_344101f8.vtex"
                g_tMask3 = resource:"materials/default/default_mask_tga_344101f8.vtex"
                g_flOpacityScale = 0.45
                g_flFresnelExponent = 0.75
                g_flFresnelFalloff = 1
                g_flFresnelMax = 0.0
                g_flFresnelMin = 1
                F_ADDITIVE_BLEND = 1
                F_BLEND_MODE = 1	
                F_TRANSLUCENT = 1
                F_IGNOREZ = 0
                F_DISABLE_Z_WRITE = 0
                F_DISABLE_Z_BUFFERING = 0
                F_RENDER_BACKFACES = 1
                g_vColorTint = [1.0, 1.0, 1.0, 0.0]
})";



void Visual::Chams::InitChams ( )
{
	 Material = CustomMaterial_t{ .pMaterial = CreateMaterial ("materials/dev/glowproperty.vmat",szVMatBufferGlow2Visible),
		.pMaterialVisible = CreateMaterial ("materials/dev/glowproperty.vmat",szVMatBufferGlow2Visible)
	};

}

CMaterial2 *Visual::Chams::CreateMaterial (const char *szMaterialName, const char szVmatBuffer[])
{
	CKeyValues3 *pKeyValue = CreateMaterialResource ( );
	if (!pKeyValue || !szVmatBuffer) return nullptr;  

	CUtilsBuff* buff = iVisual->m_Chams.BuffInit (0, strlen (szVmatBuffer) + 10, 1);
	iVisual->m_Chams.BuffPutString (buff, szVmatBuffer);
	LoadKV3 (buff,pKeyValue);

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
