#pragma once

#include "Singleton.h"

enum class Key
{
	KEY_ARROW_LEFT,
	KEY_ARROW_UP,
	KEY_ARROW_RIGHT,
	KEY_ARROW_DOWN,

	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	KEY_SHIFT,
	KEY_CONTROL,
	KEY_ALT,
	KEY_SPACE,
	KEY_ENTER,
	KEY_DELETE,

	END,
};

enum class Button
{
	CLICK_LEFT,
	CLICK_RIGHT,

	END,
};

class InputManager final : public Singleton<InputManager>
{
	SINGLETON(InputManager);

	InputManager();
	~InputManager() = default;

public:
	void Initialize();
	void Update();
	void Render();

	//Key
public:
	bool KeyDown(const Key& key) const { return m_key_vector[static_cast<UINT>(key)].m_key_state == KeyState::KEY_INPUT_STATE_DOWN; }
	bool KeyUp(const Key& key) const { return m_key_vector[static_cast<UINT>(key)].m_key_state == KeyState::KEY_INPUT_STATE_UP; }
	bool KeyPress(const Key& key) const { return m_key_vector[static_cast<UINT>(key)].m_key_state == KeyState::KEY_INPUT_STATE_PRESS; }

	//Button
public:
	bool BtnDown(const Button& button) const { return m_button_vector[static_cast<UINT>(button)].m_button_state == ButtonState::BUTTON_INPUT_STATE_DOWN; }
	bool BtnUp(const Button& button) const { return m_button_vector[static_cast<UINT>(button)].m_button_state == ButtonState::BUTTON_INPUT_STATE_UP; }
	bool BtnPress(const Button& button) const { return m_button_vector[static_cast<UINT>(button)].m_button_state == ButtonState::BUTTON_INPUT_STATE_PRESS; }

public:
	std::function<LRESULT(HWND, const UINT&, const WPARAM&, const LPARAM&)> MouseProc = nullptr;

	Vector2 GetMousePosition() const { return m_mouse_position; }
	Vector3 GetMouseMoveValue() const { return m_mouse_wheel_move_value; }

private:
	LRESULT MsgProc(HWND handle, const UINT& message, const WPARAM& wParam, const LPARAM& lParam);

private:
	enum class KeyState
	{
		KEY_INPUT_STATE_NONE		 = 0,
		KEY_INPUT_STATE_DOWN		 = 1,
		KEY_INPUT_STATE_UP			 = 2,
		KEY_INPUT_STATE_PRESS		 = 3,
	};

	struct KeyInfo
	{
		KeyState m_key_state;		//키의 상태값
		bool	 m_is_pre_input;	//이전 프레임에서 눌렸는지 여부
	};

	enum class ButtonState
	{
		BUTTON_INPUT_STATE_NONE		= 0,
		BUTTON_INPUT_STATE_DOWN		= 1,
		BUTTON_INPUT_STATE_UP		= 2,
		BUTTON_INPUT_STATE_PRESS	= 3,
		BUTTON_INPUT_STATE_DBLCLK	= 4,
	};

	struct ButtonInfo
	{
		ButtonState  m_button_state;	   //마우스 버튼 상태값
		UINT         m_click_count;		   //마우스 버튼 클릭 횟수
		DWORD        m_start_double_click; //마우스 더블 클릭 시작 시간
	};

private:
    //Key
	std::vector<int> m_virtual_key_vector;
	std::vector<KeyInfo> m_key_vector;

	//Mouse Button
	std::vector<int> m_virtual_button_vector;
	std::vector<ButtonInfo> m_button_vector;
	DWORD m_time_double_click;

	//Mouse Position
    Vector2 m_mouse_position		 = Vector2::Zero;

	//Mouse Wheel
	Vector3 m_mouse_wheel_state		 = Vector3::Zero;
	Vector3 m_mouse_wheel_old_state	 = Vector3::Zero;
	Vector3 m_mouse_wheel_move_value = Vector3::Zero;

	std::string m_render_str;
};
