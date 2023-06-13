/***********************************************************************
* ftermfreebsd-test.cpp - FTermFreeBSD unit tests                      *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2019-2022 Markus Gans                                      *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

#include <term.h>
#undef buttons  // from term.h
#include <unistd.h>

#include <limits>
#include <string>

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>

#include "conemu.h"
#include <final/final.h>

namespace test
{

//----------------------------------------------------------------------
// class FSystemTest
//----------------------------------------------------------------------

class FSystemTest : public finalcut::FSystem
{
  public:
    // Constructor
    FSystemTest() = default;

    // Methods
    auto            inPortByte (uShort) -> uChar override;
    void             outPortByte (uChar, uShort) override;
    auto              isTTY (int) const -> int override;
    auto              ioctl (int, uLong, ...) -> int override;
    auto              open (const char*, int, ...) -> int override;
    auto              close (int) -> int override;
    auto            fopen (const char*, const char*) -> FILE* override;
    auto              fclose (FILE*) -> int override;
    auto              fputs (const char*, FILE*) -> int override;
    auto              putchar (int) -> int override;
    auto            getuid() -> uid_t override;
    auto            geteuid() -> uid_t override;
    auto              getpwuid_r (uid_t, struct passwd*, char*
                                , size_t, struct passwd** ) -> int override;
    auto            realpath (const char*, char*) -> char* override;
    auto     getCharacters() -> std::string&;
    auto             getCursorType() -> int&;
    auto getTerminalKeymap() -> struct keymap_t&;

  private:
    // Data members
    std::string characters{};
    int cursor_type{0};
    static keymap_t keymap;
    static keymap_t terminal_keymap;
};


// private data member of FSystemTest
//----------------------------------------------------------------------
keymap_t FSystemTest::keymap =
{
  109,  // Number of keys
  {
    //                     map                          spcl  flag
    //------------------------------------------------  ----  ----
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0xff, 0x00 },
    { {0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x1b, 0x86, 0x1b}, 0x02, 0x00 },
    { {0x31, 0x21, 0x00, 0x00, 0x31, 0x21, 0x00, 0x00}, 0x33, 0x00 },
    { {0x32, 0x22, 0x00, 0x00, 0xb2, 0xb2, 0x00, 0x00}, 0x33, 0x00 },
    { {0x33, 0xa7, 0x00, 0x00, 0xb3, 0xb3, 0x00, 0x00}, 0x33, 0x00 },
    { {0x34, 0x24, 0x00, 0x00, 0x34, 0x24, 0x00, 0x00}, 0x33, 0x00 },
    { {0x35, 0x25, 0x00, 0x00, 0x35, 0x25, 0x00, 0x00}, 0x33, 0x00 },
    { {0x36, 0x26, 0x00, 0x00, 0x36, 0x26, 0x00, 0x00}, 0x33, 0x00 },
    { {0x37, 0x2f, 0x00, 0x00, 0x7b, 0x7b, 0x00, 0x00}, 0x33, 0x00 },
    { {0x38, 0x28, 0x1b, 0x1b, 0x5b, 0x5b, 0x1b, 0x1b}, 0x00, 0x00 },
    { {0x39, 0x29, 0x1d, 0x1d, 0x5d, 0x5d, 0x1d, 0x1d}, 0x00, 0x00 },
    { {0x30, 0x3d, 0x00, 0x00, 0x7d, 0x7d, 0x00, 0x00}, 0x33, 0x00 },
    { {0xdf, 0x3f, 0x1c, 0x1c, 0x5c, 0x5c, 0x1c, 0x1c}, 0x00, 0x00 },
    { {0x27, 0x60, 0x00, 0x00, 0xb3, 0xb4, 0x00, 0x00}, 0x33, 0x00 },
    { {0x08, 0x08, 0x7f, 0x7f, 0x08, 0x08, 0x7f, 0x7f}, 0x00, 0x00 },
    { {0x09, 0x08, 0x00, 0x00, 0x09, 0x08, 0x00, 0x00}, 0x77, 0x00 },
    { {0x71, 0x51, 0x11, 0x11, 0x40, 0x40, 0x00, 0x00}, 0x00, 0x01 },
    { {0x77, 0x57, 0x17, 0x17, 0x77, 0x57, 0x17, 0x17}, 0x00, 0x01 },
    { {0x65, 0x45, 0x05, 0x05, 0x20ac, 0x45, 0x05, 0x05}, 0x00, 0x01 },
    { {0x72, 0x52, 0x12, 0x12, 0x72, 0x52, 0x12, 0x12}, 0x00, 0x01 },
    { {0x74, 0x54, 0x14, 0x14, 0x74, 0x54, 0x14, 0x14}, 0x00, 0x01 },
    { {0x7a, 0x5a, 0x1a, 0x1a, 0x7a, 0x5a, 0x1a, 0x1a}, 0x00, 0x01 },
    { {0x75, 0x55, 0x15, 0x15, 0x75, 0x55, 0x15, 0x15}, 0x00, 0x01 },
    { {0x69, 0x49, 0x09, 0x09, 0x69, 0x49, 0x09, 0x09}, 0x00, 0x01 },
    { {0x6f, 0x4f, 0x0f, 0x0f, 0x6f, 0x4f, 0x0f, 0x0f}, 0x00, 0x01 },
    { {0x70, 0x50, 0x10, 0x10, 0x70, 0x50, 0x10, 0x10}, 0x00, 0x01 },
    { {0xfc, 0xdc, 0x00, 0x00, 0xfc, 0xdc, 0x1b, 0x00}, 0x31, 0x01 },
    { {0x2b, 0x2a, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00}, 0x33, 0x00 },
    { {0x0d, 0x0d, 0x0a, 0x0a, 0x0d, 0x0d, 0x0a, 0x0a}, 0x00, 0x00 },
    { {0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09}, 0xff, 0x00 },
    { {0x61, 0x41, 0x01, 0x01, 0x61, 0x41, 0x01, 0x01}, 0x00, 0x01 },
    { {0x73, 0x53, 0x13, 0x13, 0x73, 0x53, 0x13, 0x13}, 0x00, 0x01 },
    { {0x64, 0x44, 0x04, 0x04, 0x64, 0x44, 0x04, 0x04}, 0x00, 0x01 },
    { {0x66, 0x46, 0x06, 0x06, 0x66, 0x46, 0x06, 0x06}, 0x00, 0x01 },
    { {0x67, 0x47, 0x07, 0x07, 0x67, 0x47, 0x07, 0x07}, 0x00, 0x01 },
    { {0x68, 0x48, 0x08, 0x08, 0x68, 0x48, 0x08, 0x08}, 0x00, 0x01 },
    { {0x6a, 0x4a, 0x0a, 0x0a, 0x6a, 0x4a, 0x0a, 0x0a}, 0x00, 0x01 },
    { {0x6b, 0x4b, 0x0b, 0x0b, 0x6b, 0x4b, 0x0b, 0x0b}, 0x00, 0x01 },
    { {0x6c, 0x4c, 0x0c, 0x0c, 0x6c, 0x4c, 0x0c, 0x0c}, 0x00, 0x01 },
    { {0xf6, 0xd6, 0x00, 0x00, 0xf6, 0xd6, 0x00, 0x00}, 0x33, 0x01 },
    { {0xe4, 0xc4, 0x00, 0x00, 0xe4, 0xc4, 0x00, 0x00}, 0x33, 0x01 },
    { {0x5e, 0xb0, 0x1e, 0x1e, 0x5e, 0xb0, 0x1e, 0x1e}, 0x00, 0x00 },
    { {0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02}, 0xff, 0x00 },
    { {0x23, 0x27, 0x00, 0x00, 0x23, 0x27, 0x00, 0x00}, 0x33, 0x00 },
    { {0x79, 0x59, 0x19, 0x19, 0x79, 0x59, 0x19, 0x19}, 0x00, 0x01 },
    { {0x78, 0x58, 0x18, 0x18, 0x78, 0x58, 0x18, 0x18}, 0x00, 0x01 },
    { {0x63, 0x43, 0x03, 0x03, 0xa2, 0x43, 0x03, 0x03}, 0x00, 0x01 },
    { {0x76, 0x56, 0x16, 0x16, 0x76, 0x56, 0x16, 0x16}, 0x00, 0x01 },
    { {0x62, 0x42, 0x02, 0x02, 0x62, 0x42, 0x02, 0x02}, 0x00, 0x01 },
    { {0x6e, 0x4e, 0x0e, 0x0e, 0x6e, 0x4e, 0x0e, 0x0e}, 0x00, 0x01 },
    { {0x6d, 0x4d, 0x0d, 0x0d, 0xb5, 0xb5, 0x0d, 0x0d}, 0x00, 0x01 },
    { {0x2c, 0x3b, 0x00, 0x00, 0x2c, 0x3b, 0x00, 0x00}, 0x33, 0x00 },
    { {0x2e, 0x3a, 0x00, 0x00, 0x2e, 0x3a, 0x00, 0x00}, 0x33, 0x00 },
    { {0x2d, 0x5f, 0x1f, 0x1f, 0x2d, 0x5f, 0x1f, 0x1f}, 0x00, 0x00 },
    { {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, 0xff, 0x00 },
    { {0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a}, 0x00, 0x00 },
    { {0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07}, 0xff, 0x00 },
    { {0x20, 0x20, 0x00, 0x20, 0x20, 0x20, 0x87, 0x20}, 0x02, 0x00 },
    { {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}, 0xff, 0x00 },
    { {0x1b, 0x27, 0x33, 0x3f, 0x0b, 0x15, 0x0b, 0x15}, 0xff, 0x00 },
    { {0x1c, 0x28, 0x34, 0x40, 0x0c, 0x16, 0x0c, 0x16}, 0xff, 0x00 },
    { {0x1d, 0x29, 0x35, 0x41, 0x0d, 0x17, 0x0d, 0x17}, 0xff, 0x00 },
    { {0x1e, 0x2a, 0x36, 0x42, 0x0e, 0x18, 0x0e, 0x18}, 0xff, 0x00 },
    { {0x1f, 0x2b, 0x37, 0x43, 0x0f, 0x19, 0x0f, 0x19}, 0xff, 0x00 },
    { {0x20, 0x2c, 0x38, 0x44, 0x10, 0x1a, 0x10, 0x1a}, 0xff, 0x00 },
    { {0x21, 0x2d, 0x39, 0x45, 0x11, 0x11, 0x11, 0x11}, 0xff, 0x00 },
    { {0x22, 0x2e, 0x3a, 0x46, 0x12, 0x12, 0x12, 0x12}, 0xff, 0x00 },
    { {0x23, 0x2f, 0x3b, 0x47, 0x13, 0x13, 0x13, 0x13}, 0xff, 0x00 },
    { {0x24, 0x30, 0x3c, 0x48, 0x14, 0x14, 0x14, 0x14}, 0xff, 0x00 },
    { {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05}, 0xff, 0x00 },
    { {0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06}, 0xff, 0x00 },
    { {0x4b, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37}, 0x80, 0x02 },
    { {0x4c, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38}, 0x80, 0x02 },
    { {0x4d, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39, 0x39}, 0x80, 0x02 },
    { {0x4e, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d}, 0x80, 0x02 },
    { {0x4f, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34}, 0x80, 0x02 },
    { {0x50, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35, 0x35}, 0x80, 0x02 },
    { {0x51, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36}, 0x80, 0x02 },
    { {0x52, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b, 0x2b}, 0x80, 0x02 },
    { {0x53, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31, 0x31}, 0x80, 0x02 },
    { {0x54, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32}, 0x80, 0x02 },
    { {0x55, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33}, 0x80, 0x02 },
    { {0x56, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30}, 0x80, 0x02 },
    { {0x7f, 0x2e, 0x2e, 0x2e, 0x2e, 0x2e, 0x85, 0x85}, 0x03, 0x02 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0xff, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0xff, 0x00 },
    { {0x3c, 0x3e, 0x00, 0x00, 0x7c, 0xa6, 0x00, 0x00}, 0x33, 0x00 },
    { {0x25, 0x31, 0x3d, 0x49, 0x15, 0x15, 0x15, 0x15}, 0xff, 0x00 },
    { {0x26, 0x32, 0x3e, 0x4a, 0x16, 0x16, 0x16, 0x16}, 0xff, 0x00 },
    { {0x0d, 0x0d, 0x0a, 0x0a, 0x0d, 0x0d, 0x0a, 0x0a}, 0x00, 0x00 },
    { {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80}, 0xff, 0x00 },
    { {0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f, 0x2f}, 0x00, 0x02 },
    { {0x0a, 0x99, 0x86, 0x86, 0x00, 0x00, 0x00, 0x00}, 0xff, 0x00 },
    { {0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81}, 0xff, 0x00 },
    { {0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b, 0x4b}, 0xff, 0x00 },
    { {0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c, 0x4c}, 0xff, 0x00 },
    { {0x4d, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d}, 0xff, 0x00 },
    { {0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f}, 0xff, 0x00 },
    { {0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51}, 0xff, 0x00 },
    { {0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53}, 0xff, 0x00 },
    { {0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54, 0x54}, 0xff, 0x00 },
    { {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55}, 0xff, 0x00 },
    { {0x56, 0xa3, 0x56, 0x56, 0x56, 0x56, 0x56, 0x56}, 0xff, 0x00 },
    { {0x57, 0x57, 0x57, 0x57, 0x57, 0x57, 0x85, 0x57}, 0xff, 0x00 },
    { {0x06, 0x88, 0x06, 0x88, 0x87, 0x00, 0x87, 0x00}, 0xff, 0x00 },
    { {0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58, 0x58}, 0xff, 0x00 },
    { {0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59, 0x59}, 0xff, 0x00 },
    { {0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a, 0x5a}, 0xff, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0xff, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 },
    { {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, 0x00, 0x00 }
    //  |     |     |     |     |     |     |     |
    //  |     |     |     |     |     |     |     `--- 0: Shift-Ctrl-Alt
    //  |     |     |     |     |     |     `--------- 1: Ctrl-Alt
    //  |     |     |     |     |     `--------------- 2: Shift-Alt
    //  |     |     |     |     `--------------------- 3: Alt
    //  |     |     |     `--------------------------- 4: Shift-Ctrl
    //  |     |     `--------------------------------- 5: Ctrl
    //  |     `--------------------------------------- 6: Shift
    //  `--------------------------------------------- 7: Base
    //                                                 |
    //                                                 `- spcl bit
    //
    // spcl = A special treatment key (bits correspond to the map field)
    // flag = 0 -> 'O' = Caps lock + num lock are ignored
    // flag = 1 -> 'C' = Caps lock affects the key
    // flag = 2 -> 'N' = Num lock affects the key
  }
};

// static class attributes
//----------------------------------------------------------------------
keymap_t FSystemTest::terminal_keymap{};


// public methods of FSystemTest
//----------------------------------------------------------------------
auto FSystemTest::inPortByte (uShort) -> uChar
{
  return 0;
}

//----------------------------------------------------------------------
void FSystemTest::outPortByte (uChar, uShort)
{
}

//----------------------------------------------------------------------
auto FSystemTest::isTTY (int fd) const -> int
{
  std::cerr << "Call: isatty (fd=" << fd << ")\n";
  return 1;
}

//----------------------------------------------------------------------
auto FSystemTest::ioctl (int fd, uLong request, ...) -> int
{
  va_list args{};
  void* argp{};
  std::string req_string{};
  int ret_val{-1};

  va_start (args, request);
  argp = va_arg (args, void*);

  switch ( request )
  {
    case CONS_CURSORTYPE:
    {
      req_string = "CONS_CURSORTYPE";
      constexpr int blink_cursor  = int(1 << 0);
      constexpr int char_cursor   = int(1 << 1);
      constexpr int hidden_cursor = int(1 << 2);
      constexpr int reset_cursor  = int(1 << 30);
      constexpr int cursor_attrs  = int( blink_cursor \
                                       | char_cursor \
                                       | hidden_cursor );
      int* cur_flags = static_cast<int*>(argp);
      *cur_flags = *cur_flags & cursor_attrs;

      if ( *cur_flags & reset_cursor )
        cursor_type = 0;
      else
        cursor_type = *cur_flags;

      ret_val = 0;
      break;
    }

    case GIO_KEYMAP:
    {
      req_string = "GIO_KEYMAP";
      auto kmap = static_cast<keymap_t*>(argp);

      // Sets the default keymap of the terminal on the first call
      if ( terminal_keymap.n_keys == 0 )
      {
        terminal_keymap.n_keys = keymap.n_keys;
        std::memcpy (terminal_keymap.key, &keymap.key, sizeof(keymap.key));
      }

      kmap->n_keys = terminal_keymap.n_keys;
      std::memcpy (kmap->key, terminal_keymap.key, sizeof(keymap.key));
      ret_val = 0;
      break;
    }

    case PIO_KEYMAP:
    {
      req_string = "PIO_KEYMAP";
      auto kmap = static_cast<keymap_t*>(argp);
      std::memcpy (terminal_keymap.key, kmap->key, sizeof(keymap.key));
      ret_val = 0;
      break;
    }

    case TIOCGWINSZ:
      req_string = "TIOCGWINSZ";
      auto win_size = static_cast<winsize*>(argp);
      win_size->ws_col = 80;
      win_size->ws_row = 25;
      ret_val = 0;
      break;
  }

  va_end (args);

  std::cerr << "Call: ioctl (fd=" << fd
            << ", request=" << req_string
            << "(0x" << std::hex << request << ")"
            << ", argp=" << argp << std::dec << ")\n";
  return ret_val;
}

//----------------------------------------------------------------------
auto FSystemTest::open (const char* pathname, int flags, ...) -> int
{
  va_list args{};
  va_start (args, flags);
  auto mode = static_cast<mode_t>(va_arg (args, int));
  va_end (args);

  std::cerr << "Call: open (pathname=\"" << pathname
            << "\", flags=" << flags
            << ", mode=" << mode << ")\n";

  return 0;
}

//----------------------------------------------------------------------
auto FSystemTest::close (int fildes) -> int
{
  std::cerr << "Call: close (fildes=" << fildes << ")\n";
  return 0;
}

//----------------------------------------------------------------------
auto FSystemTest::fopen (const char* path, const char* mode) -> FILE*
{
  std::cerr << "Call: fopen (path=" << path
            << ", mode=" << mode << ")\n";
  return nullptr;
}

//----------------------------------------------------------------------
auto FSystemTest::fclose (FILE* fp) -> int
{
  std::cerr << "Call: fclose (fp=" << fp << ")\n";
  return 0;
}

//----------------------------------------------------------------------
auto FSystemTest::fputs (const char* str, FILE* stream) -> int
{
  std::cerr << "Call: fputs (" << str << ", " << stream << ")\n";
  std::string string = str;
  int count = 0;

  for (auto&& ch : string)
  {
    characters.push_back(ch);
    count++;
  }

  return count;
}

//----------------------------------------------------------------------
auto FSystemTest::putchar (int c) -> int
{
  std::cerr << "Call: putchar (" << c << ")\n";
  characters.push_back(c);
  return 1;
}

//----------------------------------------------------------------------
auto FSystemTest::getuid() -> uid_t
{
  return 0;
}

//----------------------------------------------------------------------
auto FSystemTest::geteuid() -> uid_t
{
  return 0;
}

//----------------------------------------------------------------------
auto FSystemTest::getpwuid_r ( uid_t, struct passwd*, char*
                            , size_t, struct passwd** ) -> int
{
  return 0;
}

//----------------------------------------------------------------------
auto FSystemTest::realpath (const char*, char*) -> char*
{
  return const_cast<char*>("");
}

//----------------------------------------------------------------------
auto FSystemTest::getCharacters() -> std::string&
{
  return characters;
}

//----------------------------------------------------------------------
auto FSystemTest::getCursorType() -> int&
{
  return cursor_type;
}

//----------------------------------------------------------------------
auto FSystemTest::getTerminalKeymap() -> struct keymap_t&
{
  return terminal_keymap;
}

}  // namespace test


//----------------------------------------------------------------------
// class ftermfreebsdTest
//----------------------------------------------------------------------

class ftermfreebsdTest : public CPPUNIT_NS::TestFixture
                       , test::ConEmu
{
  public:
    ftermfreebsdTest() = default;

  protected:
    void classNameTest();
    void freebsdConsoleTest();

  private:
    // Adds code needed to register the test suite
    CPPUNIT_TEST_SUITE (ftermfreebsdTest);

    // Add a methods to the test suite
    CPPUNIT_TEST (classNameTest);
    CPPUNIT_TEST (freebsdConsoleTest);

    // End of test suite definition
    CPPUNIT_TEST_SUITE_END();
    auto charEncode (finalcut::UniChar) -> wchar_t;
    auto charEncode (wchar_t) -> wchar_t;
};

//----------------------------------------------------------------------
void ftermfreebsdTest::classNameTest()
{
  const auto& freebsd = finalcut::FTermFreeBSD::getInstance();
  const finalcut::FString& classname = freebsd.getClassName();
  CPPUNIT_ASSERT ( classname == "FTermFreeBSD" );
}

//----------------------------------------------------------------------
void ftermfreebsdTest::freebsdConsoleTest()
{
  setenv ("TERM", "xterm", 1);
  setenv ("COLUMNS", "80", 1);
  setenv ("LINES", "25", 1);

  std::unique_ptr<finalcut::FSystem> fsys = std::make_unique<test::FSystemTest>();
  finalcut::FTerm::setFSystem(fsys);
  std::cout << "\n";
  auto& data = finalcut::FTermData::getInstance();

  auto& encoding_list = data.getEncodingList();
  encoding_list["UTF-8"] = finalcut::Encoding::UTF8;
  encoding_list["UTF8"]  = finalcut::Encoding::UTF8;
  encoding_list["VT100"] = finalcut::Encoding::VT100;
  encoding_list["PC"]    = finalcut::Encoding::PC;
  encoding_list["ASCII"] = finalcut::Encoding::ASCII;

  data.setTermEncoding(finalcut::Encoding::VT100);
  data.setBaudrate(9600);
  data.setTermType("xterm");
  data.setTermFileName("/dev/ttyv0");
  data.setTTYFileDescriptor(0);
  data.supportShadowCharacter (false);
  data.supportHalfBlockCharacter (false);
  data.supportCursorOptimisation (true);
  data.setCursorHidden (true);
  data.useAlternateScreen (false);
  data.setASCIIConsole (true);
  data.setVT100Console (false);
  data.setUTF8Console (false);
  data.setUTF8 (false);
  data.setNewFont (false);
  data.setVGAFont (false);
  data.setMonochron (false);
  data.setTermResized (false);

  // setupterm is needed for tputs in ncurses >= 6.1
  setupterm (static_cast<char*>(nullptr), 1, static_cast<int*>(nullptr));

  auto& term_detection = finalcut::FTermDetection::getInstance();
  term_detection.setTerminalDetection(true);
  finalcut::FTermcap::init();

  pid_t pid = forkConEmu();

  if ( isConEmuChildProcess(pid) )
  {
    // (gdb) set follow-fork-mode child
    static constexpr int left_alt = 0x38;
    finalcut::FTermFreeBSD freebsd;
    const auto& fsystem = finalcut::FSystem::getInstance();
    auto fsystest = static_cast<test::FSystemTest*>(fsystem.get());
    struct keymap_t& keymap = fsystest->getTerminalKeymap();

    setenv ("TERM", "xterm", 1);
    setenv ("COLUMNS", "80", 1);
    setenv ("LINES", "25", 1);
    unsetenv("TERMCAP");
    unsetenv("COLORTERM");
    unsetenv("COLORFGBG");
    unsetenv("VTE_VERSION");
    unsetenv("XTERM_VERSION");
    unsetenv("ROXTERM_ID");
    unsetenv("KONSOLE_DBUS_SESSION");
    unsetenv("KONSOLE_DCOP");
    unsetenv("TMUX");

    finalcut::FTerm::detectTermSize();
    CPPUNIT_ASSERT ( keymap.key[left_alt].map[0] == 0 );
    CPPUNIT_ASSERT ( freebsd.isFreeBSDConsole() );
    CPPUNIT_ASSERT ( keymap.key[left_alt].map[0] == 7 );
    CPPUNIT_ASSERT ( freebsd.getCursorStyle()
                     == finalcut::FreeBSDConsoleCursorStyle::Normal );
    freebsd.disableMetaSendsEscape();
    freebsd.disableChangeCursorStyle();
    freebsd.init();
    CPPUNIT_ASSERT ( keymap.key[left_alt].map[0] == 7 );
    CPPUNIT_ASSERT ( freebsd.getCursorStyle()
                     == finalcut::FreeBSDConsoleCursorStyle::Normal );
    freebsd.enableMetaSendsEscape();
    freebsd.enableChangeCursorStyle();
    freebsd.init();
    CPPUNIT_ASSERT ( keymap.key[left_alt].map[0] == META );
    CPPUNIT_ASSERT ( freebsd.getCursorStyle()
                     == finalcut::FreeBSDConsoleCursorStyle::Destructive );
    freebsd.setCursorStyle(finalcut::FreeBSDConsoleCursorStyle::Blink);
    freebsd.setCursorStyle(freebsd.getCursorStyle());
    CPPUNIT_ASSERT ( freebsd.getCursorStyle()
                     == finalcut::FreeBSDConsoleCursorStyle::Blink );

    const auto c1 = finalcut::UniChar::Section;                      // §
    const auto c2 = finalcut::UniChar::InverseBullet;                // ◘
    const auto c3 = finalcut::UniChar::InverseWhiteCircle;           // ◙
    const auto c4 = finalcut::UniChar::DoubleExclamationMark;        // ‼
    const auto c5 = finalcut::UniChar::UpDownArrow;                  // ↕
    const auto c6 = finalcut::UniChar::BlackRectangle;               // ▬
    const auto c7 = finalcut::UniChar::UpwardsArrow;                 // ↑
    const auto c8 = finalcut::UniChar::DownwardsArrow;               // ↓
    const auto c9 = finalcut::UniChar::RightwardsArrow;              // →
    const auto c10 = finalcut::UniChar::LeftwardsArrow;              // ←
    const auto c11 = finalcut::UniChar::Bullet;                      // •
    const auto c12 = finalcut::UniChar::BlackCircle;                 // ●
    const auto c13 = finalcut::UniChar::BlackDiamondSuit;            // ◆
    const auto c14 = finalcut::UniChar::BlackRightPointingTriangle;  // ▶
    const auto c15 = finalcut::UniChar::BlackLeftPointingTriangle;   // ◀
    const auto c16 = finalcut::UniChar::BlackRightPointingPointer;   // ►
    const auto c17 = finalcut::UniChar::BlackLeftPointingPointer;    // ◄
    CPPUNIT_ASSERT ( charEncode(c1) == 21 );   // §
    CPPUNIT_ASSERT ( charEncode(c2) == 8 );    // ◘
    CPPUNIT_ASSERT ( charEncode(c3) == 10 );   // ◙
    CPPUNIT_ASSERT ( charEncode(c4) == 19 );   // ‼
    CPPUNIT_ASSERT ( charEncode(c5) == 18 );   // ↕
    CPPUNIT_ASSERT ( charEncode(c6) == 22 );   // ▬
    CPPUNIT_ASSERT ( charEncode(c7) == 24 );   // ↑
    CPPUNIT_ASSERT ( charEncode(c8) == 25 );   // ↓
    CPPUNIT_ASSERT ( charEncode(c9) == 26 );   // →
    CPPUNIT_ASSERT ( charEncode(c10) == 27 );  // ←
    CPPUNIT_ASSERT ( charEncode(c11) == 4 );   // •
    CPPUNIT_ASSERT ( charEncode(c12) == 4 );   // ●
    CPPUNIT_ASSERT ( charEncode(c13) == 4 );   // ◆
    CPPUNIT_ASSERT ( charEncode(c14) == 16 );  // ▶
    CPPUNIT_ASSERT ( charEncode(c15) == 17 );  // ◀
    CPPUNIT_ASSERT ( charEncode(c16) == 16 );  // ►
    CPPUNIT_ASSERT ( charEncode(c17) == 17 );  // ◄

    freebsd.initCharMap();

    CPPUNIT_ASSERT ( charEncode(c1) == 36 );   // $
    CPPUNIT_ASSERT ( charEncode(c2) == 42 );   // *
    CPPUNIT_ASSERT ( charEncode(c3) == 42 );   // *
    CPPUNIT_ASSERT ( charEncode(c4) == 33 );   // !
    CPPUNIT_ASSERT ( charEncode(c5) == 73 );   // I
    CPPUNIT_ASSERT ( charEncode(c6) == 95 );   // _
    CPPUNIT_ASSERT ( charEncode(c7) == 94 );   // ^
    CPPUNIT_ASSERT ( charEncode(c8) == 118 );  // v
    CPPUNIT_ASSERT ( charEncode(c9) == 62 );   // >
    CPPUNIT_ASSERT ( charEncode(c10) == 60 );  // <
    CPPUNIT_ASSERT ( charEncode(c11) == 42 );  // *
    CPPUNIT_ASSERT ( charEncode(c12) == 42 );  // *
    CPPUNIT_ASSERT ( charEncode(c13) == 42 );  // *
    CPPUNIT_ASSERT ( charEncode(c14) == 62 );  // >
    CPPUNIT_ASSERT ( charEncode(c15) == 60 );  // <
    CPPUNIT_ASSERT ( charEncode(c16) == 62 );  // >
    CPPUNIT_ASSERT ( charEncode(c17) == 60 );  // <

    term_detection.detect();

#if DEBUG
    const finalcut::FString& sec_da = \
        finalcut::FTermDebugData::getInstance().getSecDAString();
    CPPUNIT_ASSERT ( sec_da == "\033[>0;10;0c" );
#endif

    CPPUNIT_ASSERT ( isatty(0) == 1 );
    CPPUNIT_ASSERT ( data.isTermType(finalcut::FTermType::freebsd_con) );
    CPPUNIT_ASSERT ( data.getTerminalGeometry().getWidth() == 80 );
    CPPUNIT_ASSERT ( data.getTerminalGeometry().getHeight() == 25 );
    CPPUNIT_ASSERT ( ! data.hasShadowCharacter() );
    CPPUNIT_ASSERT ( ! data.hasHalfBlockCharacter() );

    data.setCursorHidden (false);
    freebsd.setCursorStyle (finalcut::FreeBSDConsoleCursorStyle::Normal);

    CPPUNIT_ASSERT ( fsystest->getCursorType()
                     == int(finalcut::FreeBSDConsoleCursorStyle::Normal) );

    freebsd.setCursorStyle (finalcut::FreeBSDConsoleCursorStyle::Blink);
    CPPUNIT_ASSERT ( fsystest->getCursorType()
                     == int(finalcut::FreeBSDConsoleCursorStyle::Blink) );

    freebsd.setCursorStyle (finalcut::FreeBSDConsoleCursorStyle::Destructive);
    CPPUNIT_ASSERT ( fsystest->getCursorType()
                     == int(finalcut::FreeBSDConsoleCursorStyle::Destructive) );

    std::string& characters = fsystest->getCharacters();
    characters.clear();

    freebsd.setBeep (20, 100);     // Hz < 21
    CPPUNIT_ASSERT ( characters.empty() );
    freebsd.setBeep (32767, 100);  // Hz > 32766
    CPPUNIT_ASSERT ( characters.empty() );
    freebsd.setBeep (200, -1);     // ms < 0
    CPPUNIT_ASSERT ( characters.empty() );
    freebsd.setBeep (200, 2000);   // ms > 1999
    CPPUNIT_ASSERT ( characters.empty() );
    freebsd.setBeep (200, 100);    // 200 Hz - 100 ms

    CPPUNIT_ASSERT ( characters == CSI "=5965;10B" );
    characters.clear();

    freebsd.resetBeep();
    CPPUNIT_ASSERT ( characters == CSI "=800;5B" );
    characters.clear();

    freebsd.finish();

    closeConEmuStdStreams();
    exit(EXIT_SUCCESS);
  }
  else  // Parent
  {
    // Start the terminal emulation
    startConEmuTerminal (ConEmu::console::freebsd_con);
    int wstatus;

    if ( waitpid(pid, &wstatus, WUNTRACED) != pid )
      std::cerr << "waitpid error" << std::endl;

    if ( WIFEXITED(wstatus) )
      CPPUNIT_ASSERT ( WEXITSTATUS(wstatus) == 0 );
  }
}

//----------------------------------------------------------------------
auto ftermfreebsdTest::charEncode (finalcut::UniChar c) -> wchar_t
{
  return charEncode(static_cast<wchar_t>(c));
}

//----------------------------------------------------------------------
auto ftermfreebsdTest::charEncode (wchar_t c) -> wchar_t
{
  wchar_t ch_enc{L'\0'};

  for (auto&& entry : finalcut::FCharMap::getInstance().getCharEncodeMap())
  {
    if ( entry.unicode == c )
    {
      ch_enc = entry.pc;
      break;
    }
  }

  return ch_enc;
}

// Put the test suite in the registry
CPPUNIT_TEST_SUITE_REGISTRATION (ftermfreebsdTest);

// The general unit test main part
#include <main-test.inc>
