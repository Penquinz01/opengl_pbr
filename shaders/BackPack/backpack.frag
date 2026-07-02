#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D ambient;
};

out vec4 Color;
uniform sampler2D texture_diffuse1;

void main(){
    Color = vec4(texture(texture_diffuse1,TexCoords));
}
