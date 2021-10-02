#pragma once

#include "IResource.h"

template<typename T>
class Mesh final : public IResource
{
public: 
    Mesh(const std::string resource_name);
    ~Mesh();

    void Create(const MeshType& mesh_type, const Vector2& mesh_size);

    const bool& LoadFromFile(const std::string& mesh_path) override;
    void SaveFile(const std::string& mesh_path) override;

    void BindPipeline() override;

    void Render();

private:
    void CreateTriangleMesh();
    void CreateRectangleMesh();
    void CreateCircleMesh();

public:
    const std::vector<T> GetVertexVector() const { return this->m_vertex_vector; }
    const VertexBuffer* GetVertexBuffer() const { SAFE_GET_POINTER(this->m_p_vertex_buffer.Get()); }

    const std::vector<UINT> GetIndexVector() const { return this->m_index_vector; }
    const IndexBuffer* GetIndexBuffer() const { SAFE_GET_POINTER(this->m_p_index_buffer.Get()); }

    const MeshType& GetMeshType() const { return this->m_mesh_type; }

private:
    std::vector<T> m_vertex_vector;
    VertexBuffer* m_p_vertex_buffer = nullptr;

    std::vector<UINT> m_index_vector;
    IndexBuffer* m_p_index_buffer = nullptr;

    MeshType m_mesh_type;
    Vector2 m_mesh_size;
};

#include "Mesh.hpp"

