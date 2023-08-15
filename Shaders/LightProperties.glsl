struct DirectionalLight
{
    vec3 diffuse;
    vec3 direction;
    mat4 LightSpaceMatrix;
    float intensity;
};

struct PointLight
{
    vec3 diffuse;
    vec3 position;
    mat4 LightSpaceMatrix;
    float intensity;
    float radius;
};

struct SunLight
{
    vec3 diffuse;
    vec3 position;
    mat4 LightSpaceMatrix;
    float intensity;
};

struct SpotLight
{
    vec3 diffuse;
    vec3 position;
    vec3 direction;
    mat4 lightSpaceMatrix;
    float intensity;
    float radius;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};