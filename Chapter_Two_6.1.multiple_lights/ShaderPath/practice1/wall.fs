#version 330 core
out vec4 FragColor;

//定义材质结构体
struct Material {
    sampler2D diffuse;//diffuse材质向量定义了在漫反射光照下表面的颜色,漫反射颜色（和环境光照一样）也被设置为我们期望的物体颜色
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




in vec3 Normal;//顶点法向量
in vec3 FragPos;//片段位置(顶点)
in vec2 TexCoords;//纹理坐标


uniform vec3 viewPos;//相机位置
uniform DirLight dirLight;//实例化对象
uniform PointLight pointLights;//实例化对象
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
     result += CalcPointLight(pointLights, norm, FragPos, viewDir);    
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


// 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));//环境光分量
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));//漫反射分量

    return (ambient+diffuse);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)//点光源结构体  //片段位置 //法线  //视线
{
vec3 lightDir = normalize(light.position - fragPos);//点光源光线方向向量
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);



    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;

    return (ambient + diffuse);
}

vec3 CalcSpotLight(SpotLight  light, vec3 normal,vec3 fragPos, vec3 viewDir)//聚光灯结构体  //法线   //片段位置 //视线
{
vec3 lightDir=normalize(fragPos-light.position);
float theta = dot(lightDir, normalize(light.direction));//计算切光角(聚光灯)
float epsilon   = light.cutOff - light.outerCutOff;//内圆锥外圆锥之差
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //在聚光外是负的，在内圆锥内大于1.0的，在边缘处于两者之间的强


 // 漫反射着色
    float diff = max(dot(normal, -lightDir), 0.0);


    // 衰减
    float distance    = length(viewPos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));


    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));  
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));

//衰减以及聚光
    ambient  *= attenuation;  // 聚光将不对环境光做出影响，让它总是能有一点光
    diffuse =diffuse * attenuation*intensity;


    return (ambient + diffuse);
}