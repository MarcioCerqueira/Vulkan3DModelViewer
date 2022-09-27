#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;
layout(location = 3) in vec4 cameraPosition;
layout(location = 4) in float showTexture;

layout(location = 0) out vec4 outColor;

float computeAmbientLightIntensity() 
{
    return 0.2;
}

float computeDiffuseLightIntensity(vec3 normal, vec3 lightDirection)
{
    return 0.5 * max(dot(normal, lightDirection), 0.0);
}

float computeSpecularLightIntensity(vec3 normal, vec3 lightDirection)
{
    vec3 viewDirection = normalize(cameraPosition.xyz - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    return 0.5 * pow(max(dot(viewDirection, reflectDirection), 0.0), 0.5);
}

void main() 
{
    vec3 normal = normalize(fragNormal);
    vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
    float ambientLightIntensity = computeAmbientLightIntensity();
    float diffuseLightIntensity = computeDiffuseLightIntensity(normal, lightDirection);
    float specularLightIntensity = computeSpecularLightIntensity(normal, lightDirection);
    float totalLightIntensity = ambientLightIntensity + diffuseLightIntensity + specularLightIntensity;
    vec4 originalFragmentColor = (showTexture != 0.0) ? texture(texSampler, fragTexCoord) : vec4(1.0);
    outColor = totalLightIntensity * originalFragmentColor;
}