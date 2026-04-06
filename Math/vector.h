#pragma once
#include <Windows.h>
#include <math.h>
#include <numbers> // M_PI

struct Vec2
{
	constexpr Vec2(
		const float x = 0.0f,
		const float y = 0.0f) noexcept : x(x), y(y) {};

	float x, y;
};

struct Vec3
{
	constexpr Vec3(
		const float x = 0.0f,
		const float y = 0.0f,
		const float z = 0.0f) noexcept : x(x), y(y), z(z) {};

	const bool WorldToScreen(Vec2 &out, float (*ViewMatrix)[4][4]);

	float x, y, z;
};

double CalculateDistance(Vec3 Source, Vec3 Dest);
Vec3 CalculateAngles(const Vec3 &vec3Source, const Vec3 &vec3Destination, float fov);
bool IsTargetWithinFOV(float targetYaw, float targetPitch, float playerYaw, float playerPitch, float AimFov, float &resultingDistance);

void NormalizeAngles(Vec3 &qAngle);