#pragma once

#include "raylib.h"

struct Vector2i {
    int x = 0;
    int y = 0;

    constexpr inline operator Vector2() {
        return Vector2{ (float)x, (float)y };
    }

    constexpr inline Vector2i(int x, int y) : x{ x }, y{ y } {}
    constexpr inline explicit Vector2i(Vector2 v2) : x{ (int)v2.x }, y{ (int)v2.y } {}

    constexpr Vector2i()                           = default;
    constexpr Vector2i(const Vector2i&)            = default;
    constexpr Vector2i(Vector2i&&)                 = default;
    constexpr Vector2i& operator=(const Vector2i&) = default;
    constexpr Vector2i& operator=(Vector2i&&)      = default;
};

constexpr inline bool operator==(Vector2i a, Vector2i b) { return a.x == b.x && a.y == b.y; }


constexpr inline Vector2 operator-(Vector2 a) { return Vector2{ -a.x, -a.y }; }
constexpr inline Vector2i operator-(Vector2i a) { return Vector2i{-a.x, -a.y}; }

constexpr inline Vector2 operator+=(Vector2& a, Vector2 b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

constexpr inline Vector2i operator+=(Vector2i& a, Vector2i b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

constexpr inline Vector2 operator-=(Vector2& a, Vector2 b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

constexpr inline Vector2i operator-=(Vector2i& a, Vector2i b)
{
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

constexpr inline Vector2 operator*=(Vector2& a, Vector2 b)
{
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

constexpr inline Vector2i operator*=(Vector2i& a, Vector2i b)
{
    a.x *= b.x;
    a.y *= b.y;
    return a;
}

constexpr inline Vector2 operator/=(Vector2& a, Vector2 b)
{
    a.x /= b.x;
    a.y /= b.y;
    return a;
}

constexpr inline Vector2i operator/=(Vector2i& a, Vector2i b)
{
    a.x /= b.x;
    a.y /= b.y;
    return a;
}

constexpr inline Vector2 operator+(Vector2 a, Vector2 b)    { return a += b; }
constexpr inline Vector2i operator+(Vector2i a, Vector2i b) { return a += b; }

constexpr inline Vector2 operator-(Vector2 a, Vector2 b)    { return a -= b; }
constexpr inline Vector2i operator-(Vector2i a, Vector2i b) { return a -= b; }

constexpr inline Vector2 operator*(Vector2 a, Vector2 b)    { return a *= b; }
constexpr inline Vector2i operator*(Vector2i a, Vector2i b) { return a *= b; }

constexpr inline Vector2 operator/(Vector2 a, Vector2 b)    { return a /= b; }
constexpr inline Vector2i operator/(Vector2i a, Vector2i b) { return a /= b; }

constexpr inline Vector2 operator*=(Vector2& a, float b) { return a *= Vector2{ b, b }; }
constexpr inline Vector2i operator*=(Vector2i& a, int b) { return a *= Vector2i{ b, b }; }

constexpr inline Vector2 operator/=(Vector2& a, float b) { return a /= Vector2{ b, b }; }
constexpr inline Vector2i operator/=(Vector2i& a, int b) { return a /= Vector2i{ b, b }; }

constexpr inline Vector2 operator*(Vector2 a, float b) { return a * Vector2{ b, b }; }
constexpr inline Vector2i operator*(Vector2i a, int b) { return a * Vector2i{ b, b }; }

constexpr inline Vector2 operator/(Vector2 a, float b) { return a / Vector2{ b, b }; }
constexpr inline Vector2i operator/(Vector2i a, int b) { return a / Vector2i{ b, b }; }

constexpr inline Vector2 operator*(float a, Vector2 b) { return b * a; }
constexpr inline Vector2i operator*(int a, Vector2i b) { return b * a; }

constexpr inline Vector2 operator/(float a, Vector2 b) { return b / a; }
constexpr inline Vector2i operator/(int a, Vector2i b) { return b / a; }

struct Rectangle2 {
    Vector2 min = {};
    Vector2 max = {};

    constexpr inline operator Rectangle() {
        return Rectangle{ min.x, min.y, (max.x - min.x), (max.y - min.y) };
    }

    constexpr inline Rectangle2(Vector2 min, Vector2 max) : min{ min }, max{ max } {}
    constexpr inline explicit Rectangle2(Rectangle rect) : min{ rect.x, rect.y }, max{ rect.x + rect.width, rect.y + rect.height } {}

    constexpr Rectangle2() = default;
    constexpr Rectangle2(const Rectangle2&) = default;
    constexpr Rectangle2(Rectangle2&&) = default;
    constexpr Rectangle2& operator=(const Rectangle2&) = default;
    constexpr Rectangle2& operator=(Rectangle2&&) = default;
};

struct Rectangle2i {
    Vector2i min = {};
    Vector2i max = {};

    constexpr inline operator Rectangle2() {
        return Rectangle2{ min, max };
    }

    constexpr inline operator Rectangle() {
        return Rectangle{ (float) min.x, (float) min.y, (float)(max.x - min.x), (float)(max.y - min.y) };
    }

    constexpr inline Rectangle2i(Vector2i min, Vector2i max) : min{ min }, max{ max } {}
    constexpr inline explicit Rectangle2i(Rectangle rect) : min{ rect.x, rect.y }, max{ rect.x + rect.width, rect.y + rect.height } {}

    constexpr Rectangle2i() = default;
    constexpr Rectangle2i(const Rectangle2i&) = default;
    constexpr Rectangle2i(Rectangle2i&&) = default;
    constexpr Rectangle2i& operator=(const Rectangle2i&) = default;
    constexpr Rectangle2i& operator=(Rectangle2i&&) = default;

    constexpr static Rectangle2i make_min_size(Vector2i min, Vector2i size)
    {
        return Rectangle2i{ min , min + size };
    }
};

constexpr inline Rectangle2 operator+=(Rectangle2& a, Vector2 b)
{
    a.min += b;
    a.max += b;
    return a;
}

constexpr inline Rectangle2i operator+=(Rectangle2i& a, Vector2i b)
{
    a.min += b;
    a.max += b;
    return a;
}

constexpr inline Rectangle2 operator-=(Rectangle2& a, Vector2 b)
{
    a.min -= b;
    a.max -= b;
    return a;
}

constexpr inline Rectangle2i operator-=(Rectangle2i& a, Vector2i b)
{
    a.min -= b;
    a.max -= b;
    return a;
}

constexpr inline Rectangle2 operator*=(Rectangle2& a, float b)
{
    Vector2 half_delta = (a.max - a.min) * (b - 1) * 0.5f;
    a.min -= half_delta;
    a.max += half_delta;
    return a;
}

constexpr inline Rectangle2 operator/=(Rectangle2& a, float b)
{
    Vector2 half_delta = (a.max - a.min) * ( 1.f / b - 1) * 0.5f;
    a.min -= half_delta;
    a.max += half_delta;
    return a;
}

constexpr inline Rectangle2 operator+(Rectangle2 a, Vector2 b) { return a += b; }
constexpr inline Rectangle2i operator+(Rectangle2i a, Vector2i b) { return a += b; }

constexpr inline Rectangle2 operator-(Rectangle2 a, Vector2 b) { return a -= b; }
constexpr inline Rectangle2i operator-(Rectangle2i a, Vector2i b) { return a -= b; }

constexpr inline Rectangle2 operator*(Rectangle2 a, float b) { return a *= b; }
constexpr inline Rectangle2 operator/(Rectangle2 a, float b) { return a /= b; }

template<typename T>
constexpr T lerp(T a, T b, float t)
{
    return static_cast<T>(a * (1 - t) + b * t);
}

template<typename T>
constexpr T fc_min(T a, T b)
{
    return a < b ? a : b;
}

template<typename T>
constexpr T fc_max(T a, T b)
{
    return a < b ? b : a;
}

template<typename T>
constexpr T clamp(T value, T min_value, T max_value)
{
    return fc_max(fc_min(value, max_value), min_value);
}

template<typename T>
constexpr T lerp_clamp(T a, T b, float t)
{
    t = clamp(t, 0.f, 1.f);
    return static_cast<T>(a * (1 - t) + b * t);
}

constexpr inline Color color_mix(Color a, Color b, float t)
{
    Vector4 blend = {};
    blend.x = lerp(a.r, b.r, t);
    blend.y = lerp(a.g, b.g, t);
    blend.z = lerp(a.b, b.b, t);
    blend.w = lerp(a.a, b.a, t);

    return Color{ (unsigned char)blend.x, (unsigned char)blend.y, (unsigned char)blend.z, (unsigned char)blend.w };
}

constexpr inline float length_sq(Vector2 v)
{
    return v.x * v.x + v.y * v.y;
}

float length(Vector2 v);