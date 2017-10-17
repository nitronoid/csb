#ifndef MESH_H
#define MESH_H

///**********************************PLEASE NOTICE*****************************************///
///****************************************************************************************///
///*																																										  *///
///*	THIS CLASS WAS INITIALLY DESIGNED FOR THE SECOND PPP ASSIGNMENT (FIRST YEAR)        *///
///*	MINOR TWEAKS WERE MADE TO THIS CLASS BUT OVERALL IT SHOUDLN'T BE MARKED             *///
///*	AS IT HAS ALREADY BEEN SUBMITTED AND MARKED																			    *///
///*	EXEPT FOR THE TWO "WRITE" FUNCTIONS WHICH WERE ADDED SPECIFICALLY FOR THIS PROJECT  *///
///*																																										  *///
///*  original file: https://github.com/albelax/PPP_Assignment2/blob/master/src/Mesh.cpp  *///
///*																																										  *///
///****************************************************************************************///
///****************************************************************************************///

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "glm.hpp"

/// \class Mesh.h
/// \brief This class loads an obj file and stores vertices, normals and uvs, when necessary it can also export into obj
/// \author Alberto La Scala

class Mesh
{
private:

	/// \brief m_vertices is a std::vector containing the vertices
	std::vector<float> m_vertices;

	/// \brief m_normals is a std::vector containing the normals
	std::vector<float> m_normals;

	/// \brief m_normals is a std::vector containing the normals
	std::vector<float> m_uvs;

	/// \brief m_address is a string that contains the location of the file, for example "models/cube.obj"
	std::vector<std::string> m_address;


	/// min and max in x and y, it was needed in the previous assignment to calculate collision in a basic way
	float m_minX, m_minY, m_minZ, m_maxX, m_maxY, m_maxZ;

	/// \brief m_name name of the obj file
	std::string m_name;

	/// \brief m_bufferIndex is the index of the buffer that store its vertices, it makes it easy for drawing and instantiating
	int m_bufferIndex;

	/// \brief split is a utility function really similar to the omonimous python function,
	/// it splits a string whenever it encounters a specific char
	/// \param _stringIn is the string that needs to be split
	/// \param _splitChar is the char that will split the string
	/// \return returns the vector containing all the substrings
	std::vector<std::string> split(std::string _stringIn, char _splitChar = ' ');
public:


	/// \brief Mesh constructs the object taking in input the address of the obj and loading it
	/// \param _address is the address of the obj file
	/// \param _name is the name
	Mesh(std::string _address, std::string _name);

	/// \brief Mesh is the default constructor, it doesn't do anything, just construct the object
	Mesh();


	/// \brief setBufferIndex sets the index of the buffer that store its vertices
	void setBufferIndex(int _index) { m_bufferIndex = _index; }

	/// \brief getMin returns a vec4 containing the minX minY and minZ of the mesh
	glm::vec4 getMin() const { return glm::vec4(m_minX,m_minY,m_minZ,1); }

	/// \brief getMax returns a vec4 containing the maxX maxY and maxZ of the mesh
	glm::vec4 getMax() const { return glm::vec4(m_maxX,m_maxY,m_maxZ,1); }


	/// \brief getVertices returns the reference of the std::vector that contains the vertices
	const std::vector<float> &getVertices() const { return m_vertices; }

	/// \brief getNormals returns the reference of the std::vector that contains the normals
	const std::vector<float> &getNormals() const { return m_normals; }

	const std::vector<float> &getUVs() const { return m_uvs; }

	/// \brief getVerticesCopy returns a copy of the vertices
	std::vector<float> getVerticesCopy() const { return m_vertices; }

	/// \brief getNormalsCopy returns a copy of the normals
	std::vector<float> getNormalsCopy() const { return m_normals; }

	std::vector<float> getUVsCopy() const { return m_uvs; }

	const float &getVertexData() const { return m_vertices[0]; }
	const float &getNormalsData() const { return m_normals[0]; }
	const float &getUVsData() const { return m_uvs[0]; }

	/// \brief getAmountVertexData returns amount of vertices of contained in this mesh, they are stored as floats,
	/// which implies they will have to be divided by 3 to get the actual number of vertices
	size_t getAmountVertexData() const { return static_cast<int>( m_vertices.size() ); }

	/// \brief getBufferIndex gets the index of the buffer that store its vertices
	int getBufferIndex() const { return m_bufferIndex; }

	/// \brief name returns the name of the object
	std::string name() const { return m_name; }

	/// \brief getAddress returns the original address of the mesh
	std::vector<std::string> getAddress() { return m_address; }

	/// \brief write exports the vertices and normals into a new obj file at the specified destination
	void write(std::string _destination);

	/// \brief write exports the vertices and normals into a new obj file at the specified destination,
	/// this overload lets the user export his own data without having to make a mesh object
	static void write(const std::vector<float> &_vertices, const std::vector<float> &_normals, std::string _destination);
};

#endif // MESH_H
