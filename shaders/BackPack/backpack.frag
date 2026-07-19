#version 460 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D ambient;

    float shininess;
};

struct Light{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

float near = 0.1;
float far = 100.0;


out vec4 Color;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform Light light;
uniform vec3 viewPos;
uniform SpotLight spotlight;

float LinearizeDepth(float depth){
    float ndc = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - ndc * (far - near));
}

vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    float distance = length(light.position - FragPos);
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic*(distance*distance));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(texture_diffuse1,TexCoords)));
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.00f);
    vec3 specular = light.specular * spec * (vec3(texture(texture_specular1,TexCoords))); 
    
    
    ambient*= attenuation;
    diffuse*= attenuation * intensity;
    specular*= attenuation * intensity;

    vec3 result = ambient + diffuse + specular ;
    return result;
}

void main(){
    float distance = length(light.position - FragPos);
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1,TexCoords));
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic*(distance*distance));
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(texture_diffuse1,TexCoords)));

    vec3 specular = light.specular * (diff * vec3(texture(texture_specular1,TexCoords)));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    result += CalcSpotLight(spotlight, normal, FragPos, normalize(viewPos - FragPos));
    Color = vec4(result,1.0);
}
