#include "PCH.hpp"
#include "Constants.hpp"


namespace lkRay {
namespace Constants {

const std::string LKRAY_DEFAULT_MATERIAL_NAME = "LKRAY_DEFAULT_MAT";


// JSON parsing names
const std::string NAME_NODE_NAME = "name";
const std::string AMBIENT_NODE_NAME = "ambient";

// materials
const std::string MATERIALS_NODE_NAME = "materials";
const std::string MATERIAL_MATTE_NODE_NAME = "matte";
const std::string MATERIAL_MIRROR_NODE_NAME = "mirror";

const std::string MATERIAL_ATTRIBUTE_TYPE_NODE_NAME = "type";

// lights
const std::string LIGHTS_NODE_NAME = "lights";
const std::string LIGHT_POINT_NODE_NAME = "point";

const std::string LIGHT_ATTRIBUTE_TYPE_NODE_NAME = "type";
const std::string LIGHT_ATTRIBUTE_POSITION_NODE_NAME = "position";
const std::string LIGHT_ATTRIBUTE_COLOR_NODE_NAME = "color";

const std::string POINT_LIGHT_ATTRIBUTE_ATTENUATION_FACTOR_NODE_NAME = "attFactor";

// objects
const std::string OBJECTS_NODE_NAME = "objects";
const std::string OBJECT_SPHERE_NODE_NAME = "sphere";
const std::string OBJECT_PLANE_NODE_NAME = "plane";
const std::string OBJECT_MESH_NODE_NAME = "mesh";

const std::string OBJECT_ATTRIBUTE_TYPE_NODE_NAME = "type";
const std::string OBJECT_ATTRIBUTE_POSITION_NODE_NAME = "position";
const std::string OBJECT_ATTRIBUTE_MATERIAL_NODE_NAME = "material";

const std::string SPHERE_ATTRIBUTE_RADIUS_NODE_NAME = "radius";

const std::string MESH_ATTRIBUTE_VERTICES_NODE_NAME = "vertices";
const std::string MESH_ATTRIBUTE_INDICES_NODE_NAME = "indices";


} // namespace lkRay
} // namespace Constants