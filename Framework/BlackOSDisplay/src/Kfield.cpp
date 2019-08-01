//
// Created by Takudzwa Makoni on 2019-07-27.
//
#include "../inc/Kfield.h"

using namespace BlackOSDisplay;

Kfield::Kfield(const std::string name,  const std::string message,const directive script)
        : m_name(name), m_script(script), m_message(message) {}
std::string Kfield::name() const { return this->m_name; }
directive Kfield::script() const { return this->m_script; }
std::string Kfield::message() const { return this->m_message; }
