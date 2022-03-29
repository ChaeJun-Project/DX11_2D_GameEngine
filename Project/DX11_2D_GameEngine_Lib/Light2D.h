#pragma once
#include "IComponent.h"

class Light2D final : public IComponent
{
public:
	Light2D();
	explicit Light2D(const Light2D& origin);
	~Light2D() = default;

	virtual void FinalUpdate();
public:
	//Light Color
	void SetLightColor(Vector4 light_color) { m_light2D_data.color = light_color; }

	//Light Property
	void SetLightType(Light2DType light2D_type) { m_light2D_data.type = light2D_type; }
	void SetLightRange(float light_range) { m_light2D_data.range = light_range; }

public:
	const Light2D_Info& GetLight2DInfo() const { return m_light2D_data; }

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(Light2D);

private:
	int m_light2D_index = -1;

	Light2D_Info m_light2D_data;
};
