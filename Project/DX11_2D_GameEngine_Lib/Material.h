#pragma once

#include "IResource.h"

class Texture;
class Shader;

class Material : public IResource
{
public:
    Material(const std::string resource_name);
    explicit Material(const Material& origin);
    ~Material() = default;

    void BindPipeline() override;

public:
    void SetConstantBufferData(const Material_Parameter& material_parameter, void* data, std::shared_ptr<Texture> texture = nullptr);

    void SetShader(const std::shared_ptr<Shader>& shader) { m_p_shader = shader; }
    const std::shared_ptr<Shader>& GetShader() { SAFE_GET_POINTER(m_p_shader); }

    const std::shared_ptr<Texture>& GetTexture() { return m_p_texture_vector[0]; }

    const std::vector<std::shared_ptr<Texture>>& GetTextureVector() { return m_p_texture_vector; }

public:
    CLONE(Material);

private:
    CBuffer_Material m_material_data;

    std::vector<std::shared_ptr<Texture>> m_p_texture_vector;

    std::shared_ptr<Shader> m_p_shader = nullptr;
};

