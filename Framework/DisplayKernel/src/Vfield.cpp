//
// Created by Takudzwa Makoni on 2019-07-27.
//
#include "../inc/Vfield.h"

namespace BlackOS {
namespace DisplayKernel {

Vfield::Vfield(const std::string &name, const directive script,
               const std::string &message)
    : _name(name), _script(script), _message(message) {}
std::string Vfield::name() const { return this->_name; }
directive Vfield::script() const { return this->_script; }
std::string Vfield::message() const { return this->_message; }
void Vfield::setName(const std::string &name) { _name = name; }
} // namespace DisplayKernel
} // namespace BlackOS
