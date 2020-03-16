#version 330 core

struct LightMaterial {
    vec3 color;
};

struct Light{
    vec3 position;
    LightMaterial material;

    float range;
    float intensity;
};

// PBR - Physically based rendering (физически корректный рендеринг)
struct PBRMaterial{
    vec3 albedo;

    float metallic;
    float roughness;
    float occlusion;
};

const float PI = 3.14159265359;

uniform PBRMaterial pbrMaterial;
uniform Light light;

in vec3 fragPos;
in vec4 fragPosLightSpace;
in vec3 fragColor;
in vec2 UV;
in vec3 normal;

uniform mat4 model;
in mat3 TBN;

out vec3 color;

uniform bool isAlbedo;
uniform bool isNormalMap;
uniform bool isSelected;

uniform sampler2D albedoSampler;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

uniform vec3 cameraPos; // Позиция камеры

/*float shadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}*/

vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 calcPointLight(vec3 norm, vec3 F0){
    vec3 N = normalize(norm);
    vec3 V = normalize(cameraPos - fragPos);
    vec3 L = normalize(light.position - fragPos); // для точечного света (point light)
    vec3 H = normalize(V + L);

    float distance      = length(light.position - fragPos) / (light.range + 0.001);
    float attenuation   = 1.0 / (distance * distance);
    vec3 radiance       = light.material.color * attenuation;

    vec3 F      = fresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF   = DistributionGGX(N, H, pbrMaterial.roughness);
    float G     = GeometrySmith(N, V, L, pbrMaterial.roughness);

    vec3 numerator      = NDF * G * F;
    float denominator   = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular       = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - pbrMaterial.metallic;

    float NdotL = max(dot(N, L), 0.0);
    return (kD * pbrMaterial.albedo / PI + specular) * radiance * NdotL;
}

vec3 calcDirectionalLight(vec3 norm, vec3 F0){
    vec3 N = normalize(norm);
    vec3 V = normalize(cameraPos - fragPos);
    vec3 L = normalize(light.position);
    vec3 H = normalize(V + L);

    vec3 radiance       = light.material.color;

    vec3 F      = fresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF   = DistributionGGX(N, H, pbrMaterial.roughness);
    float G     = GeometrySmith(N, V, L, pbrMaterial.roughness);

    vec3 numerator      = NDF * G * F;
    float denominator   = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    vec3 specular       = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - pbrMaterial.metallic;

    float NdotL = max(dot(N, L), 0.0);
    return (kD * pbrMaterial.albedo / PI + specular) * radiance * NdotL;
}

void main(){
    vec3 norm;
    if(isNormalMap){
        norm = texture(normalMap, UV).rgb;
        norm = normalize(norm * 2.0 - 1.0);
        norm = normalize(TBN * norm);
    }
    else
        norm = mat3(transpose(inverse(model))) * normal;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, pbrMaterial.albedo, pbrMaterial.metallic);
    vec3 Lo;

    if(light.range < 0){
        Lo = calcDirectionalLight(norm, F0) * light.intensity;
    }
    else{
        Lo = calcPointLight(norm, F0) * light.intensity;
    }

    if(isAlbedo)
        color = Lo * texture(albedoSampler, UV).rgb;
    else
        color = Lo;

    if(isSelected)
        color = color * vec3(0.655, 1.0, 0.827);

    vec3 ambient = vec3(0.03) * pbrMaterial.albedo * pbrMaterial.occlusion;
    color += ambient;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
}
