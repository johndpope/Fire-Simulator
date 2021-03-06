#version 330 core
layout (location = 0) in vec3 position;
out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(position*1000, 1.0);
	gl_Position = clipPos.xyww;
    TexCoords = position;
}