#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

uniform float nx = 1.0f; //Masukan untuk merubah jumlah frame animasi
uniform float ny = 1.0f; //Masukan untuk merubah jumlah jenis aplikasi
uniform int frameIndex = 0; //Masukan untuk menjalankan animasi
uniform int animIndex = 0; //Masukan untuk mengganti jenis aplikasi
uniform mat4 transform; //Masukan untuk transformasi dari penjelasan dian

out vec3 ourColor; //Keluaran yang akan di pakai fragment shader untuk menampilkan warna
out vec2 TexCoord; //Keluaran yang akan di pakai fragment shader untuk menampilkan texture

void main()
{
	gl_Position = transform * vec4(position, 1.0f); //Mengisi transformasi
	ourColor = color;  //Mengisi warna
	TexCoord = vec2((nx * (texCoord.x + frameIndex)), ny*(1.0f - texCoord.y+animIndex)); //Perhitungan perubahan texture
}