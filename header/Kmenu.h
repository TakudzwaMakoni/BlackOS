//
// Created by Takudzwa Makoni on 2019-07-12.
//

#ifndef BLACKOS_KMENU_H
#define BLACKOS_KMENU_H

#include <string>
#include <vector>
#include "Kfield.h"


typedef std::vector<Kfield> fieldVector;
class Kmenu{

public:
    Kmenu( std::string &name, fieldVector &fields, std::vector<int> &size);

    fieldVector fields() const;
    std::vector<int> size() const;
    std::string name() const;

private:
    const fieldVector m_fields;
    const std::vector<int> m_size;
    const std::string m_name;
};



#endif //BLACKOS_KMENU_H
