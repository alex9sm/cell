#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D diffuseTexture;  

void main() {
    vec3 norm = normalize(Normal);
    
    vec4 texColor = texture(diffuseTexture, TexCoord);
    
    vec3 ambient = vec3(1.0, 0.9, 0.8);
    vec3 result = ambient * texColor.rgb;
    
    FragColor = vec4(result, texColor.a);
}