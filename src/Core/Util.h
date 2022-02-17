#pragma once
#include <fstream>
#include <string>
#include <functional>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define BT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace MyWorld
{
    std::string readText(const std::string& filepath);

    struct ColorVec
    {
        float r, g, b, a;
        ColorVec() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
        ColorVec(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
    };
}
