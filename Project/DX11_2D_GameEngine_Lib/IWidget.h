#pragma once

#include "stdafx.h"

#include "DX11Object.h"

class GameObject;

class IWidget : public DX11Obejct
{
	IWidget(const WidgetType& widget_type)
		:m_widget_type(widget_type)
	{
	}
	virtual ~IWidget()
	{
		m_p_owner_game_object = nullptr;
	}

	virtual void Start() {}
	virtual void Update() {} //업데이트
	virtual void FinalUpdate() = 0; //최종 업데이트 => 오브젝트가 움직이면 안됨

public:
	virtual void SaveToScene(FILE* p_file) override
	{
		switch (m_widget_type)
		{
		case WidgetType::RectTransform:
			fprintf(p_file, "■ Rect Transform\n");
			break;
		case WidgetType::Image:
			fprintf(p_file, "■ Image\n");
			break;
		case WidgetType::ProgressBar:
			fprintf(p_file, "■ Progress Bar\n");
			break;
		}

		//Widget Type
		fprintf(p_file, "%d\n", static_cast<UINT>(m_widget_type));

		//Active
		fprintf(p_file, "[Active]\n");
		fprintf(p_file, "%d\n", m_is_active);
	}
	virtual void LoadFromScene(FILE* p_file) override
	{
		char char_buffer[256] = {};

		//Active 
		FILE_MANAGER->FScanf(char_buffer, p_file);
		int boolen_num = -1;
		fscanf_s(p_file, "%d\n", &boolen_num);
		m_is_active = boolen_num;
	}

public:
	virtual IWidget* Clone() const = 0;

public:
	const WidgetType& GetWidgetType() { return m_widget_type; }

	GameObject* GetGameObject() { SAFE_GET_POINTER(m_p_owner_game_object); }
	void SetGameObject(GameObject* p_game_object) { m_p_owner_game_object = p_game_object; }

	const bool& GetIsActive() { return m_is_active; }
	void SetIsActive(const bool& is_active) { m_is_active = is_active; }

protected:
	const WidgetType m_widget_type;

	//해당 컴포넌트를 소유하고 있는 오브젝트의 주소
	GameObject* m_p_owner_game_object;

	bool m_is_active = true;
};