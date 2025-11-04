/*  Havok Format Unit Tests
    Copyright(C) 2020-2023 Lukas Cone

    This program is free software : you can redistribute it and / or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.If not, see <https://www.gnu.org/licenses/>.
*/

#include "test.hpp"

int main() {
  es::print::AddPrinterFunction(es::Print);

  TEST_CASES(int resultVar, TEST_FUNC(test_skeleton),
             TEST_FUNC(test_defaultmotion), TEST_FUNC(test_spline),
             TEST_FUNC(test_interleaved), TEST_FUNC(test_delta),
             TEST_FUNC(test_wavelet), TEST_FUNC(test_sample),
             TEST_FUNC(test_xml_base), TEST_FUNC(test_scene));

  return resultVar;
}
