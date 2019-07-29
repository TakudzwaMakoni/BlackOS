//
// Created by Takudzwa Makoni on 2019-07-27.
//
#include "../inc/Kfield.h"

using namespace BlackOSDisplay;

Kfield::Kfield(std::string &name,  std::string &message, directive script)
        : m_name(name), m_script(script), m_message(message) {}
std::string Kfield::name() const { return m_name; }
directive Kfield::script() const { return m_script; }
std::string Kfield::message() const { return m_message; }