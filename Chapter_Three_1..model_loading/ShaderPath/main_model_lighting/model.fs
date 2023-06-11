#version 330 core
out vec4 FragColor;

//定义一个点光源最少所需要的变量
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

in vec3 Normal;//顶点法向量
in vec3 FragPos;//片段位置(顶点)
in vec2 TexCoords;//纹理坐标

uniform vec3 viewPos;//相机位置
uniform PointLight pointLights;//实例化点光源
uniform sampler2D texture_diffuse1;//漫反射
uniform sampler2D texture_specular1;//镜面光
float shininess;//shininess影响镜面高光的散射/半径



vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)//点光源
{
vec3 lightDir = normalize(light.position - fragPos);//点光源光线方向向量
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);

    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),shininess);

    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
    // 通用属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3  result = CalcPointLight(pointLights, norm, FragPos, viewDir);
 
    FragColor = vec4(result, 1.0);
}