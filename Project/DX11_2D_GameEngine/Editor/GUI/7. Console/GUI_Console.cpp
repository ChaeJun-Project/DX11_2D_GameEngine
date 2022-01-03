#include "stdafx.h"
#include "GUI_Console.h"

#include <DX11_2D_GameEngine_Lib/LogManager.h>

GUI_Console::GUI_Console(const std::string& console_title)
	:IGUI(console_title)
{
}

GUI_Console::~GUI_Console()
{
	m_p_console_logger.reset();

	ClearLog();

	m_log_color_vector.clear();
	m_log_color_vector.shrink_to_fit();
}

void GUI_Console::Initialize()
{
	m_p_console_logger = std::make_shared<Logger>("Editor_Log.txt");
	m_p_console_logger->SetCallBack
	(
		std::bind
		(
			&GUI_Console::AddLog,
			this,
			std::placeholders::_1
		)
	);

	LOG_MANAGER->SetLogger(m_p_console_logger);
}

void GUI_Console::Update()
{
	if (KEY_PRESS(KeyCode::KEY_CONTROL) && KEY_DOWN(KeyCode::KEY_C))
	{
		m_is_active = !m_is_active;
	}
}

void GUI_Console::Render()
{
    //Show Buttons(Clear, Info, Warning, Error)
	ShowButtons();
	//Show Filter
	ShowFilter();
	//���м�
	ImGui::Separator();
	//Show Log
	ShowLog();
}

void GUI_Console::DisplayButton(const IconType& type, const ImVec2& button_size, bool& is_show)
{
	auto icon_provider = IconProvider::GetInstance();

	//���� Editor ���°� Play��� Play Button�� ������ Active �������� ����
	ImGui::PushStyleColor
	(
		ImGuiCol_Button,
		ImGui::GetStyle().Colors[is_show ? ImGuiCol_ButtonActive : ImGuiCol_Button]
	);

	//Scene Play Button �׸���
	if (icon_provider->CreateImageButton(type, button_size))
	{
		is_show = !is_show;
	}

	ImGui::PopStyleColor();
}

void GUI_Console::ShowButtons()
{
	//Log Clear
	if (ImGui::Button("Clear"))
		ClearLog();
	ImGui::SameLine();

	//Log Info
	DisplayButton(IconType::Console_Info, ImVec2(14.0f, 14.0f), is_show_info);
	ImGui::SameLine();

	//Log Warning
	DisplayButton(IconType::Console_Warning, ImVec2(14.0f, 14.0f), is_show_warning);
	ImGui::SameLine();

	//Log Error
	DisplayButton(IconType::Console_Error, ImVec2(14.0f, 14.0f), is_show_error);
	ImGui::SameLine();
}

void GUI_Console::ShowFilter()
{
	//Filter
	m_log_filter.Draw("##Filter", 400.0f);
}

void GUI_Console::ShowLog()
{
    //Log
	ImGui::BeginChild("Log", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	{
		for (const auto& log : m_log_deque)
		{
			//Filter�� ���� �˻����� �� ��ġ���� �ʴٸ� �������� ����
			if (!m_log_filter.PassFilter(log.text.c_str()))
				continue;

			if ((log.type == LogType::Info && is_show_info) ||
				(log.type == LogType::Warning && is_show_warning) ||
				(log.type == LogType::Error && is_show_error))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, m_log_color_vector[static_cast<UINT>(log.type)]);
				ImGui::BulletText(log.text.c_str());
				ImGui::PopStyleColor();
			}
		}

		if (is_update_scroll_pos)
		{
			ImGui::SetScrollHereY();
			is_update_scroll_pos = false;
		}
	}
	ImGui::EndChild();
}

void GUI_Console::AddLog(const Log& log)
{
	m_log_deque.emplace_back(log);

	is_update_scroll_pos = true;
}

void GUI_Console::ClearLog()
{
	m_log_deque.clear();
	m_log_deque.shrink_to_fit();
}
