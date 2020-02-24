//
// Created by Takudzwa Makoni on 2019-07-27.
//

#ifndef BLACKOS_VFIELD_H
#define BLACKOS_VFIELD_H

#include "Directives.h"
#include <string>

namespace BlackOS {
namespace DisplayKernel {

/// BlackOS Field Object
struct Vfield {
public:
  Vfield(std::string const &name, directive const script,
         std::string const &message = "");
  std::string name() const;
  directive script() const;
  std::string message() const;
  void setName(std::string const &name);

private:
  std::string _name;
  directive _script = BlackOS::DisplayKernel::Directives::doNothing;
  std::string _message;
};
} // namespace DisplayKernel
} // namespace BlackOS
#endif // BLACKOS_VFIELD_H
