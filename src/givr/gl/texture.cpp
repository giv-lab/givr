#include "texture.h"
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using Texture = givr::Texture;

Texture::Texture(
) : m_textureID{0}
{
    alloc();
}
// TODO(lw): make a version that just receives the source directly.
Texture::~Texture() {
    dealloc();
}

void Texture::alloc()
{
    dealloc();
    glGenTextures(1, &m_textureID);
}
void Texture::dealloc()
{
    if (m_textureID) {
		//Temporarily disabled until better way to manage IDs is decided on
        //glDeleteBuffers(1, &m_textureID);		
    }
}

void Texture::bind(GLenum target)
{
    glBindTexture(target, m_textureID);
}

void Texture::load(GLenum target, std::string filename, GLint level, GLenum format)
{
	int width, height, comp;
	unsigned char *image = stbi_load(filename.c_str(),
		&width, &height, &comp, 0);

	//Calculate alignment
	int alignment = 4 - (width * comp) % 4;

	if ((image == nullptr) || (comp > 4)) {
		throw std::runtime_error("Failed to load texture");
	}
	else {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		const GLenum formats[4] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };

		glActiveTexture(GL_TEXTURE0);	//Bind to avoid disturbing active units
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, level, formats[comp - 1], width,
			height, 0, formats[comp - 1], GL_UNSIGNED_BYTE, image);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);		//Return to default
	}

	stbi_image_free(image);

}
