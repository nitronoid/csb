#include "TriMesh.h"
#include <unordered_set>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void csb::TriMesh::load(const std::string &_fname, const size_t &_meshId)
{
  Assimp::Importer importer;
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // propably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(
        _fname,
        aiProcess_RemoveComponent |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices|
//        aiProcess_GenSmoothNormals |
        aiProcess_SortByPType |
        aiProcess_FlipUVs
        );
  const aiMesh* mesh = scene->mMeshes[_meshId];

  // Calculate the amount of vertices we will store (3 per face)
  size_t numVerts = mesh->mNumVertices;

  // Reserve memory in vectors to accomodate the incomming data
  m_vertices.reserve(numVerts);
  m_normals.reserve(numVerts);
  m_uvs.reserve(numVerts);

  // Get access to the information we will store
  auto& vertices = mesh->mVertices;
  auto& normals = mesh->mNormals;
  auto& texCoords = mesh->mTextureCoords[0];
  // Some meshes don't have UV's
  bool hasTexCoords = mesh->HasTextureCoords(0);
  bool hasNormals = mesh->HasNormals();

  for (size_t i = 0; i < numVerts; ++i)
  {
    auto& vert = vertices[i];
    m_vertices.insert(m_vertices.end(), {vert.x,vert.y,vert.z});

    if (hasNormals)
    {
      auto& norm = normals[i];
      m_normals.insert(m_normals.end(), {norm.x,norm.y,norm.z});
    }

    if (hasTexCoords)
    {
      // UV's only use the first two members
      const auto& uv = hasTexCoords ? texCoords[i] : aiVector3D(0.0f, 0.0f, 0.0f);
      m_uvs.insert(m_uvs.end(), {uv.x, uv.y});
    }
  }

  // Get the number of faces on the mesh
  size_t numFaces = mesh->mNumFaces;
  // We iterate through faces not vertices,
  // as the verts will be duplicated when used by more than one face.
  for (size_t faceIndex = 0; faceIndex < numFaces; ++faceIndex)
  {
    auto& face = mesh->mFaces[faceIndex];
    const auto numIndices = face.mNumIndices;
    // Iterate over the vertices of this face
    for (size_t i = 0; i < numIndices; ++i)
    {
      // Get the index of the vertex, we use this for it's normals and UV's too
      size_t vertInFace = face.mIndices[i];
      m_indices.push_back(static_cast<GLushort>(vertInFace));
    }
  }

  calcEdges();

  m_adjacency.resize(numVerts);
  std::vector<std::unordered_set<GLushort>> adjacencySets(numVerts);
  for (const auto& edge : m_edges)
  {
    adjacencySets[edge.p.first].insert(edge.p.second);
    adjacencySets[edge.p.second].insert(edge.p.first);
  }

  // copy the contents of the sets into vector form
  for (size_t i = 0; i < numVerts; ++i)
    m_adjacency[i].insert(m_adjacency[i].end(), adjacencySets[i].begin(), adjacencySets[i].end());

}

void csb::TriMesh::reset()
{
  m_indices.clear();
  m_vertices.clear();
  m_normals.clear();
  m_uvs.clear();
  m_edges.clear();
}

const GLushort *csb::TriMesh::getIndicesData() const noexcept
{
  return &m_indices[0];
}

const GLfloat* csb::TriMesh::getVertexData() const noexcept
{
  return &m_vertices[0].x;
}

const GLfloat* csb::TriMesh::getNormalsData() const noexcept
{
  return &m_normals[0].x;
}

const GLfloat *csb::TriMesh::getUVsData() const noexcept
{
  return &m_uvs[0].x;
}

const GLfloat *csb::TriMesh::getAttribData(const csb::MeshAttribute _attrib) const noexcept
{
  using namespace csb;
  const GLfloat * data = nullptr;
  switch (_attrib)
  {
    case VERTEX: data = getVertexData(); break;
    case NORMAL: data = getNormalsData(); break;
    case UV:     data = getUVsData(); break;
    default: break;
  }
  return data;
}


int csb::TriMesh::getNAttribData(const csb::MeshAttribute _attrib) const noexcept
{
  using namespace csb;
  int data = 0;
  switch (_attrib)
  {
    case VERTEX: data = getNVertData(); break;
    case NORMAL: data = getNNormData(); break;
    case UV:     data = getNUVData(); break;
    default: break;
  }
  return data;
}

size_t csb::TriMesh::getNVerts() const noexcept
{
  return m_vertices.size();
}

size_t csb::TriMesh::getNUVs() const noexcept
{
  return m_uvs.size();
}

size_t csb::TriMesh::getNNorms() const noexcept
{
  return m_normals.size();
}

size_t csb::TriMesh::getNIndices() const noexcept
{
  return m_indices.size();
}

size_t csb::TriMesh::getNEdges() const noexcept
{
  return m_edges.size();
}

int csb::TriMesh::getNIndicesData() const noexcept
{
  return static_cast<int>(m_indices.size());
}

int csb::TriMesh::getNVertData() const noexcept
{
  return static_cast<int>(m_vertices.size() * 3);
}

int csb::TriMesh::getNNormData() const noexcept
{
  return static_cast<int>(m_normals.size() * 3);
}

int csb::TriMesh::getNUVData() const noexcept
{
  return static_cast<int>(m_uvs.size() * 2);
}

int csb::TriMesh::getNData() const noexcept
{
  return getNVertData() + getNNormData() + getNUVData();
}

const std::vector<std::vector<GLushort>>& csb::TriMesh::getAdjacencyInfo() const noexcept
{
  return m_adjacency;
}

const std::vector<GLushort>& csb::TriMesh::getIndices() const noexcept
{
  return m_indices;
}

const std::vector<glm::vec3> &csb::TriMesh::getVertices() const noexcept
{
  return m_vertices;
}

const std::vector<glm::vec2> &csb::TriMesh::getUVs() const noexcept
{
  return m_uvs;
}

const std::vector<glm::vec3> &csb::TriMesh::getNormals() const noexcept
{
  return m_normals;
}

void csb::TriMesh::calcEdges()
{
  std::unordered_set<Edge> edgeSet;
  // Roughly
  auto numEdges = static_cast<size_t>(std::abs(static_cast<long>(m_vertices.size()) + (static_cast<long>(m_indices.size()) / 3l) - 2l));
  edgeSet.reserve(numEdges);

  const auto last = m_indices.size() - 2;
  for (size_t i = 0; i < last; i+=3)
  {
    const auto p1 = m_indices[i];
    const auto p2 = m_indices[i + 1];
    const auto p3 = m_indices[i + 2];
    edgeSet.insert({p1, p2});
    edgeSet.insert({p2, p3});
    edgeSet.insert({p3, p1});
  }
  std::copy(edgeSet.begin(), edgeSet.end(), std::back_inserter(m_edges));
}
