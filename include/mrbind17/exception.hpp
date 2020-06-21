#ifndef MRBIND17_EXCEPTION_H_
#define MRBIND17_EXCEPTION_H_

#include <mrbind17/object.hpp>
#include <mruby.h>
#include <exception>

namespace mrbind17 {

class exception : public object, public std::exception {

  public:

  static void translate_and_throw_exception(mrb_state* mrb, mrb_value exc) {
    throw std::runtime_error("MRuby exception occured");
  }
};

}

#endif
