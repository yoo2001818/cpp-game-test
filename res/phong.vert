#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
out vec3 vPosition;
out vec3 vNormal;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
   gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
   vNormal = normalize((uView * uModel * vec4(aNormal, 0.0)).xyz);
   vPosition = (uView * uModel * vec4(aPosition, 1.0)).xyz;
}
