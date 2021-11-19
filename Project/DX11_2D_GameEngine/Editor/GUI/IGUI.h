#pragma once

enum class GUIType : UINT
{
	MenuBar,
	ToolBar,
	Hierarchy,
	Scene,
	Game,
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
	virtual void Update() = 0;

public:
	virtual void Begin()
	{
		ImGui::Begin(m_gui_title.c_str(), &m_is_active, m_imgui_window_flags);
	}

	virtual void Render() = 0;

	virtual void End()
	{
		ImGui::End();
	}

public:

public:
	void SetIsActive(const bool& is_active) { m_is_active = is_active; }
    bool GetIsActive() const { return m_is_active; }

protected:
	std::string m_gui_title;

	UINT m_imgui_window_flags = 0;

	bool m_is_active = true;
};

