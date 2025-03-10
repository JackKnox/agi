#include "OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace AGI {

	void OpenGLRenderAPI::Init(GLADloadproc loadfunc)
	{
		int status = gladLoadGLLoader(loadfunc);
		AGI_ASSERT(status, "Failed to initialize Glad!");

		AGI_INFO("OpenGL Info:");
		AGI_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		AGI_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
		AGI_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRenderAPI::SetClearColour(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void OpenGLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}