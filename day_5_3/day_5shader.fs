#version 330 core
out vec4 FragColor;

in vec3 myColor;
in vec2 TexCoord;
// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec2 toumingdu;
void main()
{
   // FragColor =  min(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3) * vec4(myColor.x, toumingdu.x, myColor.z, 1.0f);
   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3) * vec4(myColor.x, toumingdu.x, myColor.z, 1.0f);
}