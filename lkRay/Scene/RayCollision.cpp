#include "PCH.hpp"
#include "RayCollision.hpp"

#include "Renderer/SurfaceDistribution.hpp"


namespace lkRay {
namespace Scene {

RayCollision::RayCollision()
    : mHitID(-1)
    , mDistance(0.0f)
    , mPoint()
    , mNormal()
    , mSurfaceDistribution(nullptr)
    , mAllocator(nullptr)
{
}

RayCollision::RayCollision(int hitID, float distance, const lkCommon::Math::Vector4& point, const lkCommon::Math::Vector4& normal)
    : mHitID(hitID)
    , mDistance(distance)
    , mPoint(point)
    , mNormal(normal)
    , mSurfaceDistribution(nullptr)
    , mAllocator(nullptr)
{
}

RayCollision::~RayCollision()
{
    if (mAllocator && mSurfaceDistribution)
    {
        mSurfaceDistribution->~SurfaceDistribution();
        mAllocator->Free(mSurfaceDistribution);
    }
}

} // namespace Scene
} // namespace lkRay
