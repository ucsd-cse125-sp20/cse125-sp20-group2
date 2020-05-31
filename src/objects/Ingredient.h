#pragma once

#include <objects/GameObject.h>
#include <string>
#include <unordered_map>
#include <constants/gameplay_vars.h>

enum class IngredientStatus
{
    Raw,
    Cut,
    Boiled, 
    Fried
};

/**
raw -> boiled
raw -> fried
raw -> cut -> boiled
raw -> cut -> fried
raw -> cut**/

struct VertexCoord {
    glm::vec3 position;
    glm::vec2 texcoord;
};

class Ingredient : public GameObject
{
protected:
    std::string name;

    IngredientStatus status;

    GLuint textureID;

    GLuint VAO, VBO, EBO;

    int qualityIndex;


public:
    static std::unordered_map<IngredientStatus, std::string> IngredientStatusToString;

    static std::unordered_map<std::string, IngredientStatus> stringToIngredientStatus;
    
    bool clicked;

    Ingredient(int ID);

    Ingredient();

    ~Ingredient();

    void setName(std::string name);

    void setStatus(IngredientStatus status);

    int getQualityIndex();

    void setQualityIndex(int i);

    std::string getStringStatus();

    void setStatusFromString(std::string);

    std::string getDetailedName();

    std::string getName();

    IngredientStatus getStatus();

    void draw();

    GLuint getTextureID();

    void setClicked(bool c);

    int getClicked();
};