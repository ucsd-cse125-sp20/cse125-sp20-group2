#include <objects/IngredientObject.h>

IngredientObject::IngredientObject(int ID) : GameObject(ID) {
    this->objType = INGREDIENT;
    
    //Config::get("Tomato_Image");
    
    // get filename (path and directory) to inventory image?
    this->textureID = 0;     // remove this line when path and directory is included
    // this->textureID = TextureFromFile(path, directory);

    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &(this->VBO));
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    glGenBuffers(1, &(this->EBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexCoord), (GLvoid*)offsetof(VertexCoord, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexCoord), (GLvoid*)offsetof(VertexCoord, texcoord));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
    
IngredientObject::~IngredientObject() {
    if (this->textureID != 0) {
        glDeleteTextures(1, &(this->textureID));
        this->textureID = 0;
    }
}

void IngredientObject::setName(std::string name) {
    this->name = name;
};

void IngredientObject::setStatus(IngredientStatus status) {
    this->status = status;
}

std::string IngredientObject::getName() {
    return this->name;
};

IngredientStatus IngredientObject::getStatus() {
    return this->status;
}

void IngredientObject::draw() {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glBindVertexArray(this->VAO);

    glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint IngredientObject::getTextureID() {
    return this->textureID;
}