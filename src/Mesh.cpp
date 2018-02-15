#include "Mesh.h"
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
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType);
  const aiMesh* mesh = scene->mMeshes[_meshNum];

  // Get the number of faces on the mesh
  size_t numFaces = mesh->mNumFaces;
  // Calculate the amount of vertices we will store (3 per face)
  size_t numStored = numFaces * 3;

  // Reserve memory in vectors to accomodate the incomming data
  m_vertices.reserve(numStored * 3);
  m_normals.reserve(numStored * 3);
  m_uvs.reserve(numStored * 2);

  // Get access to the information we will store
  auto& vertices = mesh->mVertices;
  auto& normals = mesh->mNormals;
  auto& texCoords = mesh->mTextureCoords[0];
  // Some meshes don't have UV's
  bool hasTexCoords = mesh->HasTextureCoords(0);

  // We iterate through faces not vertices,
  // as the verts will be duplicated when used by more than one face.
  for (size_t faceIndex = 0; faceIndex < numFaces; ++faceIndex)
  {
    auto& face = mesh->mFaces[faceIndex];
    // Iterate over the vertices of this face
    for (size_t i = 0; i < face.mNumIndices; ++i)
    {
      // Get the index of the vertex, we use this for it's normals and UV's too
      size_t vertInFace = face.mIndices[i];

      // Get the vertex
      auto& vert = vertices[vertInFace];
      // Push it's values into our float vector
      m_vertices.insert(m_vertices.end(), {vert.x,vert.y,vert.z});

      // Do the same for normals
      auto& normal = normals[vertInFace];
      m_normals.insert(m_normals.end(), {normal.x,normal.y,normal.z});

      // Lazy check for texCoords
      if (!hasTexCoords) continue;

      // UV's only use the first two members
      auto& uv = texCoords[vertInFace];
      m_uvs.insert(m_uvs.end(), {uv.x,uv.y});
    }
  }
}

void Mesh::reset()
{
  m_vertices.clear();
  m_normals.clear();
  m_uvs.clear();
}

const GLfloat* Mesh::getVertexData() const noexcept
{
  return &m_vertices[0];
}

const GLfloat* Mesh::getNormalsData() const noexcept
{
  return &m_normals[0];
}

const GLfloat *Mesh::getUVsData() const noexcept
{
  return &m_uvs[0];
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

int Mesh::getNVertData() const noexcept
{
  return static_cast<int>(m_vertices.size());
}

int Mesh::getNNormData() const noexcept
{
  return static_cast<int>(m_normals.size());
}

int Mesh::getNUVData() const noexcept
{
  return static_cast<int>(m_uvs.size());
}

int Mesh::getNData() const noexcept
{
  return getNVertData() + getNNormData() + getNUVData();
}



