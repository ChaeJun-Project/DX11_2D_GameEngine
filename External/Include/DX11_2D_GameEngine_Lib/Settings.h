#pragma once
#include "stdafx.h"

//윈도우 속성 값 저장 용도 클래스
class Settings final : public Singleton<Settings>
{
	//싱글톤 매크로
	SINGLETON(Settings);
	Settings() {};
	virtual ~Settings() {};

public:
	//Instance Setting
	auto GetProgramInstance() const -> HINSTANCE { return m_program_instance; }
	void SetProgramInstance(HINSTANCE program_instance) { this->m_program_instance = program_instance; }

	//Handle Setting
	auto GetWindowHandle() const -> HWND { return m_window_handle; }
	void SetWindowHandle(HWND window_handle) { this->m_window_handle = window_handle; }

	//Width Setting
	auto GetWindowWidth() const -> const UINT& { return m_window_width; }
	void SetWindowWidth(const UINT& window_width) { this->m_window_width = window_width; }

	//Height Setting
	auto GetWindowHeight() const -> const UINT& { return m_window_height; }
	void SetWindowHeight(const UINT& window_height) { this->m_window_height = window_height; }

	//전체화면 Setting
	auto IsFullScreen() const -> const bool& { return m_is_full_screen; }
	void SetFullScreen(const bool& m_is_full_screen) { this->m_is_full_screen = m_is_full_screen; }

	//수직동기화 Setting
	auto IsVsync() const -> const bool& { return m_is_vsync; }
	void SetVsync(const bool& is_vsync) { this->m_is_vsync = is_vsync; }

private:
	//HINSTANCE: 프로그램의 실제 메모리를 가지고 있는
	//프로그램의 인스턴스 식별자(핸들)
	//윈도우가 실행중인 프로그램들을 구분하기 위한 식별값
	HINSTANCE m_program_instance = nullptr;

	//HWND: 프로그램 내에서의 윈도우 식별자(핸들)
	//하나의 프로그램이 많은 윈도우를 가질 수 있기 때문에 필요
	HWND m_window_handle = nullptr;

	UINT m_window_width = 0; //윈도우 창의 너비
	UINT m_window_height = 0; //윈도우 창의 높이

	bool m_is_full_screen = false; //전체화면 여부
	bool m_is_vsync = false; //수직 동기화 여부
};