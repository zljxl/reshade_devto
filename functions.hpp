#pragma once

#include <imgui.h>

#include "types.h"
#include "class.h"

bool IsOnScreen(ImVec2 coords)
{
	if (coords.x < 0.1f || coords.y < 0.1 || coords.x > ImGui::GetIO().DisplaySize.x || coords.y > ImGui::GetIO().DisplaySize.y)
	{
		return false;
	}
	else {
		return true;
	}
}

float GetDistanceBetweenCoords(Vector3 pointA, Vector3 pointB) {
	float x_ba = (float)(pointB.x - pointA.x);
	float y_ba = (float)(pointB.y - pointA.y);
	float z_ba = (float)(pointB.z - pointA.z);
	float y_2 = y_ba * y_ba;
	float x_2 = x_ba * x_ba;
	float sum_2 = y_2 + x_2;
	return (float)sqrtf(sum_2 + z_ba);
}

ImVec2 WorldToScreen(Vector3 pos)
{
	ImVec2 tempVec2;
	ImGuiIO IO = ImGui::GetIO();
	reinterpret_cast<bool(__fastcall *)(Vector3 *, float *, float *)>(Variable::W2S)(&pos, &tempVec2.x, &tempVec2.y);
	tempVec2.x *= IO.DisplaySize.x;
	tempVec2.y *= IO.DisplaySize.y;
	return tempVec2;
}

float pythag(ImVec2 src, ImVec2 dst)
{
	return sqrtf(pow(src.x - dst.x, 2) + pow(src.y - dst.y, 2));
}
