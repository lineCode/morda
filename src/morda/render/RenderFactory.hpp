#pragma once

#include <vector>

#include <kolme/Vector2.hpp>

#include <utki/Shared.hpp>
#include <utki/Buf.hpp>

#include "Texture2D.hpp"

#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"

#include "ShaderPosTex.hpp"
#include "ShaderColorPos.hpp"
#include "ShaderPosClr.hpp"
#include "ShaderColorPosTex.hpp"
#include "FrameBuffer.hpp"

namespace morda{

class RenderFactory : public utki::Unique{
protected:
	RenderFactory(){}
	
public:
	virtual ~RenderFactory()noexcept{}	
	
	virtual std::shared_ptr<Texture2D> createTexture2D(Texture2D::TexType_e type, kolme::Vec2ui dim, const utki::Buf<std::uint8_t>& data) = 0;
	
	std::shared_ptr<Texture2D> createTexture2D(kolme::Vec2ui dim, const utki::Buf<std::uint32_t>& data);
	
	virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const utki::Buf<kolme::Vec4f> vertices) = 0;
	
	virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const utki::Buf<kolme::Vec3f> vertices) = 0;
	
	virtual std::shared_ptr<VertexBuffer> createVertexBuffer(const utki::Buf<kolme::Vec2f> vertices) = 0;
	
	virtual std::shared_ptr<IndexBuffer> createIndexBuffer(const utki::Buf<std::uint16_t> indices) = 0;
	
	virtual std::shared_ptr<VertexArray> createVertexArray(
			std::vector< std::shared_ptr<morda::VertexBuffer> >&& buffers,
			std::shared_ptr<morda::IndexBuffer> indices,
			VertexArray::Mode_e mode
		) = 0;
	
	struct Shaders : public utki::Unique{
		std::unique_ptr<ShaderPosTex> posTex;
		std::unique_ptr<ShaderColorPos> colorPos;
		std::unique_ptr<ShaderPosClr> posClr;
		std::unique_ptr<ShaderColorPosTex> colorPosTex;
	};
	
	virtual std::unique_ptr<Shaders> createShaders() = 0;
	
	virtual std::shared_ptr<FrameBuffer> createFramebuffer(std::shared_ptr<Texture2D> color) = 0;
};

}
