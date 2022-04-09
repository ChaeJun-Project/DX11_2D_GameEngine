#include "stdafx.h"
#include "InputManager.h"

#include "Core.h"

#include "FontManager.h"

InputManager::InputManager()
{
	MouseProc = std::bind
	(
		&InputManager::MsgProc,
		this,
		std::placeholders::_1,
		std::placeholders::_2,
		std::placeholders::_3,
		std::placeholders::_4
	);
}

void InputManager::Initialize()
{
	//Key
	m_virtual_key_vector =
	{
		 VK_LEFT,	//Arrow Left,
		 VK_UP,		//Arrow Up,
		 VK_RIGHT,	//Arrow Right,
		 VK_DOWN,	//Arrow Down,

		 VK_F1,
		 VK_F2,

		 0x30, //0
		 0x31, //1
		 0x32, //2
		 0x33, //3
		 0x34, //4
		 0x35, //5
		 0x36, //6
		 0x37, //7
		 0x38, //8
		 0x39, //9
		 0x41, //A
		 0x42, //B
		 0x43, //C
		 0x44, //D
		 0x45, //E
		 0x46, //F
		 0x47, //G
		 0x48, //H
		 0x49, //I
		 0x4A, //J
		 0x4B, //K
		 0x4C, //L
		 0x4D, //M
		 0x4E, //N
		 0x4F, //O
		 0x50, //P
		 0x51, //Q
		 0x52, //R
		 0x53, //S
		 0x54, //T
		 0x55, //U
		 0x56, //V
		 0x57, //W
		 0x58, //X
		 0x59, //Y
		 0x5A, //Z

		 VK_SHIFT,	 //Shift
		 VK_CONTROL, //Control
		 VK_MENU,	 //Alt
		 VK_SPACE,	 //Space
		 VK_RETURN,	 //Enter
		 VK_DELETE,	 //Delete
	};

	for (UINT i = 0; i < static_cast<UINT>(Key::END); ++i)
	{
		m_key_vector.emplace_back(KeyInfo{ KeyState::KEY_INPUT_STATE_NONE, false });
	}

	//Mouse
	m_virtual_button_vector =
	{
		VK_LBUTTON,//Mouse Left Button
		VK_RBUTTON,//Mouse Right Button 
	};

	for (UINT i = 0; i < static_cast<UINT>(Button::END); ++i)
	{
		m_button_vector.emplace_back(ButtonInfo{ ButtonState::BUTTON_INPUT_STATE_NONE, 0, 0 });
	}

	m_time_double_click = GetDoubleClickTime();
}

void InputManager::Update()
{
	//윈도우 포커싱 여부 확인
	HWND hWnd = GetFocus();

	//윈도우 포커싱 중일 때 키 이벤트 동작
	if (hWnd != nullptr)
	{
		//Key
		for (UINT i = 0; i < static_cast<UINT>(Key::END); ++i)
		{
			//현재 키가 눌려있을 경우
			if (GetAsyncKeyState(m_virtual_key_vector[i]) & 0x8000)
			{
				//이전 프레임에 눌려있었을 경우
				if (m_key_vector[i].m_is_pre_input)
					m_key_vector[i].m_key_state = KeyState::KEY_INPUT_STATE_PRESS;

				//이전 프레임에 눌려있지 않았을 경우
				else
					m_key_vector[i].m_key_state = KeyState::KEY_INPUT_STATE_DOWN;

				m_key_vector[i].m_is_pre_input = true;
			}

			//현재 키가 안 눌려있을 경우
			else
			{
				//이전 프레임에 눌려있었을 경우
				if (m_key_vector[i].m_is_pre_input)
					m_key_vector[i].m_key_state = KeyState::KEY_INPUT_STATE_UP;

				//이전 프레임에 눌려있지 않았을 경우
				else
					m_key_vector[i].m_key_state = KeyState::KEY_INPUT_STATE_NONE;

				m_key_vector[i].m_is_pre_input = false;
			}
		}

		//Mouse Button
		for (UINT i = 0; i < static_cast<UINT>(Button::END); ++i)
		{
			//현재 마우스 버튼이 눌려있을 경우
			if (GetAsyncKeyState(m_virtual_button_vector[i]) & 0x8000)
			{
				//마우스 버튼이 처음 클릭된 상태라면
				if (m_button_vector[i].m_click_count == 0)
				{
					m_button_vector[i].m_button_state = ButtonState::BUTTON_INPUT_STATE_DOWN;
					++m_button_vector[i].m_click_count;
				}

				//이전 프레임에 마우스 버튼이 눌려져 있었을 경우
				else if (m_button_vector[i].m_click_count == 1)
					m_button_vector[i].m_button_state = ButtonState::BUTTON_INPUT_STATE_PRESS;
			}

			//현재 마우스 버튼이 안 눌려있을 경우
			else
			{
				//이전 프레임에 마우스 버튼이 눌려져 있었을 경우
				if (m_button_vector[i].m_click_count == 1)
				{
					m_button_vector[i].m_button_state = ButtonState::BUTTON_INPUT_STATE_UP;
					--m_button_vector[i].m_click_count;
				}

				//이전 프레임에 눌려있지 않았을 경우
				else if (m_button_vector[i].m_click_count == 0)
					m_button_vector[i].m_button_state = ButtonState::BUTTON_INPUT_STATE_NONE;
			}
		}

		//오류 있음 수정해야 함
		//Check Mouse Double Click
		/*DWORD button_state = GetTickCount();
		for (UINT i = 0; i < static_cast<UINT>(Button::END); ++i)
		{
			if (m_button_vector[i].m_button_state == ButtonState::BUTTON_INPUT_STATE_DOWN)
			{
				if (m_button_vector[i].m_click_count == 1)
				{
					if ((button_state - m_button_vector[i].m_start_double_click) >= m_time_double_click)
						m_button_vector[i].m_click_count = 0;
				}
				++m_button_vector[i].m_click_count;

				if (m_button_vector[i].m_click_count == 1)
					m_button_vector[i].m_start_double_click = button_state;
			}

			if (m_button_vector[i].m_button_state == ButtonState::BUTTON_INPUT_STATE_UP)
			{
				if (m_button_vector[i].m_click_count == 1)
				{
					if ((button_state - m_button_vector[i].m_start_double_click) >= m_time_double_click)
						m_button_vector[i].m_click_count = 0;
				}
				else if (m_button_vector[i].m_click_count == 2)
				{
					if ((button_state - m_button_vector[i].m_start_double_click) <= m_time_double_click)
						m_button_vector[i].m_button_state = ButtonState::BUTTON_INPUT_STATE_DBLCLK;

					m_button_vector[i].m_click_count = 0;
				}
			}
		}*/

		//Mouse Wheel
		POINT point = { 0, 0 };
		GetCursorPos(&point);
		auto settings = Core::GetInstance()->GetSettings();
		ScreenToClient(settings->GetWindowHandle(), &point);

		m_mouse_wheel_old_state.x = m_mouse_wheel_state.x;
		m_mouse_wheel_old_state.y = m_mouse_wheel_state.y;

		m_mouse_wheel_state.x = static_cast<float>(point.x);
		m_mouse_wheel_state.y = static_cast<float>(point.y);

		m_mouse_wheel_move_value = m_mouse_wheel_state - m_mouse_wheel_old_state;
		m_mouse_wheel_old_state.z = m_mouse_wheel_state.z;
	}

	//윈도우 포커싱 해제상태
	else
	{
		//Key
		for (UINT i = 0; i < static_cast<UINT>(Key::END); ++i)
		{
			m_key_vector[i].m_is_pre_input = false;

			if (m_key_vector[i].m_key_state == KeyState::KEY_INPUT_STATE_DOWN
				|| m_key_vector[i].m_key_state == KeyState::KEY_INPUT_STATE_PRESS
				)
				m_key_vector[i].m_key_state = KeyState::KEY_INPUT_STATE_UP;

			else if (m_key_vector[i].m_key_state == KeyState::KEY_INPUT_STATE_UP)
				m_key_vector[i].m_key_state = KeyState::KEY_INPUT_STATE_NONE;
		}

		//Mouse
		for (UINT i = 0; i < static_cast<UINT>(Button::END); ++i)
		{
			if (m_button_vector[i].m_button_state == ButtonState::BUTTON_INPUT_STATE_DOWN
				|| m_button_vector[i].m_button_state == ButtonState::BUTTON_INPUT_STATE_PRESS
				|| m_button_vector[i].m_button_state == ButtonState::BUTTON_INPUT_STATE_DBLCLK)
			
				m_button_vector[i].m_button_state = ButtonState::BUTTON_INPUT_STATE_UP;
			
			//이전 프레임에 눌려있지 않았을 경우
			else if (m_button_vector[i].m_button_state == ButtonState::BUTTON_INPUT_STATE_UP)
				m_button_vector[i].m_button_state = ButtonState::BUTTON_INPUT_STATE_NONE;

			m_button_vector[i].m_click_count = 0;
			m_button_vector[i].m_start_double_click = 0;
		}
	}
}

void InputManager::Render()
{
	FONT_MANAGER->DrawFont(m_render_str, 20.0f, FONT_RGBA(0, 255, 0, 255), 10.0f, 30.0f);
}

LRESULT InputManager::MsgProc(HWND handle, const UINT& message, const WPARAM& wParam, const LPARAM& lParam)
{
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
	{
		m_mouse_position.x = static_cast<float>(LOWORD(lParam));
		m_mouse_position.y = static_cast<float>(HIWORD(lParam));

		m_render_str = "X: " + std::to_string(m_mouse_position.x) + " Y: " + std::to_string(m_mouse_position.y);
	}

	if (message == WM_MOUSEWHEEL)
	{
		short wheel_value = static_cast<short>(HIWORD(wParam));

		m_mouse_wheel_old_state.z = m_mouse_wheel_state.z;
		m_mouse_wheel_state.z += static_cast<float>(wheel_value);
	}

	return TRUE;
}
