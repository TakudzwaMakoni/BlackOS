//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_KFIELD_H
#define BLACKOS_KFIELD_H

#include "Directives.h"
#include <string>

namespace BlackOS {
namespace DisplayKernel {

/// BlackOS Field Object
struct Kfield {
public:
  Kfield(std::string const &name, directive const script,
         std::string const &message = "");
  std::string name() const;
  directive script() const;
  std::string message() const;
  void setName(std::string const &name);

  //~Kfield();

private:
  std::string _name;
  directive _script = BlackOS::DisplayKernel::Directives::doNothing;
  std::string _message;
};
} // namespace DisplayKernel
} // namespace BlackOS
#endif // BLACKOS_KFIELD_H
