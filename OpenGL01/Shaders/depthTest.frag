#version 330 core

out vec4 FragColor;

float near = 0.1;
float far = 100.0;

// 反算真实的深度值
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    // gl_FragCoord.z 中存储了深度信息
    float depth = LinearizeDepth(gl_FragCoord.z) / far;

    FragColor = vec4(vec3(depth), 1.0);
}