#vs
#version 430 core

layout (location = 0) in vec3 a_position;

void main()
{
	gl_Position = vec4(a_position, 1.0);
}

#gs
#version 430 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

layout (location = 0) uniform mat4 u_mvp;
layout (location = 1) uniform vec3 u_cam_pos;
layout (location = 2) uniform vec4 u_foliage_rect;

layout (location = 0) out vec2 v_uv;

void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;

	vec3 to_camera = normalize(u_cam_pos - pos);
	vec3 up = vec3(0,1,0);
	vec3 right = normalize(cross(to_camera, up));

	vec3 p0 = vec3(pos.x - 0.5, 1, pos.z);
	vec3 p1 = vec3(pos.x - 0.5, 0, pos.z);
	vec3 p2 = vec3(pos.x + 0.5, 1, pos.z);
	vec3 p3 = vec3(pos.x + 0.5, 0, pos.z);

	gl_Position = u_mvp * vec4(p0, 1.0);
	v_uv = vec2(u_foliage_rect.x, 1);
	EmitVertex();

	gl_Position = u_mvp * vec4(p1, 1.0);
	v_uv = vec2(u_foliage_rect.x, 0);
	EmitVertex();

	gl_Position = u_mvp * vec4(p2, 1.0);
	v_uv = vec2(u_foliage_rect.z, 1);
	EmitVertex();

	gl_Position = u_mvp * vec4(p3, 1.0);
	v_uv = vec2(u_foliage_rect.z, 0);
	EmitVertex();

	EndPrimitive();
}

#fs
#version 430 core


layout (location = 3) uniform sampler2D u_texture;

layout (location = 0) in vec2 v_uv;
layout (location = 0) out vec4 out_color;

void main()
{
	out_color = texture2D(u_texture, v_uv);

	if(out_color.a < 0.5)
	{
		discard;
	}
}