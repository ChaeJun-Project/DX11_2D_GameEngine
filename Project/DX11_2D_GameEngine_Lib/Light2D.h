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
    void SetLightColor(Vector4 light_color) { m_light2D_data.ligth_color.color = light_color; }
    void SetLightSpecular(Vector4 specular) { m_light2D_data.ligth_color.specular = specular; }
    void SetLightAmbient(Vector4 ambient) { m_light2D_data.ligth_color.ambient = ambient; }

	//Light Property
	void SetLightType(LightType light_type) { m_light2D_data.light_type = light_type; }
	void SetLightDir(Vector3 light_direction) { m_light2D_data.light_direction = light_direction; }
	void SetLightRange(float light_range) {  m_light2D_data.light_range = light_range; }
	void SetLightAngle(float light_angle) {  m_light2D_data.light_angle = light_angle; }

public:
    const LightInfo& GetLight2DInfo() const { return m_light2D_data; }

private:
	void SaveToScene(FILE* p_file) override;
	void LoadFromScene(FILE* p_file) override;

private:
	CLONE(Light2D);

private:
    int m_light2D_index = -1;

	LightInfo m_light2D_data;
};

