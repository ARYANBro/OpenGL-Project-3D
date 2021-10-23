#version 460

layout(location = 0) in vec4 a_VertexPos;
layout(location = 1) in vec2 a_TextureCoord;
layout(location = 2) in vec4 a_Normal;
layout(location = 3) in vec4 a_Tangent;

out vec2 v_TextureCoord;
out vec3 v_FragmentPos;
out vec3 v_Normal;
out mat3 v_TBN;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform mat4 u_Normal;

void main()
{
	vec3 normal = normalize(vec3(u_Normal * a_Normal));

	vec3 tangent = normalize(vec3(u_Model * a_Tangent));
	tangent = normalize(tangent - dot(tangent, normal) * normal);

	vec3 bitangent = cross(a_Normal.xyz, a_Tangent.xyz);
	bitangent = normalize(vec3(u_Model * vec4(bitangent, 0.0f)));

	v_TBN = mat3(tangent, bitangent, normal);

	v_TextureCoord = a_TextureCoord;
	v_FragmentPos = vec3(u_Model * a_VertexPos);
	v_Normal = normal;

	gl_Position = u_Projection * u_View * u_Model * a_VertexPos;
}