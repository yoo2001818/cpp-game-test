#version 330 core
in vec3 vPosition;
in vec3 vNormal;
out vec4 FragColor;
uniform vec4 uLightPositions[2];
uniform vec3 uLightColors[2];
uniform vec3 uColor;
uniform float uRoughness;
uniform float uMetalic;
uniform mat4 uView;
void main()
{
    vec3 result = vec3(0.0);
    vec3 ambient = vec3(0.1);
    result += ambient * uColor;
    vec3 normal = normalize(vNormal);
    for (int i = 0; i < 2; i += 1) {
        vec3 lightPos = (uView * vec4(uLightPositions[i].xyz, 1.0)).xyz;
        vec3 lightDir = normalize(lightPos - vPosition);
        result += max(dot(vNormal, lightDir), 0.0)* uLightColors[i] * uColor;
        vec3 viewDir = normalize(-vPosition);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        result += 0.5 * spec * uLightColors[i];
    }
    FragColor = vec4(result, 1.0f);
}
