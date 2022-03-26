#include "stdafx.h"
#include "GUI_AudioSource.h"

#include "GUI/Module/ItemList/GUI_ItemList.h"

#include <DX11_2D_GameEngine_Lib/SceneManager.h>

#include <DX11_2D_GameEngine_Lib/GameObject.h>
#include <DX11_2D_GameEngine_Lib/AudioSource.h>
#include <DX11_2D_GameEngine_Lib/AudioClip.h>

GUI_AudioSource::GUI_AudioSource(const std::string& audio_source_gui_name)
	:GUI_Component(audio_source_gui_name)
{
	m_p_audio_clip_list = std::make_unique<GUI_ItemList>();
}

GUI_AudioSource::~GUI_AudioSource()
{
	m_p_audio_clip_list.reset();
}

void GUI_AudioSource::Render()
{
	auto audio_source = m_p_selected_game_object->GetComponent<AudioSource>();
	if (audio_source == nullptr)
		return;

	auto is_active = audio_source->GetIsActive();
	if (BeginComponent(m_component_gui_name, ComponentType::AudioSource, is_active, IconType::Component_AudioSource))
	{
		auto audio_source = m_p_selected_game_object->GetComponent<AudioSource>();
		if (audio_source == nullptr)
			return;

		audio_source->SetIsActive(is_active);

		//Audio Clip
		ImGui::Text("Audio Clip List");
		ImGui::SameLine();

		const auto& audio_clip_map = audio_source->GetAudioClipMap();

		auto p_current_audio_clip = audio_source->GetCurrentAudioClip();
		std::string current_animation_name;
		if (p_current_audio_clip != nullptr)
			current_animation_name = p_current_audio_clip->GetResourceName();

		int index = 0;

		for (const auto& audio_clip : audio_clip_map)
		{
			m_p_audio_clip_list->AddItem(audio_clip.second->GetResourceName());

			if (p_current_audio_clip)
			{
				if (audio_clip.second->GetResourceName() == current_animation_name)
					m_p_audio_clip_list->SetCurrentListID(index);

				else
					++index;
			}
		}

		const auto& audio_clip_item_list_vector = m_p_audio_clip_list->GetItemList();

		//Show Added Audio Clip List Combo
		ImGui::PushItemWidth(150.0f);
		if (ImGui::BeginCombo("##Audio Clip List", current_animation_name.c_str()))
		{
			for (UINT i = 0; i < static_cast<UINT>(audio_clip_item_list_vector.size()); ++i)
			{
				const bool is_selected = (m_p_audio_clip_list->GetCurrentListID() == i);
				if (ImGui::Selectable(audio_clip_item_list_vector[i].c_str(), is_selected))
				{
					audio_source->SetCurrentAudioClip(audio_clip_item_list_vector[i]);
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		m_p_audio_clip_list->ClearItemList();
		ImGui::PopItemWidth();
		if (CAN_EDIT)
		{
			if (auto pay_load = DragDropEvent::ReceiveDragDropPayLoad(PayLoadType::Audio))
			{
				auto p_audio_clip = RESOURCE_MANAGER->LoadFromFile<AudioClip>(std::get<std::string>(pay_load->data));

				if (p_audio_clip != nullptr)
					audio_source->AddAudioClip(p_audio_clip->GetResourceName());
			}
		}

		auto is_loop = audio_source->GetIsLoop();
		auto volume = audio_source->GetVolume();

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
