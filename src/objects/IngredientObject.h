#pragma once

#include <objects/GameObject.h>
#include <string>
#include <unordered_map>

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

    static std::unordered_map<IngredientStatus, std::string> IngredientStatusToString;

    static std::unordered_map<std::string, IngredientStatus> stringToIngredientStatus;

public:
    IngredientObject(int ID);

    IngredientObject();

    ~IngredientObject();

    void setName(std::string name);

    void setStatus(IngredientStatus status);

    std::string getStringStatus();

    void setStatusFromString(std::string);


    std::string getName();

    IngredientStatus getStatus();

    void draw();

    GLuint getTextureID();
};