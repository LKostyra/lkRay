#include "PCH.hpp"
#include "Renderer.hpp"

#include <lkCommon/Utils/Logger.hpp>
#include <lkCommon/Utils/Pixel.hpp>


namespace {

const uint32_t PIXELS_PER_THREAD = 32;
const lkCommon::Utils::PixelFloat4 AMBIENT_COLOR({0.1f, 0.1f, 0.1f, 1.0f});

} // namespace


namespace lkRay {
namespace Renderer {

Renderer::Renderer(const uint32_t renderWidth, const uint32_t renderHeight)
    : mOutputImage(renderWidth, renderHeight)
    , mThreadPool()
{
}

lkCommon::Math::Vector4 Renderer::LerpPoints(const lkCommon::Math::Vector4& p1, const lkCommon::Math::Vector4& p2, float factor)
{
    return p1 * (1.0f - factor) + p2 * factor;
}

// -1 if no object hit, otherwise index of hit primitive
int32_t Renderer::CastRay(const Scene::Scene::PrimitiveContainer& prims, const Geometry::Ray& ray, int rayCounter,
                          int skipObjID, float& distance, lkCommon::Math::Vector4& normal)
{
    if (rayCounter > 0)
        return -1;

    int32_t hitID = -1;
    float rayDistance = 0.0f;
    distance = std::numeric_limits<float>::max();
    lkCommon::Math::Vector4 colNormal;

    for (int i = 0; i < prims.size(); ++i)
    {
        if (skipObjID == i)
            continue;

        if (prims[i]->TestCollision(ray, rayDistance, colNormal) && rayDistance < distance)
        {
            distance = rayDistance;
            normal = colNormal;
            hitID = i;
        }
    }

    return hitID;
}

lkCommon::Utils::PixelFloat4 Renderer::DrawPixel(const Scene::Scene& scene, const Geometry::Ray& primaryRay)
{
    lkCommon::Utils::PixelFloat4 resultColor = AMBIENT_COLOR;
    lkCommon::Math::Vector4 collision;
    lkCommon::Math::Vector4 normal;
    lkCommon::Math::Vector4 shadowNorm;
    float distance = 0.0f;
    float shadowDist = 0.0f;
    float lightCoeff = 0.0f;
    int32_t hitID = -1;

    const Scene::Scene::PrimitiveContainer& prims = scene.GetPrimitives();
    hitID = CastRay(prims, primaryRay, 0, -1, distance, normal);
    if (hitID != -1)
    {
        collision = primaryRay.GetOrigin() + primaryRay.GetDirection() * distance;
        for (const auto& l: scene.GetLights())
        {
            lkCommon::Math::Vector4 lightRayDir(l->GetPosition() - collision);
            float colDistance = lightRayDir.Length();
            lightRayDir = lightRayDir.Normalize();

            Geometry::Ray shadowRay(collision, lightRayDir);
            if ((CastRay(prims, shadowRay, 0, hitID, shadowDist, shadowNorm) == -1) ||
                (shadowDist > colDistance))
            {
                lightCoeff = lightRayDir.Dot(normal);
                if (lightCoeff < 0.0f)
                    lightCoeff = 0.0f;

                float att = (1.0f / (1.0f + l->GetAttenuationFactor() * colDistance * colDistance));

                resultColor += l->GetColor() * lightCoeff * att;
            }
        }

        const Scene::Material& mat = prims[hitID]->GetMaterial();
        resultColor *= mat.GetColor() * mat.GetAlbedo();
    }

    return resultColor;
}

void Renderer::DrawThread(const Scene::Scene& scene, const Scene::Camera& camera, uint32_t widthPos, uint32_t heightPos, uint32_t xCount, uint32_t yCount)
{
    for (uint32_t x = widthPos; x < widthPos + xCount; ++x)
    {
        for (uint32_t y = heightPos; y < heightPos + yCount; ++y)
        {
            // don't go out of bounds
            if (x >= mOutputImage.GetWidth() || y >= mOutputImage.GetHeight())
                continue;

            // calculate where our ray will shoot
            float xFactor = static_cast<float>(x) / static_cast<float>(mOutputImage.GetWidth());
            float yFactor = static_cast<float>(y) / static_cast<float>(mOutputImage.GetHeight());

            lkCommon::Math::Vector4 xLerp1 = LerpPoints(
                camera.GetCameraCorner(Scene::Camera::Corners::TOP_L),
                camera.GetCameraCorner(Scene::Camera::Corners::TOP_R),
                xFactor
            );
            lkCommon::Math::Vector4 xLerp2 = LerpPoints(
                camera.GetCameraCorner(Scene::Camera::Corners::BOT_L),
                camera.GetCameraCorner(Scene::Camera::Corners::BOT_R),
                xFactor
            );
            lkCommon::Math::Vector4 rayTarget(LerpPoints(xLerp1, xLerp2, yFactor));
            lkCommon::Math::Vector4 rayDir((rayTarget - camera.GetPosition()).Normalize());

            // form a ray and cast it
            Geometry::Ray ray(camera.GetPosition(), rayDir);
            mOutputImage.SetPixel(x, y, DrawPixel(scene, ray));
        }
    }
}

void Renderer::Draw(const Scene::Scene& scene, const Scene::Camera& camera)
{
    for (uint32_t x = 0; x < mOutputImage.GetWidth(); x += PIXELS_PER_THREAD)
    {
        for (uint32_t y = 0; y < mOutputImage.GetHeight(); y += PIXELS_PER_THREAD)
        {
            mThreadPool.AddTask(std::bind(&Renderer::DrawThread, this, scene, camera, x, y, PIXELS_PER_THREAD, PIXELS_PER_THREAD));
        }
    }

    mThreadPool.WaitForTasks();
}

bool Renderer::ResizeOutput(const uint32_t width, const uint32_t height)
{
    if (width == 0 || height == 0)
    {
        LOGE("Output dimensions cannot be equal to zero");
        return false;
    }

    if (!mOutputImage.Resize(width, height))
    {
        LOGE("Failed to resize output image");
        return false;
    }

    return true;
}

} // namespace Renderer
} // namespace lkRay
