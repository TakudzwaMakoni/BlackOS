#ifndef TRINKETS_ALPHA_SORT_RADIX_H
#define TRINKETS_ALPHA_SORT_RADIX_H

/**
 * AlphaSortRadix
 *
 * Copyright (C) 2020, Takudzwa Makoni <https://github.com/TakudzwaMakoni>
 *
 * This Program is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This Program is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with This Program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 */

#include <iostream>
#include <string>
#include <vector>

namespace BlackOS {
namespace Trinkets {
/// case insensitive weights
enum weightsCI : char {
  a = 'a',
  A = 'a',
  b = 'b',
  B = 'b',
  c = 'c',
  C = 'c',
  d = 'd',
  D = 'd',
  e = 'e',
  E = 'e',
  f = 'f',
  F = 'f',
  g = 'g',
  G = 'g',
  h = 'h',
  H = 'h',
  i = 'i',
  I = 'i',
  j = 'j',
  J = 'j',
  k = 'k',
  K = 'k',
  l = 'l',
  L = 'l',
  m = 'm',
  M = 'm',
  n = 'n',
  N = 'n',
  o = 'o',
  O = 'o',
  p = 'p',
  P = 'p',
  q = 'q',
  Q = 'q',
  r = 'r',
  R = 'r',
  s = 's',
  S = 's',
  t = 't',
  T = 't',
  u = 'u',
  U = 'u',
  v = 'v',
  V = 'v',
  w = 'w',
  W = 'w',
  x = 'x',
  X = 'x',
  y = 'y',
  z = 'z',
  Z = 'z'
};
int alphaSortRadix(std::vector<std::string> const &items);
} // namespace Trinkets
} // namespace BlackOS
#endif
