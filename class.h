#pragma once

#include <Windows.h>
#include <cstdint>
#include "types.h"

class cPed;

namespace Variable {
	inline uint64_t WorldPtr, ReplayInterface, BlipPTR, W2S, Bone, Camera;
	inline DWORD WeaponManager, PlayerInfo, LastVehicle, Ragdoll, Recoil, Spread, ReloadMultiplier, WeaponName, IsInVehicle, NoFall, Armour, EntityType, PedType, FreezePed, pPed;
}

enum class ePedBoneType
{
	HEAD,
	L_FOOT,
	R_FOOT,
	L_ANKLE,
	R_ANKLE,
	L_HAND,
	R_HAND,
	NECK,
	ABDOMEN
};

template<typename T>
union vector3
{
	T data[3];
	struct { T x, y, z; };

	vector3(T x, T y, T z) :
		x(x),
		y(y),
		z(z)
	{
	}

	vector3() :
		x(),
		y(),
		z()
	{
	}
};

typedef vector3<float> fvector3;

class cPedBoneInfo
{
public:
	fvector3 model_coords;
	char pad_000C[4];
};

class cPed {
public:
	cPedBoneInfo *BoneInfo() {
		if (!this) return 0;
		return (cPedBoneInfo *)(*(uint64_t *)(this + 0x410));
	}

	cModelInfo *ModelInfo() {
		if (!this) return 0;
		return (cModelInfo *)(*(uint64_t *)(this + 0x20));
	}

	cPlayerInfo *PlayerInfo() {
		if (!this) return 0;
		return (cPlayerInfo *)(*(uint64_t *)(this + Variable::PlayerInfo));
	}

	Vector3 GetPedCamCoords()
	{
		if (!this) return Vector3{ 0,0,0 };
		return *(Vector3 *)(this + 0x90);
	}
	
	bool IsPedInvisible()
	{
		if (!this) return 0;

		if (*(uint8_t *)(this + 0xAC) >= 100)
			return false;
		else
			return true;
	}
	
	float GetPedHealth()
	{
		if (!this) return 0;
		return *(float *)(this + 0x280);
	}
	
	float GetPedMaxHealth()
	{
		if (!this) return 0;
		return *(float *)(this + 0x2A0);
	}

	float GetArmour()
	{
		if (!this) return 0;
		return *(float *)(this + Variable::Armour);
	}

	uint32_t PedType()
	{
		if (!this) return 0;
		return *(uint32_t *)(this + Variable::PedType);
	}

	float GetSpeed() {
		Vector3 m_velocity = GetPedVelocity();
		return sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y + m_velocity.z * m_velocity.z);
	}
};

class cPedList
{
public:
	cPed *Ped(int index)
	{
		if (!this) return 0;
		return (cPed *)(*(uint64_t *)(this + (index * 0x10)));
	}
	uint64_t PedUintPtr(int index)
	{
		if (!this) return 0;
		return *(uint64_t *)(this + (index * 0x10));
	}
};

class cPedInterface
{
public:
	int32_t MaxPed()
	{
		if (!this) return 0;
		return *(int32_t *)(this + 0x108);
	}

	cPedList *PedList()
	{
		if (!this) return 0;
		return (cPedList *)(*(uint64_t *)(this + 0x100));
	}

	int32_t CurrentPeds()
	{
		if (!this) return 0;
		return *(int32_t *)(this + 0x110);
	}
};

class cReplayInterface
{
public:
	cPedInterface *PedInterface()
	{
		if (!this) return 0;
		return (cPedInterface *)(*(uint64_t *)(this + 0x18));
	}
};

class cWorld {
public:
	cPed *LocalPed()
	{
		if (!this) return 0;
		return (cPed *)(*(uint64_t *)(this + 0x8));
	}
	uintptr_t pedUintPtr()
	{
		if (!this) return 0;
		return *(uintptr_t *)(this + 0x8);
	}
};
