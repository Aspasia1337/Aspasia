#include "vector.h"

const bool Vec3::WorldToScreen (Vec2 &out, float(*ViewMatrix)[4][4])
{
	const float w = (*ViewMatrix)[3][0] * x + (*ViewMatrix)[3][1] * y + (*ViewMatrix)[3][2] * z + (*ViewMatrix)[3][3];

	if (w <= 0.01)
		return false;

	const float invW = 1.0f / w;

	const float screenWidth = GetSystemMetrics (SM_CXSCREEN);
	const float screenHeight = GetSystemMetrics (SM_CYSCREEN);

	out.x = (screenWidth / 2) + (((*ViewMatrix)[0][0] * x + (*ViewMatrix)[0][1] * y + (*ViewMatrix)[0][2] * z + (*ViewMatrix)[0][3]) * invW * (screenWidth / 2));

	out.y = (screenHeight / 2) - (((*ViewMatrix)[1][0] * x + (*ViewMatrix)[1][1] * y + (*ViewMatrix)[1][2] * z + (*ViewMatrix)[1][3]) * invW * (screenHeight / 2));

	return true;
}


double CalculateDistance (Vec3 Source, Vec3 Dest) {
	return sqrt (pow (Dest.x - Source.x, 2) + pow (Dest.y - Source.y, 2));
}

// Poor Angle Calculation 
Vec3 CalculateAngles (const Vec3 &vec3Source, const Vec3 &vec3Destination) {

	Vec3 qAngles;
	
	Vec3 playerToEnemyVec = Vec3 ((vec3Source.x - vec3Destination.x), (vec3Source.y - vec3Destination.y), (vec3Source.z - vec3Destination.z));
	
	double hyp = sqrtf (playerToEnemyVec.x * playerToEnemyVec.x + playerToEnemyVec.y * playerToEnemyVec.y);

	qAngles.x = (float)(atan (playerToEnemyVec.z / hyp) * (180.0 / 3.14159265358979323846)); // yaw
	qAngles.y = (float)(atan (playerToEnemyVec.y / playerToEnemyVec.x) * (180.0 / 3.14159265358979323846)); //pitch
	qAngles.z = 0.f;

	if (playerToEnemyVec.x >= 0.f)
		qAngles.y += 180.f;

	NormalizeAngles (qAngles);

	return qAngles;

}


void NormalizeAngles (Vec3 &qAngle) {
	while (qAngle.x > 89.0f) qAngle.x -= 180.0f;
	while (qAngle.x < -89.0f) qAngle.x += 180.0f;
	while (qAngle.y > 180.0f) qAngle.y -= 360.0f;
	while (qAngle.y < -180.0f) qAngle.y += 360.0f;
}