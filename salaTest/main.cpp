// Copyright (C) 2026 Tasos Varoudis
// Copyright (C) 2017 Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.


#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "salalib/salaprogram.h"

// SalaError is not derived from std::exception, so without a translator Catch
// reports it as "Unknown exception" and its message is lost.
CATCH_TRANSLATE_EXCEPTION(const SalaError &ex) { return ex.message; }
