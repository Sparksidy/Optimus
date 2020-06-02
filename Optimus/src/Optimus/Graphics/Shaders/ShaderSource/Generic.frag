#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBlock {
    mat4 model;
    mat4 view;
    mat4 projection;

    int mode;
    mat3 textureMatrix;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
	if(ubo.mode == 0)
	{
	 	outColor = texture(texSampler, fragTexCoord);
	}
	else if(ubo.mode == 1)
	{
		outColor = texture(texSampler, (ubo.textureMatrix * vec3(fragTexCoord, 1)).xy);
	}
	else
	{
		outColor = vec4(vec3(1.0, 0.0, 0.0), 0.0);
	}
}