#version 330 core
out vec4 FragColor;

//定义材质结构体
struct Material {
    vec3 ambient;//ambient材质向量定义了在环境光照下这个表面反射的是什么颜色，通常与表面的颜色相同
    vec3 diffuse;//diffuse材质向量定义了在漫反射光照下表面的颜色,漫反射颜色（和环境光照一样）也被设置为我们期望的物体颜色
    vec3 specular;//specular材质向量设置的是表面上镜面高光的颜色（或者甚至可能反映一个特定表面的颜色）
    float shininess;//shininess影响镜面高光的散射/半径
}; 
//定义光照分量，一个光源对它的ambient、diffuse和specular光照分量有着不同的强度
struct Light {
    vec3 position;//光源位置

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;//传进来
uniform Light light;
  
in vec3 Normal;//顶点法向量
in vec3 FragPos;//片段位置(顶点)
in vec3 LightPos;//view空间中光源位置

uniform vec3 objectColor;//物体本身的颜色

uniform vec3 viewPos;//视角位置

void main()
{

//////// 环境光//////
vec3 ambient  = light.ambient * material.ambient;



// /////漫反射 ////////
//标准化法向量和方向向量
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(LightPos - FragPos);

//计算光源对当前片段实际的漫反射影响。结果值再乘以光的颜色，得到漫反射分量
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse  = light.diffuse * (diff * material.diffuse);



 // //////////镜面光////////////
//计算视线方向向量，和对应的沿着法线轴的反射向量
vec3 viewDir = normalize(viewPos- FragPos);//view空间中相机一直在原点
vec3 reflectDir = reflect(-lightDir, norm);

//计算镜面分量
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = light.specular * (spec * material.specular);

//最后一件事情是把它加到环境光分量和漫反射分量里，再用结果乘以物体的颜色
vec3 result = (ambient + diffuse + specular) * objectColor;
FragColor = vec4(result, 1.0);

}