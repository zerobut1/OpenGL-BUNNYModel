#version 330 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    //-----环境光-----
    float ambientStrength = 0.1; //环境光照亮度
    vec3 ambient = ambientStrength * lightColor;//环境光
    //----------

    //-----漫反射光-----
    vec3 norm = normalize(Normal); //法向量标准化
    vec3 lightDir = normalize(lightPos - FragPos);//光照方向向量 = 光源位置向量 - 片段位置向量
    float diff = max(dot(norm,lightDir),0.0); //点乘得到漫反射分量 同时保证不小于零
    vec3  diffuse = diff * lightColor; //漫反射光
    //----------

    //-----镜面光-----
    float specularStrength = 0.5;//镜面强度
    vec3 viewDir = normalize(viewPos - FragPos);//视线方向
    vec3 reflectDir = reflect(-lightDir,norm); //反射向量
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);//32为高光的反光度
    vec3 specular = specularStrength * spec * lightColor;//镜面光
    //----------

    vec3 result = (ambient + diffuse + specular) * objectColor; 
    FragColor = vec4(result, 1.0);
}