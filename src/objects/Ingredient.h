#pragma once

#include <objects/GameObject.h>
#include <string>
#include <unordered_map>

enum class IngredientStatus
{
    Raw,
    Cut,
    Boiled, 
    Fried
};


const std::string BUN = "Bun";
const std::string ONION = "Onion";
const std::string MEAT = "Meat";
const std::string CHEESE = "Cheese";

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

    static std::unordered_map<IngredientStatus, std::string> IngredientStatusToString;

    static std::unordered_map<std::string, IngredientStatus> stringToIngredientStatus;

public:
    Ingredient(int ID);

    Ingredient();

    ~Ingredient();

    void setName(std::string name);

    void setStatus(IngredientStatus status);

    std::string getStringStatus();

    void setStatusFromString(std::string);


    std::string getName();

    IngredientStatus getStatus();

    void draw();

    GLuint getTextureID();
};