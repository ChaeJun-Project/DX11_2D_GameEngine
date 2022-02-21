#pragma once

enum class GUIType : UINT
{
	MenuBar,
	ToolBar,
	Hierarchy,
	Scene,
	Inspector,
	Project,
	Console,
};

class IGUI
{
public:
	IGUI(const std::string& gui_title)
		:m_gui_title(gui_title)
	{

	}
	virtual ~IGUI() = default;

public:
    virtual void Initialize() {};
	virtual void Update() {};

public:
	virtual void Begin()
	{
		if (m_gui_size.x != 0.0f && m_gui_size.y != 0.0f)
		{

		}

		ImGui::Begin(m_gui_title.c_str(), &m_is_active, m_window_flags);
	}

	virtual void Render() = 0;

	virtual void End()
	{
		CheckResize();

		ImGui::End();
	}

public:
	void SetIsActive(const bool& is_active) { m_is_active = is_active; }
    bool GetIsActive() const { return m_is_active; }

private:
	void CheckResize()
	{
		ImVec2 current_gui_size = ImGui::GetWindowSize();

		if (m_gui_size.x != current_gui_size.x || m_gui_size.y != current_gui_size.y)
		{
			m_is_resize = true;
		}

		else
			m_is_resize = false;

		m_gui_size = current_gui_size;
	}

protected:
	std::string m_gui_title;

	bool m_is_active = true;

	ImGuiWindowFlags m_window_flags = 0;

	ImVec2 m_gui_size = ImVec2(0.0f, 0.0f);

	bool m_is_resize = false;
	
	friend class EditorManager;
};

