#ifndef Field_H
#define Field_H

#include <iostream>
#include <typeinfo>    // Required for typeid
#include <type_traits> // required for type of variable

template <typename T>
class Field_
{
public:
    // constructor
    Field_(int ni, int nj, int nk) : ni{ni}, nj{nj}, nk{nk}
    {
        data = new T **[ni]; // ni pointers-to-pointers
        for (int i = 0; i < ni; i++)
        {
            data[i] = new T *[nj]; // nj pointers to doubles
            for (int j = 0; j < nj; j++)
                data[i][j] = new T[nk]; // nk double
        }
        // std::cout << typeid(data[0][0][0]).name() << std::endl;

        // if (!(std::is_same<decltype(data[0][0][0]), vec3<double>>))
        // {
        //     (*this) = 0;
        // }

        std::cout << "At the end of Field constructor" << std::endl;
    }

    // copy constructor
    Field_(const Field_ &other) : Field_{other.ni, other.nj, other.nk}
    {
        for (int i = 0; i < ni; i++)
            for (int j = 0; j < nj; j++)
                for (int k = 0; k < nk; k++)
                    data[i][j][k] = other(i, j, k);
    }

    // move constructor
    Field_(Field_ &&other) : ni{other.ni}, nj{other.nj}, nk{other.nk}
    {
        // if (data)
        //     ~Field_();        // deallocate own data
        data = other.data;    // steal the data
        other.data = nullptr; // invalidate
    }

    // move assignment operator
    Field_<T> &operator=(Field_ &&f)
    {
        // if (data)
        //     ~Field_(); // deallocate own data
        data = f.data;
        f.data = nullptr;
        return *this;
    }

    // destructor, frees memory in reverse order
    ~Field_()
    {
        if (data == nullptr)
            return; // return if unallicated
        for (int i = 0; i < ni; i++)
        {
            for (int j = 0; j < nj; j++)
            {
                delete data[i][j];
            }
            delete data[i];
        }
        delete[] data;
        data = nullptr; // mark as free
    }

    // overload the array access operator []
    T **operator[](int i) { return data[i]; }

    // read-only acces to data[i][j][k]
    T operator()(int i, int j, int k) const
    {
        return data[i][j][k];
    }

    // Overloading the assignment operator to allow setting all field entries to a constant value
    Field_<T> &operator=(T s)
    {
        for (int i = 0; i < ni; i++)
            for (int j = 0; j < nj; j++)
                for (int k = 0; k < nk; k++)
                    data[i][j][k] = s;

        return (*this); // return reference to the self
    }

    Field_<T> &operator/=(const Field_<T> &other)
    {
        for (int i = 0; i < ni; i++)
            for (int j = 0; j < nj; j++)
                for (int k = 0; k < nk; k++)
                    if (other.data[i][j][k] != 0)
                        data[i][j][k] /= other.data[i][j][k];
                    else
                        data[i][j][k] = 0;

        return (*this);
    }

    Field_<T> &operator+=(const Field_<T> &other)
    {
        for (int i = 0; i < ni; i++)
            for (int j = 0; j < nj; j++)
                for (int k = 0; k < nk; k++)
                    data[i][j][k] += other(i, j, k);
        return (*this);
    }

    Field_<T> &operator*=(double s)
    {
        for (int i = 0; i < ni; i++)
            for (int j = 0; j < nj; j++)
                for (int k = 0; k < nk; k++)
                    data[i][j][k] *= s;
        return (*this);
    }

    friend Field_<T> operator*(double s, const Field_<T> &f)
    {
        Field_<T> r(f);
        return std::move(r *= s); // force move
    }

    template <typename S>
    friend std::ostream &operator<<(std::ostream &os, const Field_<S> &f);

    const int ni, nj, nk;

protected:
    T ***data; // pointer of type T
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Field_<T> &f)
{
    for (int k = 0; k < f.nk; k++, os << "\n") // new line after each "k"
    {
        for (int j = 0; j < f.nj; j++)
        {
            for (int i = 0; i < f.ni; i++)
                os << f.data[i][j][k] << " ";
        }
    }
    return os;
}

using Field = Field_<double>; // field of doubles
using FieldI = Field_<int>;   // field of integers

template <typename T>
struct vec3
{
    vec3(const T u, const T v, const T w) : d{u, v, w} {}
    vec3(const T a[3]) : d{a[0], a[1], a[2]} {}
    vec3() : d{0, 0, 0} {}
    T &operator[](int i) { return d[i]; }
    T operator()(int i) const { return d[i]; }
    vec3<T> &operator=(double s)
    {
        d[0] = s;
        d[1] = s;
        d[2] = s;
        return (*this);
    }
    vec3<T> &operator+=(vec3<T> o)
    {
        d[0] += o[0];
        d[1] += o[1];
        d[2] += o[2];
        return (*this);
    }
    vec3<T> &operator-=(vec3<T> o)
    {
        d[0] -= o[0];
        d[1] -= o[1];
        d[2] -= o[2];
        return (*this);
    }

protected:
    T d[3];
};

using double3 = vec3<double>;
using int3 = vec3<int>;

using Field3 = Field_<double3>; // field of double3 i.e. field of vectors double3=vec3<double>

// vec3-vec3 operations
template <typename T> // addition of two vec3s
vec3<T> operator+(const vec3<T> &a, const vec3<T> &b)
{
    return vec3<T>(a(0) + b(0), a(1) + b(1), a(2) + b(2));
}

template <typename T> // substraction of two vec3s
vec3<T> operator-(const vec3<T> &a, const vec3<T> &b)
{
    return vec3<T>(a(0) - b(0), a(1) - b(1), a(2) - b(2));
}

template <typename T> // element-wise multiplication of two vec3s
vec3<T> operator*(const vec3<T> &a, const vec3<T> &b)
{
    return vec3<T>(a(0) * b(0), a(1) * b(1), a(2) * b(2));
}

template <typename T> // element-wise division of two vec3s
vec3<T> operator/(const vec3<T> &a, const vec3<T> &b)
{
    return vec3<T>(a(0) / b(0), a(1) / b(1), a(2) / b(2));
}

// vec3-scalar operations
template <typename T> // scalar multiplication 1
vec3<T> operator*(const vec3<T> &a, T s)
{
    return vec3<T>(a(0) * s, a(1) * s, a(2) * s);
}

template <typename T> // scalar multiplication 2
vec3<T> operator*(T s, const vec3<T> &a)
{
    return vec3<T>(a(0) * s, a(1) * s, a(2) * s);
}

// ouput
template <typename T> // ostream output
std::ostream &operator<<(std::ostream &out, vec3<T> &v)
{
    out << v[0] << " " << v[1] << " " << v[2];
    return out;
}

#endif