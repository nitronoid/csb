#include "Mesh.h"

Mesh::Mesh()
{

}

//----------------------------------------------------------------------------------------------------------------------

Mesh::Mesh(std::string _address, std::string _name)
{
	m_name = _name;
	std::vector<float> tempVertices;
	std::vector<float> tempNormals;
	std::vector<float> tempUvs;
	std::vector<std::string> tempFaces;
	std::string line;
	std::vector<std::string> tempData;

	std::ifstream Source;
	Source.open(_address);
	Source.seekg(0, std::ios::end); // starts from the beginning and goes to the end of the file to know the lenght of it
	int len = (int)Source.tellg();
	Source.seekg(0);

	bool testuv = true;
	/* set min and max to arbitrary values,
	 * it is necessary to update them to somethong meaningful later on
	 */
	m_minX = 500;
	m_minY = 500;
	m_minZ = 500;
	m_maxX = 0;
	m_maxY = 0;
	m_maxZ = 0;

	// indices of the temporary std::vectors
	std::size_t i_tempVertices = 0;
	std::size_t i_tempNormals = 0;
	std::size_t i_tempUvs = 0;
	std::size_t iVertices = 0;
	std::size_t iNormals = 0;
	std::size_t iUvs = 0;

	while (len > 0)
	{
		getline(Source, line);
		if (line != "")
		{
			tempData = Mesh::split(line);

			if (tempData[0] == "v")
			{
				if (i_tempVertices == tempVertices.size())
				{
						tempVertices.resize(tempVertices.size()+1200);
						//std::cout << "size " << tempVertices.size() << '\n';
				}
				tempVertices[i_tempVertices++] = std::stof(tempData[1]);
				tempVertices[i_tempVertices++] = std::stof(tempData[2]);
				tempVertices[i_tempVertices++] = std::stof(tempData[3]);

				//std::cout <<"i: " << i_tempVertices << " size: " << tempVertices.size() << '\n';
			}
			else if (tempData[0] == "vn")
			{
				if (i_tempNormals == tempNormals.size())
				{
					tempNormals.resize(tempNormals.size()+1200);
					//std::cout << "size " << tempNormals.size() << '\n';
				}
				tempNormals[i_tempNormals++] = std::stof(tempData[1]);
				tempNormals[i_tempNormals++] = std::stof(tempData[2]);
				tempNormals[i_tempNormals++] = std::stof(tempData[3]);
				//std::cout <<"i: " << i_tempNormals << " size: " << tempNormals.size() << '\n';
			}

			else if (tempData[0] == "vt")
			{
				if (i_tempUvs == tempUvs.size())
				{
					tempUvs.resize(tempUvs.size()+1200);
					//std::cout << "size " << tempUvs.size() << '\n';
				}
				tempUvs[i_tempUvs++] = std::stof(tempData[1]);
				tempUvs[i_tempUvs++] = std::stof(tempData[2]);
				//std::cout <<"i: " << i_tempUvs << " " << tempData[1] << " " << tempData[2] << '\n';
			}

			else if (tempData[0] == "f")
			{

				for (int i = 0; i < static_cast<int>(tempData.size()-1); ++i) // -1 to avoid overflow
				{
					tempFaces = Mesh::split(tempData[i+1], '/');

					// takes the vertices in the right order and place them into a std::vector
					if (iVertices == m_vertices.size())
					{
						m_vertices.resize(m_vertices.size()+1200);
					}
					m_vertices[iVertices++] = tempVertices[(std::stoi(tempFaces[0])-1)*3];
					m_vertices[iVertices++] = tempVertices[((std::stoi(tempFaces[0])-1)*3)+1];
					m_vertices[iVertices++] = tempVertices[((std::stoi(tempFaces[0])-1)*3)+2];

					if (i_tempUvs > 0)
					{
						// takes the uvs in the right order and place them into a std::vector
						if (iUvs == m_uvs.size())
						{
							m_uvs.resize(m_uvs.size()+1200);
						}
//						std::cout << tempFaces[0] << " " << tempFaces[1] << " " << tempFaces[2] <<'\n';
						m_uvs[iUvs++] = tempUvs[(std::stoi(tempFaces[1])-1)*2];
						m_uvs[iUvs++] = tempUvs[((std::stoi(tempFaces[1])-1)*2)+1];
					}
					// takes the normals in the right order and place them into a std::vector
					if (iNormals == m_normals.size())
					{
						m_normals.resize(m_normals.size()+1200);
					}
					m_normals[iNormals++] = tempNormals[(std::stoi(tempFaces[2])-1)*3];
					m_normals[iNormals++] = tempNormals[((std::stoi(tempFaces[2])-1)*3)+1];
					m_normals[iNormals++] = tempNormals[((std::stoi(tempFaces[2])-1)*3)+2];


					if (tempVertices[(std::stoi(tempFaces[0])-1)*3] < m_minX)
						m_minX = tempVertices[(std::stoi(tempFaces[0])-1)*3];
					else if (tempVertices[(std::stoi(tempFaces[0])-1)*3] > m_maxX)
						m_maxX = tempVertices[(std::stoi(tempFaces[0])-1)*3];
					if (tempVertices[((std::stoi(tempFaces[0])-1)*3)+1] < m_minY)
						m_minY = tempVertices[((std::stoi(tempFaces[0])-1)*3)+1];
					else if (tempVertices[((std::stoi(tempFaces[0])-1)*3)+1] > m_maxY)
						m_maxY = tempVertices[((std::stoi(tempFaces[0])-1)*3)+1];
					if(tempVertices[((std::stoi(tempFaces[0])-1)*3)+2] < m_minZ)
						m_minZ = tempVertices[((std::stoi(tempFaces[0])-1)*3)+2];
					else if(tempVertices[((std::stoi(tempFaces[0])-1)*3)+2] > m_maxZ)
						m_maxZ = tempVertices[((std::stoi(tempFaces[0])-1)*3)+2];
					tempFaces.clear();
				}
			}
		}
		--len;
		tempData.clear();
		line.clear();
	}
	Source.close();
	if (iVertices < m_vertices.size())
	{
		m_vertices.resize(iVertices);
	}
	if (iNormals < m_normals.size())
	{
		m_normals.resize(iNormals);
	}
	if (iUvs < m_uvs.size())
	{
		m_uvs.resize(iUvs);
	}
	//std::cout << m_name << "\t" << tempVertices.size() << "\t" << m_vertices.size() <<std::endl;
}

//----------------------------------------------------------------------------------------------------------------------

std::vector<std::string> Mesh::split(std::string _stringIn, char _splitChar)
{
	int count = 0;
	std::string tempString;
	std::vector<std::string> retVector;
	for (int i = 0; i < static_cast<int>(_stringIn.length()+1); i++)
	{
		if(tempString.length() < 1 && _stringIn[i] == ' ') continue;
		else if(_stringIn[i] != _splitChar)
		{
			tempString += _stringIn[i];
		}
		else
		{
			retVector.resize(retVector.size() + 1);
			retVector[count] = tempString;
			tempString.clear();
			count++;
		}

		if(i == static_cast<int>(_stringIn.length()))
		{
			retVector.resize(retVector.size()+1);
			retVector[count] = tempString;
			tempString.clear();
			count++;
		}
	}
	return retVector;
}

//----------------------------------------------------------------------------------------------------------------------

void Mesh::write(std::string _destination)
{
	Mesh::write(m_vertices, m_normals, _destination);
}

//----------------------------------------------------------------------------------------------------------------------

void Mesh::write(std::vector<float> const & _vertices, std::vector<float>const & _normals, std::string _destination)
{
	std::ofstream out;
	out.open(_destination);
	out.clear();
	for(unsigned int i = 0; i < _vertices.size(); i+=3)
	{
		out << "v " << _vertices[i] << " " << _vertices[i+1] << " " << _vertices[i+2] << "\n";
	}
	for(unsigned int i = 0; i < _normals.size(); i+=3)
	{
		out << "vn " << _normals[i] << " " << _normals[i+1] << " " << _normals[i+2] << "\n";
	}

	for(unsigned int i = 0; i < _vertices.size()/9; ++i)
	{
		out << "f " << (i*3)+1 << "//"<< (i*3)+1 << " " << (i*3)+2 << "//" << (i*3)+2 << " " << (i*3)+3 << "//" << (i*3)+3 << "\n";
	}
}
//----------------------------------------------------------------------------------------------------------------------

