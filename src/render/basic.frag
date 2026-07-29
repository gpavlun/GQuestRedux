#version 330 core

uniform sampler2D palette;

in vec2 frag_uv;

out vec4 color;

void main(){

    color = texture(palette, frag_uv);
}