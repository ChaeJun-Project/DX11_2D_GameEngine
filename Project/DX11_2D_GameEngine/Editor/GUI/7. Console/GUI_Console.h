#pragma once
#include "GUI/IGUI.h"
#include "Helper/IconProvider.h"
#include <DX11_2D_GameEngine_Lib/Logger.h>

class GUI_Console final : public IGUI
{
public:
    GUI_Console(const std::string& console_title);
    ~GUI_Console();

public:
    virtual void Initialize() override;
    virtual void Render() override;

private:
    void DisplayButton(const IconType& type, const ImVec2& button_size, bool& is_show);
    void ShowButtons();
    void ShowFilter();
    void ShowLog();
    void AddLog(const Log& log);
    void ClearLog();

private:
    std::shared_ptr<Logger> m_p_console_logger;
    std::deque<Log> m_log_deque;
    //Log�� �߰��ɼ��� ��ũ���� �������Ƿ� ��ũ���� �������� �߰��� Log�� ��ġ�� ��ġ��Ű�� ���� bool����
    bool is_update_scroll_pos = false; 

    bool is_show_info = true;
    bool is_show_warning = true;
    bool is_show_error = true;

    std::vector<ImVec4> m_log_color_vector
    {
        ImVec4(0.76f, 0.77f, 0.8f, 1.0f), // Info(Gray)
        ImVec4(1.0f, 1.0f, 0.0f, 1.0f),   // Warning(Yellow)
        ImVec4(1.0f, 0.0f, 0.0f, 1.0f)    // Error(Red)
    };

    ImGuiTextFilter m_log_filter;

    friend class EditorManager;
};


