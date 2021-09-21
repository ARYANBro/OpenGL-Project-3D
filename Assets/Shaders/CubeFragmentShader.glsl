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

#define MAX_NUM_LIGHTS 32

uniform vec3 u_CameraPos;
uniform sampler2D u_DiffuseTex;
uniform sampler2D u_SpecularTex;
uniform sampler2D u_NormalMap;
uniform vec3 u_Color;
uniform float u_Smoothness;

// uniform SpotLight u_SpotLight;
// uniform PointLight u_PointLight;
// uniform DirectionalLight u_DirLight;

uniform PointLight u_PointLights[MAX_NUM_LIGHTS];
uniform SpotLight u_SpotLights[MAX_NUM_LIGHTS];
uniform DirectionalLight u_DirLights[MAX_NUM_LIGHTS];

uniform uint u_NumActivePointLights;
uniform uint u_NumActiveSpotLights;
uniform uint u_NumActiveDirLights;

vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 cameraPos)
{
	vec3 ambient = spotLight.Color.Ambient * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

	vec3 lightDir = normalize(spotLight.Position - v_FragmentPos);
	float diffuseAmt = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = spotLight.Color.Diffuse * diffuseAmt * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

	vec3 cameraDir = normalize(cameraPos - v_FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularAmt = pow(max(dot(reflectDir, cameraDir), 0.0f), u_Smoothness * 256);
	vec3 specular = spotLight.Color.Specular * specularAmt * texture2D(u_SpecularTex, v_TextureCoord).r;

	float theta = dot(lightDir, normalize(-spotLight.Direction));
	float epsilon = spotLight.CutOff - spotLight.OuterCutOff;
	float intensity = clamp((theta - spotLight.OuterCutOff) / epsilon, 0.0f, 1.0f);

	float distance = length(spotLight.Position - v_FragmentPos);
	float constant = spotLight.Attenuation.Constant;
	float linear = spotLight.Attenuation.Linear * distance;
	float quadratic = spotLight.Attenuation.Quadratic * distance * distance;
	float attenuation = 1.0f / (constant + linear + quadratic);

	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	ambient *= attenuation;

	return (diffuse + specular + ambient);
}

vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 cameraPos)
{
	vec3 ambient = pointLight.Color.Ambient * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

	vec3 lightDir = normalize(pointLight.Position - v_FragmentPos);
	float diffuseAmt = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = pointLight.Color.Diffuse * diffuseAmt * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

	vec3 cameraDir = normalize(cameraPos - v_FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularAmt = pow(max(dot(reflectDir, cameraDir), 0.0f), u_Smoothness * 256);
	vec3 specular = pointLight.Color.Specular * specularAmt * texture2D(u_SpecularTex, v_TextureCoord).r;

	float distance = length(pointLight.Position - v_FragmentPos);
	float constant = pointLight.Attenuation.Constant;
	float linear = pointLight.Attenuation.Linear * distance;
	float quadratic = pointLight.Attenuation.Quadratic * distance * distance;
	float attenuation = 1.0 / (constant + linear + quadratic);

	diffuse *= attenuation;
	specular *= attenuation;
	ambient *= attenuation;

	return (diffuse + specular + ambient);
}

vec3 CalculateDirLight(DirectionalLight dirLight, vec3 normal, vec3 cameraPos)
{
	vec3 ambient = dirLight.Color.Ambient * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

	vec3 lightDir = normalize(-dirLight.Direction);
	float diffuseAmt = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = dirLight.Color.Diffuse * diffuseAmt * texture2D(u_DiffuseTex, v_TextureCoord).rgb;

	vec3 cameraDir = normalize(cameraPos - v_FragmentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float specularAmt = pow(max(dot(reflectDir, cameraDir), 0.0f), u_Smoothness * 256);
	vec3 specular = dirLight.Color.Specular * specularAmt * texture2D(u_SpecularTex, v_TextureCoord).r;

	return (diffuse + specular + ambient);
}

void main()
{
	vec3 normal = (2 * texture2D(u_NormalMap, v_TextureCoord).rgb) - 1;
	normal = normalize(v_TBN * normal);

	vec3 result;

	for (uint i = 0; i < u_NumActivePointLights; i++)
		result += CalculatePointLight(u_PointLights[i], normal, u_CameraPos);

	for (uint i = 0; i < u_NumActiveSpotLights; i++)
		result += CalculateSpotLight(u_SpotLights[i], normal, u_CameraPos);

	for (uint i = 0; i < u_NumActiveDirLights; i++)
		result += CalculateDirLight(u_DirLights[i], normal, u_CameraPos);

	// result = CalculateSpotLight(u_SpotLight, normalize(normal), u_CameraPos);
	// result += CalculatePointLight(u_PointLight, normalize(normal), u_CameraPos);
	// result += CalculateDirLight(u_DirLight, normalize(normal), u_CameraPos);

	v_Color = vec4(result + u_Color, 1.0f);
}