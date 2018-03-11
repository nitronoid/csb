#include "Mesh.h"
#include <unordered_set>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


void Mesh::load(const std::string &_fname, const size_t _meshNum)
{
  Assimp::Importer importer;
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // propably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(
        _fname,
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType |
        aiProcess_FlipUVs
        );
  const aiMesh* mesh = scene->mMeshes[_meshNum];

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

  for (size_t i = 0; i < numVerts; ++i)
  {
    auto& vert = vertices[i];
    m_vertices.insert(m_vertices.end(), {vert.x,vert.y,vert.z});

    auto& norm = normals[i];
    m_normals.insert(m_normals.end(), {norm.x,norm.y,norm.z});

    if (!hasTexCoords) continue;
    // UV's only use the first two members
    const auto& uv = hasTexCoords ? texCoords[i] : aiVector3D(0.0f, 0.0f, 0.0f);
    m_uvs.insert(m_uvs.end(), {uv.x, uv.y});
  }

  m_adjacency.resize(numVerts);
  std::vector<std::unordered_set<GLushort>> adjacencySets(numVerts);
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

      GLushort otherV1 = static_cast<GLushort>(face.mIndices[(i + 1) % numIndices]);
      GLushort otherV2 = static_cast<GLushort>(face.mIndices[(i + 2) % numIndices]);

      adjacencySets[vertInFace].insert(otherV1);
      adjacencySets[vertInFace].insert(otherV2);

      adjacencySets[otherV1].insert(vertInFace);
      adjacencySets[otherV1].insert(otherV2);

      adjacencySets[otherV2].insert(otherV1);
      adjacencySets[otherV2].insert(vertInFace);
    }
  }
  // copy the contents of the sets into vector form
  for (size_t i = 0; i < numVerts; ++i)
    m_adjacency[i].insert(m_adjacency[i].end(), adjacencySets[i].begin(), adjacencySets[i].end());
}

void Mesh::reset()
{
  m_indices.clear();
  m_vertices.clear();
  m_normals.clear();
  m_uvs.clear();
}

const GLushort *Mesh::getIndicesData() const noexcept
{
  return &m_indices[0];
}

const GLfloat* Mesh::getVertexData() const noexcept
{
  return &m_vertices[0].x;
}

const GLfloat* Mesh::getNormalsData() const noexcept
{
  return &m_normals[0].x;
}

const GLfloat *Mesh::getUVsData() const noexcept
{
  return &m_uvs[0].x;
}

const GLfloat *Mesh::getAttribData(const MeshAttributes::Attribute _attrib) const noexcept
{
  using namespace MeshAttributes;
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

int Mesh::getNIndicesData() const noexcept
{
  return static_cast<int>(m_indices.size());
}

int Mesh::getNVertData() const noexcept
{
  return static_cast<int>(m_vertices.size()) * 3;
}

int Mesh::getNNormData() const noexcept
{
  return static_cast<int>(m_normals.size()) * 3;
}

int Mesh::getNUVData() const noexcept
{
  return static_cast<int>(m_uvs.size()) * 2;
}

int Mesh::getNData() const noexcept
{
  return getNVertData() + getNNormData() + getNUVData();
}



