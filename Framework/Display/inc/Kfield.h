//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KFIELD_H
#define BLACKOS_KFIELD_H

#include "Directives.h"
#include <string>
namespace BlackOS {
namespace Display {

/// BlackOS Field Object
class Kfield {
public:
  Kfield(const std::string &name, const directive script,
         const std::string &message = "");
  Kfield();
  std::string name() const;
  directive script() const;
  std::string message() const;
  void setName(const std::string &name);

private:
  std::string _name;
  directive _script = BlackOS::Display::Directives::doNothing;
  std::string _message;
};
} // namespace Display
} // namespace BlackOS
#endif // BLACKOS_KFIELD_H