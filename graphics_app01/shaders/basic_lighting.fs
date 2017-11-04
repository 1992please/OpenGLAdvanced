#version 330

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

out vec4 FragColor;

struct ColorChannel
{
	sampler2D Tex;
	vec3 Color;
};

struct Material
{
	ColorChannel Diffuse;
	float SpecularIntensity;
	float Shininess;
};

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];
uniform Material gMaterial;
uniform vec3 gEyeWorldPos;

vec3 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)
{
    float AmbientColorRatio = 0;
    float DiffuseColorRatio = 0;
    float SpecularColorRatio = 0;

    AmbientColorRatio =  Light.AmbientIntensity;
    float DiffuseFactor = max(dot(Normal, -LightDirection), 0.0);                                     
                                                                                            
    DiffuseColorRatio  = DiffuseFactor * Light.DiffuseIntensity;

    vec3 ViewDirection = normalize(gEyeWorldPos - WorldPos0);
    vec3 LightReflect = normalize(reflect(LightDirection, Normal));
    float SpecularFactor = dot(ViewDirection, LightReflect);
    if(SpecularFactor > 0)
    {
        SpecularFactor = pow(SpecularFactor, gMaterial.Shininess);
        SpecularColorRatio =  gMaterial.SpecularIntensity * SpecularFactor ;
    }

    return Light.Color * (AmbientColorRatio + DiffuseColorRatio + SpecularColorRatio);
}

vec3 CalcDirectionalLight(vec3 Normal)
{
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);
}

vec3 CalcPointLight(PointLight l, vec3 Normal)
{
    vec3 LightDirection = WorldPos0 - l.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec3 Color = CalcLightInternal(l.Base, LightDirection, Normal);
    float Attenuation = l.Atten.Constant + 
                        l.Atten.Linear * Distance +
                        l.Atten.Exp * Distance * Distance;
    return Color / Attenuation;
}

vec3 CalcSpotLight(SpotLight l, vec3 Normal)
{
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);
    float SpotFactor = dot(LightToPixel, l.Direction);

    if(SpotFactor > l.Cutoff)
    {
        vec3 Color = CalcPointLight(l.Base, Normal);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff)); 
    }
    else
    {
        return vec3(0.0, 0.0, 0.0);
    }
}

void main()
{
    vec3 Normal = normalize(Normal0);
    vec3 TotalLight = CalcDirectionalLight(Normal);

    for(int i = 0; i < gNumPointLights; i++)
    {
        TotalLight += CalcPointLight(gPointLights[i], Normal);
    }

    for(int i = 0; i < gNumSpotLights; i++)
    {
        TotalLight += CalcSpotLight(gSpotLights[i], Normal);
    }

    vec4 DiffuseColor = texture(gMaterial.Diffuse.Tex, TexCoord0) * vec4(gMaterial.Diffuse.Color, 1.0);

    FragColor =  DiffuseColor * vec4(TotalLight, 1.0);
}