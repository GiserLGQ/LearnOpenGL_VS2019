#version 330 core
out vec4 FragColor;
  
in vec3 Normal;//顶点法向量
in vec3 FragPos;//片段位置(顶点)

uniform vec3 objectColor;//物体本身的颜色
uniform vec3 lightColor;//光颜色
uniform vec3 lightPos;//光源位置
uniform vec3 viewPos;//视角位置

void main()
{
    float ambientStrength = 0.1;

    vec3 ambient = ambientStrength * lightColor;

    //vec3 result = ambient * objectColor;

    //FragColor = vec4(result, 1.0);

//标准化法向量和方向向量
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);

//计算光源对当前片段实际的漫反射影响。结果值再乘以光的颜色，得到漫反射分量
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

//有了环境光分量和漫反射分量，我们把它们相加，然后把结果乘以物体的颜色，来获得片段最后的输出颜色
//vec3 result = (ambient + diffuse) * objectColor;
//FragColor = vec4(result, 1.0);


//定义一个镜面强度
float specularStrength =2.0;

//计算视线方向向量，和对应的沿着法线轴的反射向量
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);

//计算镜面分量
float spec = pow(max(dot(viewDir, reflectDir), 0.0),256);
vec3 specular = specularStrength * spec * lightColor;

//最后一件事情是把它加到环境光分量和漫反射分量里，再用结果乘以物体的颜色
vec3 result = (ambient + diffuse + specular) * objectColor;
FragColor = vec4(result, 1.0);

}