#include "stdafx.h"
#include "GUI_AudioSource.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/AudioSource.h>
#include <DX11_2D_GameEngine_Lib/AudioClip.h>

GUI_AudioSource::GUI_AudioSource(const std::string& audio_source_gui_name)
	:GUI_Component(audio_source_gui_name)
{
}

void GUI_AudioSource::Render()
{
	auto audio_source = m_select_game_object->GetComponent<AudioSource>();
	if (audio_source == nullptr)
		return;

	auto is_active = audio_source->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::AudioSource, is_active, IconType::Component_Audio_Source))
	{
		auto audio_source = m_select_game_object->GetComponent<AudioSource>();
		if (audio_source == nullptr)
			return;

		audio_source->SetIsActive(is_active);

		auto p_audio_clip = audio_source->GetAudioClip();
		std::string audio_clip_name;
		if (p_audio_clip != nullptr)
			audio_clip_name = p_audio_clip->GetResourceName();

		auto is_loop = audio_source->GetIsLoop();
		auto volume = audio_source->GetVolume();

		//Audio Clip
		ImGui::Text("Audio Clip");
		ImGui::SameLine(90.0f);
		ImGui::InputText("##AudioClipName", &audio_clip_name, ImGuiInputTextFlags_ReadOnly);
		if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Audio))
		{

		}

		//Loop
		if (ImGui::Checkbox("##Audio Loop", &is_loop))
		{
			if (CAN_EDIT)
				audio_source->SetIsLoop(is_loop);
		}
		ImGui::SameLine();
		ImGui::Text("Loop");

		//Volume
		ImGui::Text("Volume");
		ImGui::SameLine(90.0f);
		ImGui::PushItemWidth(120.0f);
		ImGui::SliderFloat("##Audio Volume", &volume, 0.0f, 1.0f);
		ImGui::PopItemWidth();

		//Set Data
		if (CAN_EDIT)
			audio_source->SetVolume(volume);

		DrawComponentEnd();
	}
}
