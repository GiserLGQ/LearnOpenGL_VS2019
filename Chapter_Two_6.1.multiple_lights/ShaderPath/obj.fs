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

//定义一个定向光源最少所需要的变量
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

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



//定义一个聚光灯最少所需要的变量
struct SpotLight {
    vec3 position;
    vec3  direction;//手电筒朝向
    float cutOff;//内切光角
    float outerCutOff;//外切光角

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



#define NR_POINT_LIGHTS 4//预处理指令
in vec3 Normal;//顶点法向量
in vec3 FragPos;//片段位置(顶点)
in vec2 TexCoords;//纹理坐标


uniform vec3 viewPos;//相机位置
uniform DirLight dirLight;//实例化对象
uniform PointLight pointLights[NR_POINT_LIGHTS];//实例化对象
uniform SpotLight spotLight;//实例化对象
uniform Material material;//传进来


//函数
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);//定向光
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//点光源
vec3 CalcSpotLight(SpotLight  light, vec3 normal,vec3 fragPos, vec3 viewDir);//聚光灯


//所有的光照计算都在世界坐标系下进行，符合人的直觉
//也可以将所有的光照计算放到相机坐标系下进行，减少开支
void main()
{
    // 通用属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    

    FragColor = vec4(result, 1.0);
}


//定向光函数
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)//平行光结构体  //法线  //视线
{
vec3 lightDir = normalize(-light.direction);//定向光方向向量

// 漫反射着色
float diff = max(dot(normal, lightDir), 0.0);//漫反射系数（0~1）

// 镜面光着色
vec3 reflectDir = reflect(-lightDir, normal);//沿着法线轴的反射向量
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//视线与反射光线夹角

// 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));//环境光分量
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));//漫反射分量
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));//镜面分量

    return (ambient+diffuse+specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)//点光源结构体  //片段位置 //法线  //视线
{
vec3 lightDir = normalize(light.position - fragPos);//点光源光线方向向量
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);

    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight  light, vec3 normal,vec3 fragPos, vec3 viewDir)//聚光灯结构体  //法线   //片段位置 //视线
{
vec3 lightDir=normalize(fragPos-light.position);
float theta = dot(lightDir, normalize(light.direction));//计算切光角(聚光灯)
float epsilon   = light.cutOff - light.outerCutOff;//内圆锥外圆锥之差
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //在聚光外是负的，在内圆锥内大于1.0的，在边缘处于两者之间的强


 // 漫反射着色
    float diff = max(dot(normal, -lightDir), 0.0);

    // 镜面光着色
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // 衰减
    float distance    = length(viewPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));  
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

//衰减以及聚光
    ambient  *= attenuation;  // 聚光将不对环境光做出影响，让它总是能有一点光
    diffuse =diffuse * attenuation*intensity;
    specular =specular * attenuation*intensity;

    return (ambient + diffuse + specular);
}