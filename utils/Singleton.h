//
//  Singleton.h
//  Adventures on the farm
//
//  Created by Jochen Heizmann on 03.04.13.
//
//

#ifndef Adventures_on_the_farm_Singleton_h
#define Adventures_on_the_farm_Singleton_h

template <typename T>
class Singleton
{
public:
    static T& getInstance()
    {
        static T oInstance;
        return oInstance;
    }

protected:
    Singleton() {}
    virtual ~Singleton() {}

private:
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&)
    {
        return *this;
    }
};

#endif
