#pragma once
#include "stdafx.h"

//윈도우 속성 값 저장 용도 클래스
class Settings final
{
public:
	Settings() = default;
	~Settings() = default;

public:
	//Instance Setting
	HINSTANCE GetProgramInstance() const { return m_program_instance; }
	void SetProgramInstance(HINSTANCE program_instance) { m_program_instance = program_instance; }

	//Handle Setting
	HWND GetWindowHandle() const { return m_window_handle; }
	void SetWindowHandle(HWND window_handle) { m_window_handle = window_handle; }

	//Width Setting
	const UINT GetWindowWidth() const { return m_window_width; }
	void SetWindowWidth(const UINT& window_width) { m_window_width = window_width; }

	//Height Setting
	const UINT GetWindowHeight() const { return m_window_height; }
	void SetWindowHeight(const UINT& window_height) { m_window_height = window_height; }

	//Game Resolution
	const Vector2 GetGameResolution() const { return m_game_resolution; }
	void SetGameResolution(const Vector2 game_resolution) { m_game_resolution = game_resolution; }

	//전체화면 Setting
	const bool IsFullScreen() const { return m_is_full_screen; }
	void SetFullScreen(const bool& is_full_screen) { m_is_full_screen = is_full_screen; }

	//수직동기화 Setting
	const bool IsVsync() const { return m_is_vsync; }
	void SetVsync(const bool& is_vsync) { m_is_vsync = is_vsync; }

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

	Vector2 m_game_resolution = Vector2::Zero; //게임 해상도

	bool m_is_full_screen = false; //전체화면 여부
	bool m_is_vsync = false; //수직 동기화 여부
};