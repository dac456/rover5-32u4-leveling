#ifndef __MAT33_HPP
#define __MAT33_HPP

#include "Vec3.hpp"

template<typename T>
class Mat33{
public:
    T _m[9];

public:
    Mat33(){
        for(int i=0; i<9; i++){
            _m[i] = static_cast<T>(0);
        }
    }
    Mat33(T* m){
        _m = m;
    }
    ~Mat33(){

    }

    Mat33<T> mult(Mat33<T> rhs){
        Mat33<T> c;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                T s = static_cast<T>(0);
                for(int k=0; k<3; k++){
                    s += _m[k + (i*3)] * rhs._m[i + (k*3)];
                }
                c[j + (i*3)] = s;
            }
        }

        return c;
    }

    Vec3<T> mult(Vec3<T> rhs){
        Vec3<T> c;
        for(int i=0; i<3; i++){
            T s = static_cast<T>(0);
            for(int j=0; j<3; j++){
                s += _m[j + (i*3)] * rhs._v[j];
            }
            c[i] = s;
        }
    }
};

#endif
