#pragma once

class IGUI_Popup
{
public:
	IGUI_Popup() = default;
	virtual ~IGUI_Popup() = default;

    virtual void Render() = 0;

protected:
	bool m_is_active = false;
};

