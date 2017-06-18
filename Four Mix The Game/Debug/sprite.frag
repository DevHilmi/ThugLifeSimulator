#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

// Texture samplers
uniform sampler2D ourTexture;

void main()
{
    gl_FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);
}