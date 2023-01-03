#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1; //环境光照亮度
    vec3 ambient = ambientStrength * lightColor;//环境光

    vec3 result = ambient * objectColor;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(lightColor * objectColor, 1.0);
}