//
// Created by Takudzwa Makoni on 2019-07-12.
//

#include "../header/Kfield.h"

Kfield::Kfield(std::string &name, directive script) : m_name(name), m_script(script) {};
std::string Kfield::name() const { return m_name; };
directive Kfield::script() const { return m_script; };