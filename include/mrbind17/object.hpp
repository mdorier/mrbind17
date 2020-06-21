#ifndef MRBIND17_OBJECT_H_
#define MRBIND17_OBJECT_H_

#include <mruby.h>

namespace mrbind17 {

class module;

/**
 * @brief The object class wraps an mrb_value handle to
 * work on such a handle in a C++ oriented way. 
 */
class object {

  public:

    object(const module& mod);

    object(mrb_state* mrb)
    : m_mrb(mrb)
    , m_value(mrb_nil_value()) {}

    object(mrb_state* mrb, mrb_value val)
    : m_mrb(mrb)
    , m_value(val) {}

    template<typename T>
    object(mrb_state* mrb, T&& val);

    template<typename T>
    object(const module& mod, T&& val);

    template<typename T>
    auto as() const; 

    object(const object&) = default;

    object(object&&) = default;

    object& operator=(const object&) = default;

    object& operator=(object&&) = default;

    virtual ~object() = default;

    mrb_state* mrb() const { return m_mrb; }

    mrb_value value() const { return m_value; }

  private:

    mrb_state* m_mrb;
    mrb_value  m_value;
};

}

#include <mrbind17/module.hpp>
#include <mrbind17/type_binder.hpp>

namespace mrbind17 {

object::object(const module& mod)
: m_mrb(mod.m_mrb)
, m_value(mrb_nil_value()) {}

template<typename T>
object::object(mrb_state* mrb, T&& val)
: m_mrb(mrb)
, m_value(cpp_to_mrb(mrb, val)) {}

template<typename T>
object::object(const module& mod, T&& val)
: m_mrb(mod.m_mrb)
, m_value(cpp_to_mrb(m_mrb, val)) {}

template<typename T>
auto object::as() const {
  return detail::mrb_to_cpp<T>(m_mrb, m_value);
}

}

#endif
