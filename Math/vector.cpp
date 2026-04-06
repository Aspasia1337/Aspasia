#include "vector.h"
#include "../globals.h"
#include "../EntityManager/EntityManager.h"
#include "../Helper/helper.h"

const bool Vec3::WorldToScreen(Vec2 &out, float (*ViewMatrix)[4][4])
{
    const float w = (*ViewMatrix)[3][0] * x + (*ViewMatrix)[3][1] * y + (*ViewMatrix)[3][2] * z + (*ViewMatrix)[3][3];

    if (w <= 0.01)
        return false;

    const float invW = 1.0f / w;
    const float screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const float screenHeight = GetSystemMetrics(SM_CYSCREEN);

    out.x = (screenWidth / 2) + (((*ViewMatrix)[0][0] * x + (*ViewMatrix)[0][1] * y + (*ViewMatrix)[0][2] * z + (*ViewMatrix)[0][3]) * invW * (screenWidth / 2));
    out.y = (screenHeight / 2) - (((*ViewMatrix)[1][0] * x + (*ViewMatrix)[1][1] * y + (*ViewMatrix)[1][2] * z + (*ViewMatrix)[1][3]) * invW * (screenHeight / 2));

    return true;
}

double CalculateDistance(Vec3 Source, Vec3 Dest)
{
    return sqrt(pow(Dest.x - Source.x, 2) + pow(Dest.y - Source.y, 2));
}

float ApproachAngle(float target, float current, float speed)
{
    float delta = target - current;

    while (delta > 180.0f)
        delta -= 360.0f;
    while (delta < -180.0f)
        delta += 360.0f;

    if (fabs(delta) > speed)
        return current + (delta / fabs(delta)) * speed;
    return target;
}

bool IsTargetWithinFOV(float targetYaw, float targetPitch, float playerYaw, float playerPitch, float AimFov, float &resultingDistance)
{
    float deltaYaw = targetYaw - playerYaw;
    float deltaPitch = targetPitch - playerPitch;

    while (deltaYaw > 180.0f)
        deltaYaw -= 360.0f;
    while (deltaYaw < -180.0f)
        deltaYaw += 360.0f;
    while (deltaPitch > 180.0f)
        deltaPitch -= 360.0f;
    while (deltaPitch < -180.0f)
        deltaPitch += 360.0f;

    resultingDistance = sqrtf(deltaYaw * deltaYaw + deltaPitch * deltaPitch);

    return resultingDistance <= AimFov;
}

Vec3 CalculateAngles(const Vec3 &vec3Source, const Vec3 &vec3Destination, float AimFov)
{
    Vec3 *viewangles = (Vec3 *)(iGameEntitySystem->clientDll + 0x1A733C0);

    float returningDistance;
    Vec3 qAngles;

    Vec3 playerToEnemyVec = Vec3(
        vec3Destination.x - vec3Source.x,
        vec3Destination.y - vec3Source.y,
        vec3Destination.z - vec3Source.z);

    double hyp = sqrtf(playerToEnemyVec.x * playerToEnemyVec.x + playerToEnemyVec.y * playerToEnemyVec.y);

    float targetX = -atan2(playerToEnemyVec.z, hyp) * (180.0 / std::numbers::pi);
    float targetY = atan2(playerToEnemyVec.y, playerToEnemyVec.x) * (180.0 / std::numbers::pi);

    if (!IsTargetWithinFOV(targetY, targetX, viewangles->y, viewangles->x, AimFov, returningDistance))
        return *viewangles;

    qAngles.x = ApproachAngle(targetX, viewangles->x, Globals::smoothing);
    qAngles.y = ApproachAngle(targetY, viewangles->y, Globals::smoothing);
    qAngles.z = 0.f;

    NormalizeAngles(qAngles);

    return qAngles;
}

void NormalizeAngles(Vec3 &qAngle)
{
    if (qAngle.x > 89.0f)
        qAngle.x = 89.0f;
    if (qAngle.x < -89.0f)
        qAngle.x = -89.0f;

    while (qAngle.y > 180.0f)
        qAngle.y -= 360.0f;
    while (qAngle.y < -180.0f)
        qAngle.y += 360.0f;
}