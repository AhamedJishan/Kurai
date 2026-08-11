#include "Viewport.h"

#include <imgui/imgui.h>
#include <Dawn/Rendering/Texture.h>

namespace Dawn
{
	namespace Editor
	{
		void BeginViewport(Texture* viewportTexture)
		{
			ImGui::Begin("Viewport");

			ImVec2 availRegionSize = ImGui::GetContentRegionAvail();
			float dstAspect = availRegionSize.x / availRegionSize.y;
			float srcAspect = static_cast<float>(viewportTexture->GetWidth()) / viewportTexture->GetHeight();

			ImVec2 imageSize = availRegionSize;

			if (srcAspect > dstAspect)
				imageSize.y = availRegionSize.x / srcAspect;
			else
				imageSize.x = availRegionSize.y * srcAspect;

			ImVec2 cursor = ImGui::GetCursorPos();
			cursor.x += (availRegionSize.x - imageSize.x) * 0.5f;
			cursor.y += (availRegionSize.y - imageSize.y) * 0.5f;
			ImGui::SetCursorPos(cursor);

			ImGui::Image((ImTextureID)(intptr_t)(viewportTexture->GetId()), imageSize);
		}

		void EndViewport()
		{
			ImGui::End();
		}
	}
}