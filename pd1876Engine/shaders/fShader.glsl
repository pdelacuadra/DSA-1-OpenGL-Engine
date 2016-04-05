#version 430

out vec4 fragColor;
in vec2 fragUV;
uniform sampler2D myTexture;

void main()
{
	fragColor = texture(myTexture, fragUV);
}