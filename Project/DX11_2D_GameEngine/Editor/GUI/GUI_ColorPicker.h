#pragma once

class GUI_ColorPicker final
{
public:
    GUI_ColorPicker(const std::string& title);
    ~GUI_ColorPicker();

    void Update();

public:
    const Vector4& GetColor() { return m_color; }
    void SetColor(const Vector4& color) { m_color = color; }

private:
    std::string m_title;
    Vector4 m_color = Vector4::White;
    bool m_is_active = false;
};

