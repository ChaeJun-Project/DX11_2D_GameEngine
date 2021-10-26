#pragma once
#include "IComponent.h"

class Light2D final : public IComponent
{
public:
	Light2D();
	explicit Light2D(const Light2D& origin);
	~Light2D();

	virtual void FinalUpdate();

	void UpdateConstantBuffer();

public:
    //Light Color
    void SetLightColor(Color4 light_color) { this->m_light2D_data.ligth_color.color = light_color; }
    void SetLightSpecular(Color4 specular) { this->m_light2D_data.ligth_color.specular = specular; }
    void SetLightAmbient(Color4 ambient) { this->m_light2D_data.ligth_color.ambient = ambient; }

	//Light Property
	void SetLightType(LightType light_type) { this->m_light2D_data.light_type = light_type; }
	void SetLightDir(Vector3 light_direction) { this->m_light2D_data.light_direction = light_direction; }
	void SetLightPos(Vector3 light_position) { this->m_light2D_data.light_position = light_position; }
	void SetLightRange(float light_range) {  this->m_light2D_data.light_range = light_range; }
	void SetLightAngle(float light_angle) {  this->m_light2D_data.light_angle = light_angle; }

private:
	LightInfo m_light2D_data;
};

