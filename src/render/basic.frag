#version 330 core

uniform sampler2D palette;
uniform vec3 light_ray_direction;
uniform float ambient_light;

in vec3 frag_normal;
in vec2 frag_uv;

out vec4 color;

void main(){

    vec4 base_color = texture(palette, frag_uv);
    float light = max(dot(normalize(frag_normal), normalize(light_ray_direction)), 0.0);
    color = base_color * (ambient_light + light);

    // debug color
    // color = vec4(normalize(frag_normal) * 0.5 + 0.5, 1.0);

}