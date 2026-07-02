#pragma once
#include "../Mesh/Mesh.h"
#include <vector>

class Model
{
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
};

