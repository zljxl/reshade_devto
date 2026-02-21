#pragma once

#include "class.h"
#include "xor_string.hpp"
#include "xor_func.hpp"
#include "functions.hpp"

#include <d3d9.h>
#include <iostream>
#include <unordered_map>

#include <d3d9/d3d9_math.h>

#pragma comment(lib, "d3dx9.lib")

bool initalized = false;

// version 2060

void init() {

	uint64_t BaseAddr = (uint64_t)SAFE_CALL(GetModuleHandleA)("FiveM_b2060_GTAProcess.exe");

	Variable::WorldPtr = 0x24c8858;
	Variable::W2S = 0xa25cc8;
	Variable::ReplayInterface = 0x1ec3828;
	Variable::Bone = 0x7935d0;

	Variable::WeaponManager = 0x10D8;
	Variable::PlayerInfo = 0x10C8;
	Variable::LastVehicle = 0xD30;

	Variable::Armour = 0x14E0;
	Variable::Ragdoll = 0x10B8;
	Variable::Recoil = 0x2F4;
	Variable::Spread = 0x84;
	Variable::ReloadMultiplier = 0x134;
	Variable::WeaponName = 0x5F0;
	Variable::PedType = 0x10B8;
	Variable::FreezePed = 0x218;
	Variable::pPed = 0x1C8;
}

ImVec2 GetBonePos3D(uintptr_t cPed, uintptr_t wMask)
{
	__m128 tempVec4;
	reinterpret_cast<void *(__fastcall *)(int64_t, __m128 *, int32_t)>(Variable::Bone)(cPed, &tempVec4, wMask);
	return WorldToScreen(Vector3::FromM128(tempVec4));
}

Vector3 GetBonePos3DMy(uintptr_t ped_ptr, int bone_id)
{
	D3DXMATRIX matrix = *(D3DXMATRIX *)(ped_ptr + 0x60);
	D3DXVECTOR3 bone = *(D3DXVECTOR3 *)(ped_ptr + (0x430 + bone_id * 0x10));
	D3DXVECTOR4 transform;
	D3DXVec3Transform(&transform, &bone, &matrix);
	return Vector3(transform.x, transform.y, transform.z);

}
int tickkkk()
{
	cWorld *world = (cWorld *)*(uint64_t *)(Variable::WorldPtr);
	if (!world)
		return 0;

	cPed *myPed = world->LocalPed();
	if (myPed == nullptr)
		return 0;

	cReplayInterface *replayInterface = (cReplayInterface *)*(uint64_t *)(Variable::ReplayInterface);
	if (replayInterface == nullptr)
		return 0;

	float closestdist = 9999999.f;
	float dist;
	cPed *ClosestPlayer = nullptr;
	ImVec2 closestHeadPos;

	for (int i = 0; i < 128; i++) {
		uintptr_t myPedUintPtr = world->pedUintPtr();
		cPed *tempPed = replayInterface->PedInterface()->PedList()->Ped(i);
		uintptr_t tempPedUintPtr = replayInterface->PedInterface()->PedList()->PedUintPtr(i);

		if (tempPed != nullptr && world != nullptr && myPed != nullptr)
		{
			auto pedType = tempPed->PedType();
			pedType = pedType << 11 >> 25;
			if (pedType != PedTypes::NETWORK_PLAYER)
				continue;

			float Distance = GetDistanceBetweenCoords(tempPed->GetPedCamCoords(), myPed->GetPedCamCoords());
			if (Distance > 100.f) continue;

			float tempPedHealth = tempPed->GetPedHealth();
			if ((tempPed->GetPedHealth() - 100) <= 0) continue;

			ImDrawList *draw = ImGui::GetBackgroundDrawList();

			ImVec2 Ped2DPos = WorldToScreen(tempPed->GetPedCamCoords());
			ImVec2 LocPos2D = WorldToScreen(myPed->GetPedCamCoords());

			/*if (IsOnScreen(Head) && IsOnScreen(Pelvis))
			{
				draw->AddLine(Head, Pelvis, ImColor(1.f, 0.f, 0.f, 1.f), 1.f);
				LOG(INFO) << "Head X " << Head.x << " Head Y " << Head.y << " Pelvis X " << Pelvis.x << " Pelvis Y " << Pelvis.y;
			}*/

			ImVec2 Head = WorldToScreen(GetBonePos3DMy(tempPedUintPtr, PedBones::HEAD));
			ImVec2 Neck = WorldToScreen(GetBonePos3DMy(tempPedUintPtr, PedBones::NECK));
			ImVec2 LeftFoot = WorldToScreen(GetBonePos3DMy(tempPedUintPtr, PedBones::L_ANKLE));
			ImVec2 RightFoot = WorldToScreen(GetBonePos3DMy(tempPedUintPtr, PedBones::R_ANKLE));
			ImVec2 LeftHand = WorldToScreen(GetBonePos3DMy(tempPedUintPtr, PedBones::L_HAND));
			ImVec2 RightHand = WorldToScreen(GetBonePos3DMy(tempPedUintPtr, PedBones::R_HAND));
			ImVec2 Pelvis = WorldToScreen(GetBonePos3DMy(tempPedUintPtr, PedBones::ABDOMEN));

			float FootAdv = (LeftFoot.y + RightFoot.y) / 2;
			float height = fabs(Head.y - (FootAdv)) * 1.15;
			float width = height / 2;
			float font_size = 18;

			if (IsOnScreen(Head) && IsOnScreen(RightFoot) && IsOnScreen(LeftFoot) && IsOnScreen(RightHand) && IsOnScreen(LeftHand) && IsOnScreen(Pelvis))
			{

				ImVec2 dpSize = ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
				ImVec2 middle = ImVec2(dpSize.x / 2, dpSize.y / 2);

				dist = pythag(middle, Head);

				draw->AddRect(ImVec2(Ped2DPos.x - width / 1.5, Ped2DPos.y - height / 1.5), ImVec2(Ped2DPos.x + width / 2, Ped2DPos.y + height / 1.7), ImColor(1.f, 0.f, 0.f, 1.f), 1, 1, 1.f);

				ImColor skCol = ImColor(1.f, 0.f, 0.f, 1.f);

				draw->AddLine(Head, Neck, skCol, 1.f);
				draw->AddLine(Neck, LeftHand, skCol, 1.f);
				draw->AddLine(Neck, RightHand, skCol, 1.f);
				draw->AddLine(Neck, Pelvis, skCol, 1.f);
				draw->AddLine(Pelvis, LeftFoot, skCol, 1.f);
				draw->AddLine(Pelvis, RightFoot, skCol, 1.f);
			}
		}
	}
	return 0;
}
