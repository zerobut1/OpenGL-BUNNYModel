#version 330 core
struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;//反光度
};
struct Light{
    //vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
   

    //-----环境光-----
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));//环境光
    //----------

    //-----漫反射光-----
    vec3 norm = normalize(Normal); //法向量标准化
    //vec3 lightDir = normalize(light.position - FragPos);//光照方向向量 = 光源位置向量 - 片段位置向量
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm,lightDir),0.0); //点乘得到漫反射分量 同时保证不小于零
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); //漫反射光
    //----------

    //-----镜面光-----
    vec3 viewDir = normalize(viewPos - FragPos);//视线方向
    vec3 reflectDir = reflect(-lightDir,norm); //反射向量
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);//镜面分量
    vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));//镜面光
    //----------

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}