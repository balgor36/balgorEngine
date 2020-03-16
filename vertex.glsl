#version 330 core

layout(location=0) in vec3 vertexPos;
layout(location=1) in vec3 vertexNormal;
layout(location=2) in vec2 vertexUV;
layout(location=3) in vec3 vertexTangent;
layout(location=4) in vec3 vertexBitangent;
layout(location=5) in vec3 vertexColor;

out vec3 fragPos;
out vec4 fragPosLightSpace;
out vec3 fragColor;
out vec2 UV;
out vec3 normal;

out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform vec3 inputColor;

void main(){
	gl_Position = projection * view * model * vec4(vertexPos, 1.0);
	fragPos = vec3(model * vec4(vertexPos, 1.0));
	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);
	UV = vertexUV;
	normal = vertexNormal;

	vec3 t = normalize(vec3(model * vec4(vertexTangent, 0.0)));
	vec3 b = normalize(vec3(model * vec4(vertexBitangent, 0.0)));
	vec3 n = normalize(vec3(model * vec4(vertexNormal, 0.0)));
        fragColor = vertexColor;
	TBN = mat3(t, b, n);
}
