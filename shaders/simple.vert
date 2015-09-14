#version 330

//TODO: receive a uniform value
uniform float timeVal;
uniform float xOffset;
uniform float yOffset;
uniform float theta;

in vec2 pos;
in vec3 color;
in float speed;

out vec4 smoothColor;

void main()
{
	gl_Position = vec4(pos.x + xOffset * speed, pos.y + yOffset * speed, 0, 1);
	smoothColor = vec4(color, 1);
	
	//TODO: make an animation!
	//could use the frag shader also
    smoothColor = vec4(color.r*0.5*cos(timeVal)+0.5, color.gb*0.5*cos(timeVal)+0.5, 1);

}
