#include <abpch.h>
#include <Above/Renderer/SubTexture2D.h>

namespace Above
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		:
		m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize )
	{

		const float x = coords.x, y = coords.y;
		const float cellWidth = cellSize.x, cellHeight = cellSize.y;
		const float sheetWidth = texture->GetWidth(), sheetHeight = texture->GetHeight();
		const float spriteWidth = spriteSize.x, spriteHeight = spriteSize.y;

		glm::vec2 min = { (x * cellWidth) / sheetWidth, (y * cellHeight) / sheetHeight };
		glm::vec2 max = { ((x + spriteWidth) * cellWidth) / sheetWidth, ((y + spriteHeight) * cellHeight) / sheetHeight };

		return CreateRef<SubTexture2D>(texture, min, max);
	}
}


