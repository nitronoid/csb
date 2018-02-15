#version 420                                            // Keeping you on the bleeding edge!
#extension GL_EXT_gpu_shader4 : enable

// This is no longer a built-in variable
layout(location=0) out vec4 FragColor;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;



in FragData
{
    vec3 vert;
    vec3 norm;
    vec2 uv;
    vec3 wireframeDist;
} inData;

void main(void)
{
    vec3 d = fwidth(inData.wireframeDist);

    vec3 a3 = smoothstep(vec3(0.0), d * 1.5, inData.wireframeDist);
    float edgeFactor = min(min(a3.x, a3.y), a3.z);

    FragColor = vec4(1.0, 1.0, 1.0, (1.0-edgeFactor)*0.95);
}
