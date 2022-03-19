#pragma once
#include "IComponent.h"

enum class ImageType : UINT
{
   Default = 0,
   Filled = 1,
};

enum class FillMethodType : UINT
{
    Horizontal = 0,
    Vertical = 1,
};

//수평 채우기 방향
enum class HorizontalDirectionType : UINT
{
     Left = 0,
     Right = 1
};

//수직 채우기 방향
enum class VerticalDirectionType : UINT
{
     Bottom = 0,
     Top = 1
};

class ImageRenderer final : public IComponent
{
public:
    ImageRenderer();
    explicit ImageRenderer(const ImageRenderer& origin);
    ~ImageRenderer();

    void FinalUpdate() override {};

    void Render();

private:
    void SetImageRendererInfo();

public:
    std::shared_ptr<Texture> GetTexture() { SAFE_GET_POINTER(m_p_texture); }
    void SetTexture(const std::shared_ptr<Texture>& p_texture);

    const Vector4& GetImageColor() { return m_image_color; }
    void SetImageColor(const Vector4& image_color) { m_image_color = image_color; }

    std::shared_ptr<Material> GetMaterial() { SAFE_GET_POINTER(m_p_material); }
    void SetMaterial(const std::shared_ptr<Material>& p_material);

    std::shared_ptr<Mesh> GetMesh() { SAFE_GET_POINTER(m_p_mesh); }
    void SetMesh(const std::shared_ptr<Mesh>& p_mesh);

    ImageType GetImageType() const { return m_image_type; }
    void SetImageType(const ImageType& image_type) { m_image_type = image_type; }

    FillMethodType GetFillMethod() const { return m_fill_method_type; }
    void SetFillMethod(const FillMethodType& fill_method_type) { m_fill_method_type = fill_method_type; }

    //Horizontal
    HorizontalDirectionType GetHorizontalDirection() const { return m_horizontal_direction_type; }
    void SetHorizontalDirection(const HorizontalDirectionType& horizontal_direction_type) { m_horizontal_direction_type = horizontal_direction_type; }

    //Vertical
    VerticalDirectionType GetVerticalDirection() const { return m_vertical_direction_type; }
    void SetVerticalDirection(const VerticalDirectionType& vertical_direction_type) { m_vertical_direction_type = vertical_direction_type; }

    //Fill Amount
    float GetFillAmount() const { return m_fill_amount; }
    void SetFillAmount(const float& fill_amount) { m_fill_amount = fill_amount; }

private:
    void SaveToScene(FILE* p_file) override;
    void LoadFromScene(FILE* p_file) override;

private:
    CLONE(ImageRenderer);

private:
    std::shared_ptr<Texture> m_p_texture = nullptr;
    Vector4 m_image_color = Vector4::White;

    std::shared_ptr<Material> m_p_material = nullptr;
    std::shared_ptr<Mesh> m_p_mesh = nullptr;

    ImageType m_image_type = ImageType::Default;
    FillMethodType m_fill_method_type = FillMethodType::Horizontal;
    HorizontalDirectionType m_horizontal_direction_type = HorizontalDirectionType::Left;
    VerticalDirectionType m_vertical_direction_type = VerticalDirectionType::Bottom;
    float m_fill_amount = 1.0f;
};

