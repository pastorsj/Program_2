#version 330

//TODO: receive a uniform value
uniform float timeVal;

in vec2 pos;
in vec3 color;

out vec4 smoothColor;

void main()
{
	gl_Position = vec4(pos, 0, 1);
	smoothColor = vec4(color, 1);
	
	//TODO: make an animation!
	//could use the frag shader also
    smoothColor = vec4(color.r*0.5*cos(timeVal)+0.5, color.gb*0.5*cos(timeVal)+0.5, 1);

}