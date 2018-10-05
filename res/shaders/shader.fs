#version 330 core

out vec4 FragColor;

//in vec4 vertexColor; //given from vertex shader
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2; //sampler2D loads texture from texture unit

void main()
{
    //FragColor = vertexColor; //Given from vertex shader
    //FragColor = ourColor; //uniform

    FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.2);
}
