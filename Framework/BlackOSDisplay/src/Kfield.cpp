//
// Created by Takudzwa Makoni on 2019-07-27.
//
#include "../inc/Kfield.h"

using namespace BlackOSDisplay;

Kfield::Kfield(const std::string name, const std::string message,
               const directive script)
    : _name(name), _script(script), _message(message) {}
std::string Kfield::name() const { return this->_name; }
directive Kfield::script() const { return this->_script; }
std::string Kfield::message() const { return this->_message; }
void Kfield::setName(const std::string &name) { _name = name; }
