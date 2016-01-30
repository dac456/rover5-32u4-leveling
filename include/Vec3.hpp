#ifndef __VEC3_HPP
#define __VEC3_HPP


template<typename T>
class Vec3{
public:
    T _v[3];

public:
    Vec3(){
        for(int i=0; i<3; i++){
            _v[i] = static_cast<T>(0);
        }
    }
    Vec3(T* v){
        _v = v;
    }
    ~Vec3(){}
};

#endif
