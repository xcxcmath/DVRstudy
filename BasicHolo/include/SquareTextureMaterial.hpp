//
// Created by 김 범주 on 30/09/2019.
//

#ifndef DVRSTUDY_SQUARETEXTUREMATERIAL_HPP
#define DVRSTUDY_SQUARETEXTUREMATERIAL_HPP

#include <GLEngine/Base/Material.hpp>

class SquareTextureMaterial : public GLEngine::Material {
public:
    explicit SquareTextureMaterial(GLuint texUnit);
    ~SquareTextureMaterial() override = default;
};

#endif //DVRSTUDY_SQUARETEXTUREMATERIAL_HPP
