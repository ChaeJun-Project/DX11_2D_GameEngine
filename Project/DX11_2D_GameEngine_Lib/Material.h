#pragma once

#include "IResource.h"

class Texture;
class Shader;

class Material : public IResource
{
public:
    Material(const std::string resource_name);
    ~Material() = default;

    void BindPipeline() override;

public:
    void SetConstantBufferData(const Material_Parameter& material_parameter, void* data, std::shared_ptr<Texture> texture = nullptr);

    void SetShader(const std::shared_ptr<Shader>& shader) { this->m_p_shader = shader; }
    const std::shared_ptr<Shader>& GetShader() { SAFE_GET_POINTER(this->m_p_shader); }

private:
    CBuffer_Material m_material_data;

    std::vector<std::shared_ptr<Texture>> m_p_texture_vector;

    std::shared_ptr<Shader> m_p_shader = nullptr;
};

