// depthmapX - spatial network analysis platform
// Copyright (C) 2017 Christian Sailer

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

#ifndef VERSION_H
#define VERSION_H

// use these to define the depthmap versions
#define DEPTHMAPX_VERSION 0.55
#define DEPTHMAPX_MINOR_VERSION_RAW b   // b Beta version, r is standard release, s is stable release, W - M - L for single OS
#define DEPTHMAP_MODULE_VERSION 10.04

// leave these alone - C Preprocessor magic to get stuff to the right format
#define STRINGIFY(v) #v
#define DEPTHMAPX_MINOR_VERSION_FORMAT(minor) STRINGIFY(minor)
#define DEPTHMAPX_MINOR_VERSION DEPTHMAPX_MINOR_VERSION_FORMAT(DEPTHMAPX_MINOR_VERSION_RAW)
#define TITLE_BASE_FORMAT(version, minor) "depthmapX " STRINGIFY(version) STRINGIFY(minor)
#define TITLE_BASE TITLE_BASE_FORMAT(DEPTHMAPX_VERSION, DEPTHMAPX_MINOR_VERSION_RAW)

#endif // VERSION_H
