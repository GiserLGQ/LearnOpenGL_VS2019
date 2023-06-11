#version 330 core
out vec4 FragColor;

//定义材质结构体
struct Material {
                                  //vec3 ambient;//ambient材质向量定义了在环境光照下这个表面反射的是什么颜色，通常与表面的颜色相同
    sampler2D diffuse;//diffuse材质向量定义了在漫反射光照下表面的颜色,漫反射颜色（和环境光照一样）也被设置为我们期望的物体颜色
    sampler2D specular;//specular材质向量设置的是表面上镜面高光的颜色（或者甚至可能反映一个特定表面的颜色）
    float shininess;//shininess影响镜面高光的散射/半径
                                 //sampler2D emission;//放射光贴图
}; 
//定义光照分量，一个光源对它的ambient、diffuse和specular光照分量有着不同的强度
struct Light {
    vec3 position;//光源位置，使用定向光就不再需要了

    //vec3 direction;//定向光向量
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

  
in vec3 Normal;//顶点法向量
in vec3 FragPos;//片段位置(顶点)
in vec2 TexCoords;//纹理坐标



uniform Material material;//传进来
uniform Light light;
uniform vec3 viewPos;


void main()
{

//////// 环境光//////
vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;



// /////漫反射 ////////
//标准化法向量和方向向量
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(light.position - FragPos);

//计算光源对当前片段实际的漫反射影响。结果值再乘以光的颜色，得到漫反射分量
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;



 // //////////镜面光////////////
//计算视线方向向量，和对应的沿着法线轴的反射向量
vec3 viewDir = normalize(viewPos- FragPos);
vec3 reflectDir = reflect(-lightDir, norm);

//计算镜面分量
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;//在这里，我们反转采样的镜面反射颜色。黑变成白，白变成黑。


//计算光线衰减
float distance    = length(light.position - FragPos);
float attenuation = 1.0 / (light.constant + light.linear * distance +light.quadratic * (distance * distance));

ambient  *= attenuation; 
diffuse  *= attenuation;
specular *= attenuation;


vec3 result = ambient +diffuse +specular;
FragColor = vec4(result, 1.0);

}