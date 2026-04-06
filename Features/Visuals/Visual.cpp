#include "Visual.h"
#include "../../Includes/imgui/imgui.h"
#include "../../EntityManager/EntityManager.h"
#include "../../Math/vector.h"

#include <algorithm>

ByteColor Visual::WorldModulation::ColorCache;
ByteColor Visual::UpdateSkybox::ColorCache;

void *Visual::LightningModulation::hLightningModulation(__int64 a1, CAggregateSceneObject *CAgregateSceneObject, __int64 a3)
{
	CAgregateSceneObject->RedColor = Globals::lightModulationColor[0] * Globals::lightIntensity;
	CAgregateSceneObject->GreenColor = Globals::lightModulationColor[1] * Globals::lightIntensity;
	CAgregateSceneObject->BlueColor = Globals::lightModulationColor[2] * Globals::lightIntensity;

	return oLightningModulation(a1, CAgregateSceneObject, a3);
}

void Visual::SmokeRender::hRenderSmoke(__int64 a1, __int64 a2, int a3, int a4, __int64 a5, __int64 a6)
{

	if (Globals::RenderSmokeHook)
	{
		return;
	}
	return oRenderSmokeParticles(a1, a2, a3, a4, a5, a6);
}

void Visual::FlashEffect::hFlashEffect(__int64 a1, __int64 a2, float *a3)
{

	if (Globals::RenderFlashHook)
	{
		return;
	}
	return oFlashEffect(a1, a2, a3);
}

void Visual::DrawObjectClass::hDrawObject(void *pAnimatableSceneObjectDesc, void *pDx11, CMeshData *arrMeshDraw, int nDataCount, void *pSceneView, void *pSceneLayer, void *pUnk, void *pUnk2)
{
	if (1)
	{
		if (!arrMeshDraw)
		{
			return oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		auto *sceneObject = arrMeshDraw->SceneAnimatableObject;
		if (!sceneObject)
		{
			return oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		auto *object = &arrMeshDraw->SceneAnimatableObject->hOwner;

		if (object == nullptr)
		{
			return oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		auto *index = &arrMeshDraw->SceneAnimatableObject->hOwner.nIndex;

		if (index == nullptr)
		{
			return oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		CBaseHandle hOwner = sceneObject->hOwner;

		auto pEntity = iGameEntitySystem->GetEntityByIndexFunction(hOwner.nIndex & 0x7FFF);

		if (pEntity == nullptr)
		{
			return oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
		}

		auto schemaName = iGameEntitySystem->GetSchemaName(pEntity);

		// Menu MODEL
		if (std::strcmp(schemaName.c_str(), std::string("C_CSPlayerPawn").c_str()) == 0)
		{
			ByteColor Color = ToByteColor(Globals::ChamsColor);

			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at(Globals::MaterialNames[Globals::MaterialSelected]).pMaterialVisible;

			*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
			*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
			*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
			*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;

			Color = ToByteColor(Globals::ChamsColorGun);
			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at(Globals::MaterialNames[Globals::MaterialSelected]).pMaterial;

			*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
			*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
			*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
			*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;
		}

		// In Game Player Model
		if ((std::strcmp(schemaName.c_str(), std::string("C_CSPlayerPawnBase").c_str()) == 0 ||
			 std::strcmp(schemaName.c_str(), std::string("c_cs_player_for_precache").c_str()) == 0 ||
			 std::strcmp(schemaName.c_str(), std::string("CBasePlayerController").c_str()) == 0) &&
			Globals::PlayersChams)
		{
			ByteColor Color;

			if (Globals::InvisibleChams)
			{
				Color = ToByteColor(Globals::InvisibleChamsColor);

				arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at(Globals::MaterialNames[Globals::MaterialSelected]).pMaterialVisible;

				*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
				*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
				*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
				*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;

				oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
			}

			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at(Globals::MaterialNames[Globals::MaterialSelected]).pMaterial;

			Color = ToByteColor(Globals::ChamsColor);

			*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
			*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
			*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
			*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;
		}

		// Player Gun
		if (std::strcmp(schemaName.c_str(), std::string("C_PredictedViewModel").c_str()) == 0 && Globals::WeaponChams)
		{

			ByteColor Color = ToByteColor(Globals::ChamsColorGun);

			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at(Globals::MaterialNames[Globals::MaterialGunSelected]).pMaterial;

			*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
			*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
			*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
			*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;

			oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);

			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at(Globals::MaterialNames[Globals::MaterialGunSelected]).pMaterial;

			Color = ToByteColor(Globals::ChamsColorGun);

			*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
			*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
			*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
			*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;
		}

		// Arms and Gloves
		if (std::strcmp(schemaName.c_str(), std::string("CBaseAnimGraph").c_str()) == 0 && Globals::ArmsChams)
		{
			ByteColor Color = ToByteColor(Globals::ArmsColor);

			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at(Globals::MaterialNames[Globals::MaterialArmSelected]).pMaterial;

			*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
			*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
			*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
			*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;

			oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);

			arrMeshDraw->CMaterial = *(CMaterial2 **)iVisual->m_Chams.CustomMaterialMap.at(Globals::MaterialNames[Globals::MaterialArmSelected]).pMaterial;

			Color = ToByteColor(Globals::ArmsColor);

			*(byte *)((uintptr_t)arrMeshDraw + 0x50) = Color.r;
			*(byte *)((uintptr_t)arrMeshDraw + 0x51) = Color.g;
			*(byte *)((uintptr_t)arrMeshDraw + 0x52) = Color.b;
			*(byte *)((uintptr_t)arrMeshDraw + 0x53) = (byte)255;
		}
	}
	return oDrawObject(pAnimatableSceneObjectDesc, pDx11, arrMeshDraw, nDataCount, pSceneView, pSceneLayer, pUnk, pUnk2);
}

void OverrideMaterial(void *pAnimatableSceneObjectDesc, void *pDx11, CMeshData *arrMeshDraw, int nDataCount, void *pSceneView, void *pSceneLayer, void *pUnk, void *pUnk2)
{
}

ByteColor Visual::ToByteColor(float *floatColor)
{
	return ByteColor(
		static_cast<unsigned char>(std::clamp(floatColor[0], 0.0f, 1.0f) * 255.0f + 0.5f),
		static_cast<unsigned char>(std::clamp(floatColor[1], 0.0f, 1.0f) * 255.0f + 0.5f),
		static_cast<unsigned char>(std::clamp(floatColor[2], 0.0f, 1.0f) * 255.0f + 0.5f));
}

void Visual::UpdateSkybox::ChangeSkybox()
{
	ByteColor Color = Visual::ToByteColor(Globals::SkyTintColor);

	if (Globals::ChangeSkyColor && Color != ColorCache)
	{
		for (auto &sky : CEnvSkyVector)
		{

			unsigned char *p = reinterpret_cast<unsigned char *>(&sky->m_vTintColor);

			p[0] = Color.r;
			p[1] = Color.g;
			p[2] = Color.b;
			p[3] = 255;

			sky->m_flBrightnessScale = 1.f;
			iVisual->m_UpdateSkybox.UpdateSkyboxFunction(sky);
		}
		ColorCache = Color;
	}
}

void *Visual::WorldModulation::hModulateWorldColor(CAggregateSceneObjectWorld *pAggregateSceneObject, void *a2)
{

	ByteColor Color = Visual::ToByteColor(Globals::worldModulationColor);

	if (Globals::worldModulation && Color != ColorCache)
	{
		int count = pAggregateSceneObject->count;
		for (int i = 0; i < count; i++)
		{
			CAggregateSceneObjectDataWorld *pAggregateSceneObjectData = &pAggregateSceneObject->array[i];

			pAggregateSceneObjectData->r = Color.r;
			pAggregateSceneObjectData->g = Color.g;
			pAggregateSceneObjectData->b = Color.b;

			ColorCache = Color;
		}
	}
	return oModulateWorldColor(pAggregateSceneObject, a2);
}

void Visual::Chams::InitChams()
{
	const char *MaterialNames[6] = {"WhiteMaterial", "Illuminate", "Latex", "Metalic", "Glow", "Blom"};

	int i = 0;
	for (auto &MaterialName : MaterialNames)
	{
		CustomMaterialMap.emplace(MaterialName, CustomMaterial_t(CreateMaterial("materials/dev/glowproperty.vmat", MaterialInfo[i]),
																 CreateMaterial("materials/dev/glowproperty.vmat", MaterialInfo[i + 1])));
		i += 2;
	}
}

CMaterial2 *Visual::Chams::CreateMaterial(const char *szMaterialName, const char szVmatBuffer[])
{
	CKeyValues3 *pKeyValue = CreateMaterialResource();
	if (!pKeyValue || !szVmatBuffer)
		return nullptr;

	CUtilsBuff buffer(0, strlen(szVmatBuffer) + 10, 1);

	buffer.PutString(szVmatBuffer);

	LoadKV3(&buffer, pKeyValue);

	CMaterial2 *pCustomMate = {};

	CreateMaterialFunction(nullptr, &pCustomMate, szMaterialName, pKeyValue, 0, 1);

	return pCustomMate;
}

CKeyValues3 *Visual::Chams::CreateMaterialResource()
{
	CKeyValues3 *pKeyValue = new CKeyValues3[0x10];
	return SetTypeKV3(pKeyValue, 1U, 6U);
}

bool Visual::Chams::LoadKV3(CUtilsBuff *buff, CKeyValues3 *CkeyVal)
{
	KV3ID_t kv3ID = KV3ID_t("generic", 0x41B818518343427E, 0xB5F447C23C0CDF8C);
	return LoadKeyValues(CkeyVal, nullptr, buff, &kv3ID, nullptr, nullptr, nullptr, nullptr, "");
}

CUtilsBuff::CUtilsBuff(int a1, int nSize, int a3)
{
	iVisual->m_Chams.BuffInit(this, a1, nSize, a3);
}

void CUtilsBuff::PutString(const char *szString)
{
	iVisual->m_Chams.BuffPutString(this, szString);
}

void Visual::OverlayRender::RenderAllPlayerVisuals(C_PlayerPawn *Player)
{
	if (!Player || Player == iGameEntitySystem->GetPlayerPawn() || Player->pawnHealth <= 0 || Player->isAlive || Player->m_pGameSceneNode->m_bDormant)
		return;

	ImDrawList *drawList = ImGui::GetForegroundDrawList();
	if (!drawList)
		return;

	Vec3 worldPos = Player->vOldOrigin;
	Vec2 screenPos;

	if (!worldPos.WorldToScreen(screenPos, iGameEntitySystem->ViewMatrix))
		return;

	// 1. Render Health Bar
	if (Globals::HealthBar)
	{
		Vec3 localPlayerPos = iGameEntitySystem->GetPlayerPawn()->vOldOrigin;
		double distance = CalculateDistance(localPlayerPos, worldPos);

		float minDistance = 100.0f;
		float maxDistance = 1000.0f;
		float scale = 1.0f - (distance - minDistance) / (maxDistance - minDistance);
		scale = std::clamp(scale, 0.3f, 1.0f);

		float width = 60.0f * scale;
		float height = 3.0f * scale;
		float offsetY = 12.0f * scale;

		int health = Player->pawnHealth;
		float healthPerc = static_cast<float>(health) / 100.0f;

		ImU32 healthColor = IM_COL32(
			(1.0f - healthPerc) * 200,
			healthPerc * 230,
			50, 255);

		ImVec2 pos(screenPos.x - width / 2, screenPos.y + offsetY);

		drawList->AddRectFilled(
			ImVec2(pos.x - 0.5f, pos.y - 0.5f),
			ImVec2(pos.x + width + 0.5f, pos.y + height + 0.5f),
			IM_COL32(30, 30, 30, 180));

		drawList->AddRectFilled(
			ImVec2(pos.x, pos.y),
			ImVec2(pos.x + (width * healthPerc), pos.y + height),
			healthColor);
	}

	// 2. Render Weapon Name 
	// I don't really know why but sometimes it crashes. Need to be checked.
	if (Globals::PlayerInfo)
	{
		const char *weaponName = "??";

		if (Player && Player->m_pClippingWeapon && Player->m_pClippingWeapon->m_pEntity)
		{
			weaponName = Player->m_pClippingWeapon->m_pEntity->weapon_name;
		}

		float fontSize = 12.0f;

		ImFont *font = ImGui::GetFont();
		ImVec2 textSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, weaponName);

		ImVec2 textPos = ImVec2(screenPos.x - textSize.x / 2.0f, screenPos.y);

		ImU32 softColor = IM_COL32(100, 200, 180, 160);

		drawList->AddText(font, fontSize, textPos, softColor, weaponName);
	}

	// 3. Render Bone IDs (colorful text at bone positions)
	if (Globals::RenderPlayerBones)
	{
		for (int bone = terroristBones::pelvis; bone <= terroristBones::leg_upper_r_twist1; ++bone)
		{
			Vec2 boneScreen;
			if (!Player->GetBone(bone)->vecPosition.WorldToScreen(boneScreen, iGameEntitySystem->ViewMatrix))
				continue;

			char buffer[20];
			snprintf(buffer, sizeof(buffer), "%d", bone);

			ImU32 boneColor = IM_COL32(
				(bone * 15) % 255,
				(bone * 35) % 255,
				(bone * 55) % 255,
				255);

			drawList->AddText(ImVec2(boneScreen.x, boneScreen.y), boneColor, buffer);
		}
	}

	// 4. Render Skeleton
	if (Globals::RenderSkeleton)
	{
		auto &boneMap = Globals::boneMap;
		std::unordered_map<const char *, ImVec2> boneScreenPositions;

		for (const auto &bone : boneMap)
		{
			Vec2 bonePos;
			if (Player->GetBone(bone.second) &&
				Player->GetBone(bone.second)->vecPosition.WorldToScreen(bonePos, iGameEntitySystem->ViewMatrix))
			{
				boneScreenPositions[bone.first] = ImVec2(bonePos.x, bonePos.y);
			}
		}

		std::vector<std::pair<const char *, const char *>> boneConnections = {
			{"Head", "Neck"}, {"Neck", "Torso"}, {"Torso", "Pelvis"}, {"Torso", "Left Clavicle"}, {"Left Clavicle", "Left Upper Arm"}, {"Left Upper Arm", "Left Lower Arm"}, {"Left Lower Arm", "Left Hand"}, {"Torso", "Right Clavicle"}, {"Right Clavicle", "Right Upper Arm"}, {"Right Upper Arm", "Right Lower Arm"}, {"Right Lower Arm", "Right Hand"}, {"Pelvis", "Left Upper Leg"}, {"Left Upper Leg", "Left Lower Leg"}, {"Left Lower Leg", "Left Feet"}, {"Pelvis", "Right Upper Leg"}, {"Right Upper Leg", "Right Lower Leg"}, {"Right Lower Leg", "Right Feet"}};

		std::vector<ImU32> boneColors = {
			IM_COL32(255, 255, 255, 255), IM_COL32(255, 200, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 0, 0, 255),
			IM_COL32(255, 0, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 0, 0, 255), IM_COL32(0, 255, 0, 255),
			IM_COL32(0, 255, 0, 255), IM_COL32(0, 255, 0, 255), IM_COL32(0, 255, 0, 255), IM_COL32(0, 0, 255, 255),
			IM_COL32(0, 0, 255, 255), IM_COL32(0, 0, 255, 255), IM_COL32(255, 0, 255, 255), IM_COL32(255, 0, 255, 255),
			IM_COL32(255, 0, 255, 255)};

		if (boneConnections.size() == boneColors.size())
		{
			for (size_t i = 0; i < boneConnections.size(); ++i)
			{
				auto it1 = boneScreenPositions.find(boneConnections[i].first);
				auto it2 = boneScreenPositions.find(boneConnections[i].second);

				if (it1 == boneScreenPositions.end() || it2 == boneScreenPositions.end())
					continue;

				drawList->AddLine(it1->second, it2->second, boneColors[i], 2.5f);
			}
		}
	}
}
