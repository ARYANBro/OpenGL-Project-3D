#version 460

out vec4 v_Color;

in vec2 v_TextureCoord;
in vec3 v_FragmentPos;
in mat3 v_TBN;

struct LightColor
{
	vec3 Diffuse;
	vec3 Specular;
	vec3 Ambient;
};

struct AttenuationTerms
{
	float Constant;
	float Linear;
	float Quadratic;
};

struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	LightColor Color;
	AttenuationTerms Attenuation;
	float CutOff;
	float OuterCutOff;
};

struct PointLight
{
	vec3 Position;
	LightColor Color;
	AttenuationTerms Attenuation;
};

struct DirectionalLight
{
	vec3 Direction;
	LightColor Color;
};

struct Material
{
	sampler2D DiffuseTex;
	sampler2D SpecularTex;
	sampler2D NormalMap;

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float SpecularExp;
};

#define MAX_NUM_LIGHTS 32

uniform Material u_Material;

uniform vec3 u_CameraPos;

uniform PointLight u_PointLights[MAX_NUM_LIGHTS];
uniform SpotLight u_SpotLights[MAX_NUM_LIGHTS];
uniform DirectionalLight u_DirLights[MAX_NUM_LIGHTS];

uniform uint u_NumActivePointLights;
uniform uint u_NumActiveSpotLights;
uniform uint u_NumActiveDirLights;

vec3 CalculateAmbient(vec3 lightAmbient, Material material)
{
	vec3 ambient;
	vec3 textureAmbient = texture2D(material.DiffuseTex, v_TextureCoord).rgb;

	if (textureAmbient != 0)
		ambient = textureAmbient;
	else 
		ambient = material.Ambient;

	return lightAmbient * ambient;
}

vec3 CalculateDiffuse(vec3 normal, vec3 lightDir, vec3 lightDiffuse, Material material)
{
	float diffuseAmt = max(dot(normal, lightDir), 0.0f);

	vec3 diffuse;
	vec3 textureDiffuse = texture2D(material.DiffuseTex, v_TextureCoord).rgb;

	if (textureDiffuse != 0)
		diffuse = textureDiffuse;
	else
		diffuse = material.Diffuse; 

	return lightDiffuse * diffuseAmt * diffuse;
}

vec3 CalculateSpecular(vec3 cameraPos, vec3 lightDir, vec3 normal, vec3 lightSpecular, Material material)
{
	vec3 cameraDir = normalize(cameraPos - v_FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	vec3 specular;
	vec3 textureSpecular = texture2D(material.SpecularTex, v_TextureCoord).rrr;

	if (textureSpecular != 0)
		specular = textureSpecular;
	else
		specular = material.Specular;

	float specularAmt = pow(max(dot(reflectDir, cameraDir), 0.0f), material.SpecularExp);
	return lightSpecular * specularAmt * specular;
}

float CalculateLightAttenuation(vec3 lightPos, AttenuationTerms attenuation)
{
	float distance = length(lightPos - v_FragmentPos);
	float constant = attenuation.Constant;
	float linear = attenuation.Linear * distance;
	float quadratic = attenuation.Quadratic * distance * distance;
	return 1.0f / (constant + linear + quadratic);
}

vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 cameraPos)
{
	vec3 lightDir = normalize(spotLight.Position - v_FragmentPos);

	vec3 ambient = CalculateAmbient(spotLight.Color.Ambient, u_Material);
	vec3 diffuse = CalculateDiffuse(normal, lightDir, spotLight.Color.Diffuse, u_Material);
	vec3 specular = CalculateSpecular(cameraPos, lightDir, normal, spotLight.Color.Specular, u_Material);

	float theta = dot(lightDir, normalize(-spotLight.Direction));
	float epsilon = spotLight.CutOff - spotLight.OuterCutOff;
	float intensity = clamp((theta - spotLight.OuterCutOff) / epsilon, 0.0f, 1.0f);

	float attenuation = CalculateLightAttenuation(spotLight.Position, spotLight.Attenuation);

	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	ambient *= attenuation;

	return (diffuse + specular + ambient);
}

vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 cameraPos)
{
	vec3 lightDir = normalize(pointLight.Position - v_FragmentPos);

	vec3 ambient = CalculateAmbient(pointLight.Color.Ambient, u_Material);
	vec3 diffuse = CalculateDiffuse(normal, lightDir, pointLight.Color.Diffuse, u_Material);
	vec3 specular = CalculateSpecular(cameraPos, lightDir, normal, pointLight.Color.Specular, u_Material);

	float attenuation = CalculateLightAttenuation(pointLight.Position, pointLight.Attenuation);

	diffuse *= attenuation;
	specular *= attenuation;
	ambient *= attenuation;

	return (diffuse + specular + ambient);
}

vec3 CalculateDirLight(DirectionalLight dirLight, vec3 normal, vec3 cameraPos)
{
	vec3 lightDir = normalize(-dirLight.Direction);
	vec3 ambient = CalculateAmbient(dirLight.Color.Ambient, u_Material);
	vec3 diffuse = CalculateDiffuse(normal, lightDir, dirLight.Color.Diffuse, u_Material);
	vec3 specular = CalculateSpecular(cameraPos, lightDir, normal, dirLight.Color.Specular, u_Material);

	return (diffuse + specular + ambient);
}

void main()
{
	vec3 normal = (2 * texture2D(u_Material.NormalMap, v_TextureCoord).rgb) - 1;
	normal = normalize(v_TBN * normal);

	vec3 result;

	for (uint i = 0; i < u_NumActivePointLights; i++)
		result += CalculatePointLight(u_PointLights[i], normal, u_CameraPos);

	for (uint i = 0; i < u_NumActiveSpotLights; i++)
		result += CalculateSpotLight(u_SpotLights[i], normal, u_CameraPos);

	for (uint i = 0; i < u_NumActiveDirLights; i++)
		result += CalculateDirLight(u_DirLights[i], normal, u_CameraPos);

	v_Color = vec4(result, 1.0f);
}