/*
 Copyright (c) 2020 Matthieu Dorier <matthieu.dorier@gmail.com>
 All rights reserved. Use of this source code is governed by a
 BSD-style license that can be found in the LICENSE file.
*/
#ifndef MRBIND17_INTERPRETER_H_
#define MRBIND17_INTERPRETER_H_

#include <mrbind17/object.hpp>
#include <mrbind17/module.hpp>
#include <mrbind17/exception.hpp>
#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/variable.h>
#include <string>
#include <exception>

namespace mrbind17 {

/**
 * @brief The interpreter object enables creating an MRuby state
 * and executing scripts from it. It extends the module class, which
 * enables it to be extended with new modules, classes, and functions.
 */
class interpreter : public module {

  public:

  /**
   * @brief Constructor. Creates a new MRuby state.
   */
  interpreter()
  : module(mrb_open()) {}

  /**
   * @brief The copy-constructor is deleted.
   */
  interpreter(const interpreter&) = delete;

  /**
   * @brief Move constructor.
   */
  interpreter(interpreter&& other)
  : module(std::move(other)) {
    other.m_mrb = nullptr;
  }

  /**
   * @brief The copy-assignment operator is deleted.
   */
  interpreter& operator=(const interpreter&) = delete;

  /**
   * @brief Move-assignment operator.
   */
  interpreter& operator=(interpreter&& other) {
    if(m_mrb == other.m_mrb) return *this;
    if(m_mrb) mrb_close(m_mrb);
    m_mrb = other.m_mrb;
    other.m_mrb = nullptr;
    return *this;
  }

  /**
   * @brief The destructor will close the underlying Mruby state
   * and free up its resources.
   */
  ~interpreter() {
    if(m_mrb) mrb_close(m_mrb);
  }

  /**
   * @brief Sets a global variable in the interpreter.
   *
   * @tparam ValueType Type of the value.
   * @param name Name of the global variable (including $).
   * @param val Value.
   */
  template<typename ValueType>
  void set_global(const char* name, const ValueType& val) {
    mrb_sym sym = mrb_intern_static(m_mrb, name, strlen(name));
    mrb_gv_set(m_mrb, sym, detail::cpp_to_mrb(m_mrb, val));
  }

  /**
   * @brief Gets the value of a global variable.
   *
   * @tparam ValueType Type of the value.
   * @param name Name of the global variable (including $).
   *
   * @return The value associated with a global variable.
   */
  template<typename ValueType>
  ValueType get_global(const char* name) {
    mrb_sym sym = mrb_intern_static(m_mrb, name, strlen(name));
    return detail::mrb_to_cpp<ValueType>(m_mrb, mrb_gv_get(m_mrb, sym));
  }

  /**
   * @brief Executes the given Ruby script, provided as a null-terminated string.
   *
   * @param script Ruby script.
   *
   * @return The value returned by the Ruby script.
   */
  object execute(const char* script) {
    auto val = mrb_load_string(m_mrb, script);
    if(m_mrb->exc) {
      exception::translate_and_throw_exception(m_mrb, mrb_obj_value(m_mrb->exc));
    }
    return object(m_mrb, val);
  }

};

}

#endif
