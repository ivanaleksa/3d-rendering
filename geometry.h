#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <vector>


template<typename T>
class Vec3d
{
private:
    T x, y, z;

public:
    Vec3d() : x(T()), y(T()), z(T()) {}

    Vec3d(T x_val, T y_val, T z_val) : x(x_val), y(y_val), z(z_val) {}

    template<typename U>
    Vec3d(const Vec3d<U>& other) : x(static_cast<T>(other.getX())), y(static_cast<T>(other.getY())), z(static_cast<T>(other.getZ())) {}

    T getX() const { return x; }
    T getY() const { return y; }
    T getZ() const { return z; }
    void setX(T x_val) { x = x_val; }
    void setY(T y_val) { y = y_val; }
    void setZ(T z_val) { z = z_val; }

    Vec3d operator+(const Vec3d& other) const {
        return Vec3d(x + other.x, y + other.y, z + other.z);
    }

    Vec3d operator-(const Vec3d& other) const {
        return Vec3d(x - other.x, y - other.y, z - other.z);
    }

    Vec3d operator*(const T& scalar) const {
        return Vec3d(x * scalar, y * scalar, z * scalar);
    }

    T operator*(const Vec3d& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3d operator^(const Vec3d& other) const {
        return Vec3d(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
            );
    }

    T& operator[](const int i) {
        return i == 0 ? x : (i == 1 ? y : z);
    }

    T norm() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3d& normalize(T l = 1) {
        T n = norm();
        if (n != 0) {
            *this = (*this) * (l / n);
        }
        return *this;
    }
};


const int DEFAULT_ALLOC = 4;

class Matrix
{
private:
    std::vector<std::vector<float>> m;
    int rows, cols;

public:
    Matrix(int r = DEFAULT_ALLOC, int c = DEFAULT_ALLOC);
    inline int nrows();
    inline int ncols();

    static Matrix identity(int dimensions);
    std::vector<float>& operator[](const int i);
    Matrix operator*(const Matrix& a);
    Matrix transpose();
    Matrix inverse();
};

#endif // GEOMETRY_H
