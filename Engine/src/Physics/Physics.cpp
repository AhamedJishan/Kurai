#include "Physics.h"

#include <cmath>
#include <glm/geometric.hpp>

namespace Dawn::Physics
{
    bool Intersects(const Sphere& a, const Sphere& b)
    {
        float radiusSum = a.radius + b.radius;
        float dist = glm::length(a.center - b.center);
        return dist <= radiusSum;
    }

    // Ray–Sphere Intersection
    // 
    // O = ray Origin
    // D = ray Direction
    // C = sphere Center
    // R = sphere radius
    // OC = O - C
    // 
    // any Point P on Ray : P(t) = O + tD
    // for Point P to intersect sphere:
    // ||P(t) - C|| = R 
    // ||P(t) - C||^2 = R^2
    // ||O + tD - C||^2 = R^2
    // ||tD + OC||^2 = R^2
    // (D.D)t^2 + 2(OC.D)t + OC.OC - R^2 = 0
    // 
    // To use quadratic formula:
    // a = (D.D)
    // b = 2(OC.D)
    // c = OC.OC - R^2
    // 
    // discriminant = b^2 - 4ac
    // 
    // discriminant < 0     -> No hit
    // discriminant >= 0    -> potential hit
    // 
    // we choose smallest positive t (closest to ray origin)
    //
    bool Intersects(const Ray& ray, const Sphere& sphere, float& outT)
    {
        glm::vec3 OC = ray.origin - sphere.center;

        float a = glm::dot(ray.direction, ray.direction); // or 1 if normalized
        float b = 2.0f * glm::dot(ray.direction, OC);
        float c = glm::dot(OC, OC) - sphere.radius * sphere.radius;

        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0.0f)
            return false;

        float sqrtD = std::sqrt(discriminant);

        float t1 = (-b - sqrtD) / (2 * a);
        float t2 = (-b + sqrtD) / (2 * a);

        if (t1 >= 0.0f && t2 >= 0.0f)
            outT = glm::min(t1, t2);
        else if (t1 >= 0.0f)
            outT = t1;
        else if (t2 >= 0.0f)
            outT = t2;
        else
            return false;

        return true;
    }
}