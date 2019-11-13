//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KFIELD_H
#define BLACKOS_KFIELD_H

#include "Directives.h"
#include <string>
namespace BlackOSDisplay {

typedef void (*directive)();
/// BlackOS Field Object
class Kfield {
public:
  Kfield(const std::string name = "", const std::string message = "",
         const directive script = Directives::doNothing);
  std::string name() const;
  directive script() const;
  std::string message() const;
  void setName(const std::string &name);

private:
  std::string _name;
  directive _script;
  std::string _message;
};
} // namespace BlackOSDisplay

#endif // BLACKOS_KFIELD_H
