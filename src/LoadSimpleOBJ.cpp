
// Cabeçalhos necessários (para esta função), acrescentar ao seu código 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Mesh 
{
    GLuint VAO; 
    string texturePath;
};


string loadMTL(const string& mtlFilePath, const string& directory)
{
    std::ifstream mtlFile(mtlFilePath);
    if (!mtlFile.is_open()) 
    {
        std::cerr << "Erro ao abrir arquivo MTL: " << mtlFilePath << std::endl;
        return "";
    }
    
    string line;
    string texturePath = "";
    
    while (std::getline(mtlFile, line)) 
    {
        std::istringstream ss(line);
        string word;
        ss >> word;
        
        if (word == "map_Kd") 
        {
            ss >> texturePath;
            if (texturePath.find(':') == string::npos && texturePath[0] != '/') 
            {
                texturePath = directory + texturePath;
            }
            break;
        }
    }
    
    mtlFile.close();
    return texturePath;
}

int loadSimpleOBJ(string filePATH, int &nVertices, string &texturePath)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<GLfloat> vBuffer;
    
    glm::vec3 color = glm::vec3(1.0, 0.0, 0.0);
    string mtlFileName = ""; 
    texturePath = ""; 
    
    std::ifstream arqEntrada(filePATH.c_str());
    if (!arqEntrada.is_open()) 
    {
        std::cerr << "Erro ao tentar ler o arquivo " << filePATH << std::endl;
        return -1;
    }
    
    string directory = filePATH.substr(0, filePATH.find_last_of("/\\") + 1);
    
    std::string line;
    while (std::getline(arqEntrada, line)) 
    {
        std::istringstream ssline(line);
        std::string word;
        ssline >> word;
        
        if (word == "mtllib") 
        {
            ssline >> mtlFileName;
            string mtlFilePath = directory + mtlFileName;
            
            texturePath = loadMTL(mtlFilePath, directory);
        }
        else if (word == "v") 
        {
            glm::vec3 vertice;
            ssline >> vertice.x >> vertice.y >> vertice.z;
            vertices.push_back(vertice);
        } 
        else if (word == "vt") 
        {
            glm::vec2 vt;
            ssline >> vt.s >> vt.t;
            texCoords.push_back(vt);
        } 
        else if (word == "vn") 
        {
            glm::vec3 normal;
            ssline >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        } 
        else if (word == "f")
        {
            while (ssline >> word) 
            {
                int vi = 0, ti = 0, ni = 0;
                std::istringstream ss(word);
                std::string index;
                
                if (std::getline(ss, index, '/')) 
                    vi = !index.empty() ? std::stoi(index) - 1 : 0;
                if (std::getline(ss, index, '/')) 
                    ti = !index.empty() ? std::stoi(index) - 1 : 0;
                if (std::getline(ss, index)) 
                    ni = !index.empty() ? std::stoi(index) - 1 : 0;
                
                vBuffer.push_back(vertices[vi].x);
                vBuffer.push_back(vertices[vi].y);
                vBuffer.push_back(vertices[vi].z);
                
                if (ti >= 0 && ti < texCoords.size()) 
                {
                    vBuffer.push_back(texCoords[ti].s);
                    vBuffer.push_back(texCoords[ti].t);
                } 
                else 
                {
                    vBuffer.push_back(0.0f);
                    vBuffer.push_back(0.0f);
                }
                
                vBuffer.push_back(color.r);
                vBuffer.push_back(color.g);
                vBuffer.push_back(color.b);
            }
        }
    }
    
    arqEntrada.close();
    
    if (!mtlFileName.empty()) {
        std::cout << "Nome do arquivo de textura a ser carregado: " << directory + mtlFileName << std::endl;
    }

    
    GLuint VBO, VAO;
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vBuffer.size() * sizeof(GLfloat), vBuffer.data(), GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    nVertices = vBuffer.size() / 8;
    
    
    return VAO;
}
