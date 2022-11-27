#include "my_math.h"

#include <cmath>

float length(Vector2 v)
{
    return sqrtf(length_sq(v));
}