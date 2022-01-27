#pragma once

#include "IResource.h"

class VertexBuffer;
class IndexBuffer;

class Mesh final : public IResource
{
public:
	Mesh(const std::string& mesh_resource_name);
	explicit Mesh(const Mesh& origin) = default;
	~Mesh();

	void Create(const MeshType& mesh_type, const UINT& count_x = 0, const UINT& count_y = 0);

	void BindPipeline() override;

	void Render();
	void RenderInstance(const UINT& render_count);

private:
    void CreatePointMesh();
	void CreateTriangleMesh();
	void CreateRectangleMesh();
	void CreateCircleMesh();
	void CreateGridMesh(const UINT& count_x, const UINT& count_y);

public:
    void Clear();

	const std::vector<VertexColorTexture> GetVertexVector() const { return m_vertex_vector; }
	const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { SAFE_GET_POINTER(m_p_vertex_buffer); }

	const std::vector<UINT> GetIndexVector() const { return m_index_vector; }
	const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { SAFE_GET_POINTER(m_p_index_buffer); }

	const MeshType& GetMeshType() const { return m_mesh_type; }

public:
    //Grid
	const std::vector<Vector2> GetGridLeftTopVector() const { return m_grid_left_top; }

public:
	bool SaveToFile(const std::string& animation2D_path) override;
	bool LoadFromFile(const std::string& animation2D_path) override;

public:
	CLONE(Mesh);

private:
	std::vector<VertexColorTexture> m_vertex_vector;
	std::shared_ptr<VertexBuffer> m_p_vertex_buffer = nullptr;

	std::vector<UINT> m_index_vector;
	std::shared_ptr<IndexBuffer> m_p_index_buffer = nullptr;

	MeshType m_mesh_type = MeshType::Point;

	//Grid
	std::vector<Vector2> m_grid_left_top;
};