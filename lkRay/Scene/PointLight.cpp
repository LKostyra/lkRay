#include "PCH.hpp"
#include "PointLight.hpp"
#include "Constants.hpp"


namespace lkRay {
namespace Scene {

PointLight::PointLight(const std::string& name)
    : Light(name)
    , mAttentuationFactor(1.0)
{
}

PointLight::PointLight(const std::string& name, const lkCommon::Math::Vector4& pos, const lkCommon::Utils::PixelFloat4& color, float attenuation)
    : Light(name, pos, color)
    , mAttentuationFactor(attenuation)
{
}

lkCommon::Utils::PixelFloat4 PointLight::Sample(const RayCollision& collision) const
{
    lkCommon::Math::Vector4 lightRayDir(mPosition - collision.mCollisionPoint);
    float distToLight = lightRayDir.Length();
    lightRayDir = lightRayDir.Normalize();

    float lightCoeff = lightRayDir.Dot(collision.mCollisionNormal);
    if (lightCoeff < 0.0f)
        lightCoeff = 0.0f;

    return mColor * lightCoeff * (1.0f / (1.0f + mAttentuationFactor * distToLight * distToLight));
}

bool PointLight::ReadParametersFromNode(const rapidjson::Value& value)
{
    if (!Light::ReadParametersFromNode(value))
    {
        LOGE("Failed to read base light parameters");
        return false;
    }

    // read light's attenuation factor
    for (auto& a: value.GetObject())
    {
        if (Constants::POINT_LIGHT_ATTRIBUTE_ATTENUATION_FACTOR_NODE_NAME.compare(a.name.GetString()) == 0)
        {
            mAttentuationFactor = a.value.GetFloat();
            LOGD("     -> Point light attenuation factor " << mAttentuationFactor);
            break;
        }
    }

    return true;
}

} // namespace Scene
} // namespace lkRay