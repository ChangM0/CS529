#include "Matrix4.h"


Matrix4::Matrix4()
{
    // Initialize as identity matrix
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            data[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void Matrix4::updateElement(int row, int col, float value)
{
    if (row < 0 || row >= 4 || col < 0 || col >= 4) {
        throw std::out_of_range("Matrix indices out of range");
    }
    data[row][col] = value;
}

float Matrix4::getElement(int row, int col) const
{
    if (row < 0 || row >= 4 || col < 0 || col >= 4) {
        throw std::out_of_range("Matrix indices out of range");
    }
    return data[row][col];
}

const float* Matrix4::getData() const
{
    return &data[0][0];
}

Matrix4 Matrix4::operator*(const Matrix4& other)
{
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.data[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.data[i][j] += data[k][i] * other.data[j][k];
            }
        }
    }
    return result;
}

Vector3 Matrix4::operator*(const Vector3& vec) const
{
    float x = vec.x * data[0][0] + vec.y * data[1][0] + vec.z * data[2][0] + data[3][0];
    float y = vec.x * data[0][1] + vec.y * data[1][1] + vec.z * data[2][1] + data[3][1];
    float z = vec.x * data[0][2] + vec.y * data[1][2] + vec.z * data[2][2] + data[3][2];
    float w = vec.x * data[0][3] + vec.y * data[1][3] + vec.z * data[2][3] + data[3][3];


    // Perform perspective division
    if (w != 0) {
        x /= w;
        y /= w;
        z /= w;
    }

    return Vector3(x, y, z);
}

Matrix4 Matrix4::translation(float tx, float ty, float tz)
{
    Matrix4 result;
    result.data[3][0] = tx;
    result.data[3][1] = ty;
    result.data[3][2] = tz;
    return result;
}

Matrix4 Matrix4::scale(float sx, float sy, float sz)
{
    Matrix4 result;
    result.data[0][0] = sx;
    result.data[1][1] = sy;
    result.data[2][2] = sz;
    return result;
}

Matrix4 Matrix4::rotationX(float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);
    Matrix4 result;
    result.data[1][1] = c;
    result.data[1][2] = -s;
    result.data[2][1] = s;
    result.data[2][2] = c;
    return result;
}

Matrix4 Matrix4::rotationY(float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);
    Matrix4 result;
    result.data[0][0] = c;
    result.data[2][0] = -s;
    result.data[0][2] = s;
    result.data[2][2] = c;
    return result;
}

Matrix4 Matrix4::rotationZ(float angle)
{
    float c = std::cos(angle);
    float s = std::sin(angle);
    Matrix4 result;
    result.data[0][0] = c;
    result.data[1][0] = s;
    result.data[0][1] = -s;
    result.data[1][1] = c;
    return result;
}

Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float near, float far)
{
    Matrix4 result;
    result.data[0][0] = 2.0f / (right - left);
    result.data[1][1] = 2.0f / (top - bottom);
    result.data[2][2] = -2.0f / (far - near);
    result.data[0][3] = -(right + left) / (right - left);
    result.data[1][3] = -(top + bottom) / (top - bottom);
    result.data[2][3] = -(far + near) / (far - near);
    result.data[3][3] = 1.0f;
    return result;
}

Matrix4 Matrix4::perspective(float fov, float aspectRatio, float near, float far)
{
    Matrix4 result;
    float tanHalfFov = std::tan(fov / 2.0f);

    result.data[0][0] = 1.0f / (aspectRatio * tanHalfFov);
    result.data[1][1] = 1.0f / tanHalfFov;
    result.data[2][2] = -(far + near) / (far - near);
    result.data[2][3] = -1.0f;
    result.data[3][2] = -(2.0f * far * near) / (far - near);
    result.data[3][3] = 0.0f;

    return result;
}

Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
{
    Vector3 f = (center - eye).normalized();
    Vector3 s = f.cross(up.normalized());
    Vector3 u = s.cross(f);

    Matrix4 result;
    result.data[0][0] = s.x;
    result.data[0][1] = u.x;
    result.data[0][2] = -f.x;
    result.data[0][3] = 0.0f;

    result.data[1][0] = s.y;
    result.data[1][1] = u.y;
    result.data[1][2] = -f.y;
    result.data[1][3] = 0.0f;

    result.data[2][0] = s.z;
    result.data[2][1] = u.z;
    result.data[2][2] = -f.z;
    result.data[2][3] = 0.0f;

    result.data[3][0] = -s.dot(eye);
    result.data[3][1] = -u.dot(eye);
    result.data[3][2] = f.dot(eye);
    result.data[3][3] = 1.0f;

    return result;
}

