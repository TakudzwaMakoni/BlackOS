//
// Created by Takudzwa Makoni on 2019-07-27.
//
#include "../inc/Kfield.h"

namespace BlackOS {
namespace Display {

Kfield::Kfield(const std::string &name, const directive script,
               const std::string &message)
    : _name(name), _script(script), _message(message) {}
std::string Kfield::name() const { return this->_name; }
directive Kfield::script() const { return this->_script; }
std::string Kfield::message() const { return this->_message; }
void Kfield::setName(const std::string &name) { _name = name; }
} // namespace Display
} // namespace BlackOS
