#ifndef __VECTOR_2D__
#define __VECTOR_2D__
#include <functional>

class Vector2D {
    public:
        float x, y;

        Vector2D();
        Vector2D(float single);
        Vector2D(float x, float y);
        Vector2D(const Vector2D& vec);
        Vector2D(Vector2D&& vec);
        auto operator=(const Vector2D& vec) -> Vector2D&;
        auto operator=(Vector2D&& vec) -> Vector2D&;
        ~Vector2D();

        auto operator+(const Vector2D& other) -> Vector2D;
        auto operator-(const Vector2D& other) -> Vector2D;
        auto operator*(float scalar) -> Vector2D;
        auto operator/(float scalar) -> Vector2D;

        auto operator+=(const Vector2D& other) -> Vector2D&;
        auto operator-=(const Vector2D& other) -> Vector2D&;
        auto operator*=(float scalar) -> Vector2D&;
        auto operator/=(float scalar) -> Vector2D&;

        [[nodiscard]] auto Magnitude() const -> float;
        [[nodiscard]] auto Normalized() const -> Vector2D;
        [[nodiscard]] auto GetOrtho() const -> Vector2D;

        auto map(std::function<float(float)> function) -> void;
        [[nodiscard]] auto to_string() -> std::string;
};

#endif
