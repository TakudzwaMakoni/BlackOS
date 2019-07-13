//
// Created by Takudzwa Makoni on 2019-07-12.
//

#include "../header/Kmenu.h"


Kmenu::Kmenu( std::string &name, fieldVector &fields, std::vector<int> &size) : m_fields(fields), m_size(size), m_name(name){};
fieldVector Kmenu::fields() const { return m_fields; };
std::vector<int> Kmenu::size() const { return m_size; }
std::string Kmenu::name() const { return m_name; }