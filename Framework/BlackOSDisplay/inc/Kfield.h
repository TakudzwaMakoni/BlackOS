//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KFIELD_H
#define BLACKOS_KFIELD_H

#include <string>
namespace BlackOSDisplay {

    typedef void (*directive)();
/// BlackOS Field Object
    class Kfield {
    public:
        Kfield(std::string &name, std::string &message, directive script);
        std::string name() const;
        directive script() const;
        std::string message() const;

    private:
        const std::string m_name;
        const directive m_script;
        const std::string m_message;
    };
} // namespace BlackOSDisplay


#endif //BLACKOS_KFIELD_H
