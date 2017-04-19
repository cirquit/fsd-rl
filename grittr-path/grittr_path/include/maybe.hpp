/** \file maybe.hpp
 *  Definition of the datatype Maybe
 *
 *  This class is useful to show possible failures in the typelevel
 *  May return **Something** or **Nothing**
 */

#ifndef GRITTR_PATH_UTIL_MAYBE_HPP
#define GRITTR_PATH_UTIL_MAYBE_HPP

namespace grittr {
namespace path {
namespace util {

template <typename Success>
class Maybe {
public:
    Maybe(Success s)
    : success(true)
    , s(s) {}

    Maybe() {}

    Success getSuccess() { return s; }

    bool successful() const { return success; } 
    bool failed() const { return !successful(); }

private:
    bool success = false;
    Success s;
};

template <typename T>
Maybe<T> Nothing() {
    return Maybe<T>();
}

template <typename T>
Maybe<T> Something(T t) {
    return Maybe<T>(t);
}

}
}
}

#endif 