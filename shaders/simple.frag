#version 330

in vec4 smoothColor;
out vec4 fragColor;

void main()
{
	//TODO: make an animation
	//could use the vertex shader also
	
	fragColor = vec4(smoothColor.r * 3, smoothColor.g * 3, smoothColor.b * 3, smoothColor.a);
}
