#pragma once

#include <objects/GameObject.h>
#include <string>

enum class IngredientStatus
{
    Raw,
    Cut,
    Cooked
};

struct VertexCoord {
    glm::vec3 position;
    glm::vec2 texcoord;
};



class IngredientObject : public GameObject
{
protected:
    std::string name;

    IngredientStatus status;

    GLuint textureID;

    GLuint VAO, VBO, EBO;

public:
// FIXME: need to update coordinate for more than one ingredient
// FIXME: Need to make sure this is always on top regardless player's location
VertexCoord vert[4] = {
    glm::vec3(-1.f, 1.f, 0.f), glm::vec2(0.f, 1.f),
    glm::vec3(-1.f, 0.8f, 0.f), glm::vec2(0.f, 0.f),
    glm::vec3(-0.85f, 0.8f, 0.f), glm::vec2(1.f, 0.f),
    glm::vec3(-0.85, 1.f, 0.f), glm::vec2(1.f, 1.f)
};
unsigned nrOfVertices = sizeof(vert) / sizeof(VertexCoord);

GLuint indices[6] = {
    0,1,2, // Triangle 1
    0,2,3  // Triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);
    IngredientObject(int ID);

    IngredientObject();

    ~IngredientObject();

    void setName(std::string name);

    void setStatus(IngredientStatus status);

    std::string getName();

    IngredientStatus getStatus();

    void draw();

    GLuint getTextureID();
};