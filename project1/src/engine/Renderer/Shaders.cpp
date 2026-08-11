#include "Shaders.h"

namespace quad
{
    const char* vertex = R"(
    #version 430 core

    layout (location = 0) in vec4 aPos;

    out vec2 texCoords;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
        texCoords = vec2(aPos.z, aPos.w);
    }
    )";
};

namespace render
{
	const char* fragment = R"(
    #version 430 core
    
    in vec2 texCoords;

    uniform sampler2D rTex;
    uniform sampler2D r_blurTex;
    uniform sampler2D gColor;
    uniform sampler2D ssTex;

    uniform float gamma = 2.2f;
    uniform bool isLight;

    out vec4 outColor;

    void main()
    {
        vec3 fragColor = texture(gColor, texCoords).rgb;
        vec3 directColor = texture(rTex, texCoords).rgb;
        vec3 blurColor = texture(r_blurTex, texCoords).rgb;
        vec3 indirectColor = texture(ssTex, texCoords).rgb;

        if (isLight)
        {
            vec3 lightColor = directColor + indirectColor;
            vec3 finalColor = lightColor + blurColor;

            vec3 mapped = finalColor / (finalColor + vec3(1.0f));
            mapped = pow(mapped, vec3(1.0f / gamma));

            outColor = vec4(mapped, 1.0f);
        }
        else
        {
            outColor = vec4(fragColor, 1.0f);
        }
     
    }

    )";
};

namespace pingpong
{
	const char* fragment = R"(
    #version 430 core

    layout (location = 0) out vec4 fragColor;

    in vec2 texCoords;

    uniform sampler2D image;

    uniform bool horizontal;

    uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

    void main()
    {             
        vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
        vec3 result = texture(image, texCoords).rgb * weight[0]; // current fragment's contribution
        if(horizontal)
        {
            for(int i = 1; i < 5; ++i)
            {
                result += texture(image, texCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
                result += texture(image, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            }
        }
        else
        {
            for(int i = 1; i < 5; ++i)
            {
                result += texture(image, texCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
                result += texture(image, texCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            }
        }
        fragColor = vec4(result, 1.0);
    }
    )";
};

namespace ss
{
	const char* fragment = R"(
    #version 430 core

    in vec2 texCoords;
    out vec4 outColor;

    uniform sampler2D gNormal;
    uniform sampler2D gColor;
    uniform sampler2D gMisc;

    uniform mat4 projection;

    void main()
    {
        vec3 fragColor = texture(gColor, texCoords).rgb;

        outColor = vec4(vec3(0.05f) * fragColor, 1.0);
    }
    )";
};

namespace g
{
    const char* fragment = R"(
    #version 430 core

    layout (location = 0) out vec4 rTex;
    layout (location = 1) out vec4 brightTex;

    in vec2 texCoords;
    vec4 outColor;

    struct Light
    {
        int type; // 0: parallel 1: point 2: spot

        vec4 position;
        vec4 direction;
        vec4 color;

        float constant;
        float linear;
        float quadratic;

        float cutOff;
        float outerCutOff;
        float strength;
    };

    layout(std430, binding = 0) buffer LightBuffer {
            Light lights[];
    };

    uniform uint lightCount;

    uniform float PI;

    uniform sampler2D gNormal;
    uniform sampler2D gColor;
    uniform sampler2D gMisc;
    uniform sampler2D gBright;

    uniform samplerCube skybox;
    uniform bool isSkyBox = false;
    uniform float skyboxMaxMipLevel;

    uniform mat4 projection;
    uniform mat4 view;

    float DistributionGGX(vec3 N, vec3 H, float roughness);
    float GeometrySchlickGGX(float NdotV, float roughness);
    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
    vec3 fresnelSchlick(float cosTheta, vec3 F0);

    void main()
    {
        vec3 fragColor = texture(gColor, texCoords).rgb;
        vec3 bright = texture(gBright, texCoords).rgb;
        
        float depth = texture(gMisc, texCoords).r;
        float metallic = texture(gMisc, texCoords).g;
        float roughness = texture(gMisc, texCoords).b;

        vec2 ndcPos = texCoords * 2.0 - 1.0;
        vec3 fragPos = vec3(
                ndcPos.x * (-depth) / (projection[0][0]), 
                ndcPos.y * (-depth) / (projection[1][1]),
                depth);

        vec3 light_Out = vec3(0.0f);
        vec3 lightPos = vec3(0.0f);
        vec3 N = texture(gNormal, texCoords).rgb;
        vec3 L = vec3(0.0f);
        vec3 H = vec3(0.0f);
        vec3 V = normalize(-fragPos);

        float attenuation = 1.0f;
        float intensity = 1.0f;

        vec3 F0 = vec3(0.04); 
        F0 = mix(F0, fragColor, metallic);

        for (uint i = 0; i < lightCount; i++)
        {
            if (lights[i].type == 0)
            {
                L = -normalize(mat3(view) * lights[i].direction.xyz);
            }
            else
            {
                lightPos = vec3(view * vec4(lights[i].position.xyz, 1.0f));
                L = normalize(lightPos - fragPos);

                float distance = length(lightPos - fragPos);
                attenuation = 1.0f / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);

                if (lights[i].type == 2)
                {
                    float theta = dot(L, -normalize(mat3(view) * lights[i].direction.xyz));
                    float epsilon   = lights[i].cutOff - lights[i].outerCutOff;
                    intensity = clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);   
                }
                
            }

            H = normalize(V + L);

            float NDF = DistributionGGX(N, H, roughness);   
            float G   = GeometrySmith(N, V, L, roughness);      
            vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

            vec3 numerator    = NDF * G * F; 
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
            vec3 specular = numerator / denominator;

            vec3 kS = F;
            vec3 kD = vec3(1.0) - kS;
            kD *= 1.0 - metallic;	 

            float NdotL = max(dot(N, L), 0.0);

            light_Out += bright * lights[i].strength + (kD * (fragColor / PI) * intensity + kS * specular) * lights[i].color.xyz * lights[i].strength * attenuation * NdotL;
        }    

        outColor = vec4(light_Out, 1.0f);

        if (isSkyBox)
        {
            vec3 R = reflect(-V, N);
            vec3 skybox_color = textureLod(skybox, R, roughness * skyboxMaxMipLevel).rgb;
    
            vec3 kS_sb = fresnelSchlick(max(dot(N, V), 0.0), F0);

            outColor += vec4(mix(vec3(0.0f), skybox_color, kS_sb), 0.0f);
        }

        rTex = outColor;

        brightTex = vec4(0.0, 0.0, 0.0, 1.0);
        float brightness = dot(outColor.rgb, vec3(0.2126, 0.7152, 0.0722));
        if(brightness > 1.0)
            brightTex = vec4(outColor.rgb, 1.0);

    }

    float DistributionGGX(vec3 N, vec3 H, float roughness)
    {
        float a = roughness*roughness;
        float a2 = a*a;
        float NdotH = max(dot(N, H), 0.0);
        float NdotH2 = NdotH*NdotH;

        float nom   = a2;
        float denom = (NdotH2 * (a2 - 1.0) + 1.0);
        denom = PI * denom * denom;

        return nom / denom;
    }

    float GeometrySchlickGGX(float NdotV, float roughness)
    {
        float r = (roughness + 1.0);
        float k = (r*r) / 8.0;

        float nom   = NdotV;
        float denom = NdotV * (1.0 - k) + k;

        return nom / denom;
    }

    float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
    {
        float NdotV = max(dot(N, V), 0.0);
        float NdotL = max(dot(N, L), 0.0);
        float ggx2 = GeometrySchlickGGX(NdotV, roughness);
        float ggx1 = GeometrySchlickGGX(NdotL, roughness);

        return ggx1 * ggx2;
    }

    vec3 fresnelSchlick(float cosTheta, vec3 F0)
    {
        return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
    }

    )";
};

namespace mesh
{
    const char* vertex = R"(
    #version 430 core

    layout (location = 0) in vec4 aPos;
    layout (location = 1) in vec4 aTexCoords;
    layout (location = 2) in vec4 aNormal;
    layout (location = 3) in vec4 aColor;
    layout (location = 4) in vec4 aTangent;

    out vec2 texCoords;
    out vec3 color;
    out float depth;

    out vec3 N;
    out mat3 TBN;

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    void main()
    {
        mat4 modelView = view * model;
        vec4 viewFragPos = modelView * vec4(aPos.xyz, 1.0);
        gl_Position = projection * viewFragPos;

        texCoords = vec2(aTexCoords.xy);

        mat3 normalMat = mat3(transpose(inverse(modelView)));
        N = normalize(normalMat * aNormal.xyz);
        vec3 T = normalize(normalMat * aTangent.xyz);

        T = normalize(T - dot(T, N) * N);

        vec3 B = cross(T, N);

        TBN = mat3(T, B, N);

        color = vec3(aColor.xyz);

        depth = viewFragPos.z;
    }
    )";

    const char* fragment = R"(
    #version 430 core

    layout (location = 0) out vec4 gNormal;
    layout (location = 1) out vec4 gColor;
    layout (location = 2) out vec4 gMisc;
    layout (location = 3) out vec4 gBright;

    in vec2 texCoords;
    in vec3 color;
    in float depth;

    in vec3 N;
    in mat3 TBN;

    uniform float epsilon;

    uniform sampler2D texture_albedo;
    uniform sampler2D texture_normal;
    uniform sampler2D texture_metallic;
    uniform sampler2D texture_roughness;

    uniform float metallic;
    uniform float roughness;

    uniform bool isTex;
    uniform bool isRed = false;

    uniform vec3 bright;

    void main()
    {
        vec3 fragColor;
        vec3 normal;

        float misc_g;
        float misc_b;

        if (isTex)
        {
            if (isRed)
            {
                fragColor = vec3(texture(texture_albedo, texCoords).r);

                misc_g = texture(texture_metallic, texCoords).r;
                misc_b = texture(texture_roughness, texCoords).r;
            }
            else
            {
               vec4 temp = texture(texture_albedo, texCoords);
               if (temp.a < epsilon)
               {
                    discard;      
               }
               fragColor = temp.xyz;
            }

            normal = texture(texture_normal ,texCoords).rgb;
            normal = normalize(normal * 2.0 - 1.0); 
            normal = normalize(TBN * normal);
        }
        else
        {
            fragColor = color;
            normal = N;

            misc_g = metallic;
            misc_b = roughness;
        }

        gColor.rgb = fragColor;
        gNormal.rgb = normal;
        gMisc.r = depth;
        gMisc.g = misc_g;
        gMisc.b = misc_b;
        gBright.rgb = bright;
    }
    )";
};

namespace skybox
{
    const char* vertex = R"(
    #version 430 core

    layout (location = 0) in vec3 aPos;

    out vec3 texCoords;

    uniform mat4 projection;
    uniform mat4 view;

    void main()
    {
        texCoords = aPos;
        vec4 pos = projection * vec4(mat3(view) * aPos, 1.0);
        gl_Position = pos.xyww;
    }  
    )";

	const char* fragment = R"(
    #version 430 core

    out vec4 outColor;

    in vec3 texCoords;

    uniform samplerCube skybox;

    void main()
    {
        outColor = texture(skybox, texCoords);
    }
    )";

};