//
// Created by Takudzwa Makoni on 2019-07-12.
//

#ifndef BLACKOS_KFIELD_H
#define BLACKOS_KFIELD_H

#include "BlackOSScripts.h"
#include <string>


class Kfield {

public:
    Kfield(std::string &name, directive script);

    std::string name() const;
    directive script() const;

private:
    const std::string m_name;
    const directive m_script;
};


#endif //BLACKOS_KFIELD_H
