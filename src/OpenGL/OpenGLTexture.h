#pragma once

#include "agi/Texture.h"

#include <glad/glad.h>

namespace AGI {

	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(uint32_t width, uint32_t height, uint16_t channels);
		virtual ~OpenGLTexture();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((OpenGLTexture&)other).m_RendererID;
		}
	private:
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		GLenum m_InternalFormat, m_DataFormat;
	};

}