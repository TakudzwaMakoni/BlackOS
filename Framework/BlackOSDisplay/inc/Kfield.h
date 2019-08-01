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
        Kfield(const std::string name = "", const std::string message = "", const directive script = nullptr);
        std::string name() const;
        directive script() const;
        std::string message() const;

    private:
         std::string m_name;
         directive m_script;
         std::string m_message;
    };
} // namespace BlackOSDisplay


#endif //BLACKOS_KFIELD_H
