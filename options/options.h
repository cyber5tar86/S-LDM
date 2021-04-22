/*
*	Options C module
*	
*	C module for easy and efficient command-line options management
*	The code is derived from LaTe, out project for an advanced latency measurement tool: https://github.com/francescoraves483/LaMP_LaTe
*
*	Copyright (C) 2019-2021 Francesco Raviglione (CNIT - Politecnico di Torino), Marco Malinverno (CNIT - Politecnico di Torino)
*	
*	This program is free software; you can redistribute it and/or
*	modify it under the terms of the GNU General Public License
*	as published by the Free Software Foundation; either version 2
*	of the License, or (at your option) any later version.
*	
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	
*	You should have received a copy of the GNU General Public License
*	along with this program; if not, write to the Free Software
*	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef OPTIONSMODULE_H_INCLUDED
#define OPTIONSMODULE_H_INCLUDED

#include <inttypes.h>
#include <stdbool.h>
#include <float.h>

// Insert here the version string
#define VERSION_STR "S-LDM 0.1.0-beta"

// Valid options
// Any new option should be handled in the switch-case inside parse_options() and the corresponding char should be added to VALID_OPTS
// If an option accepts an additional argument, it is followed by ':'
#define VALID_OPTS "hvA:E:F:"

#define INIT_CODE 0xAE

#define INVALID_LONLAT -DBL_MAX

typedef struct options {
	// = INIT_CODE if 'struct options' has been initialized via options_initialize()
	uint8_t init_code;

	double min_lat;
	double min_lon;
	double max_lat;
	double max_lon;

	// Extended area parameters (additive factors to the min/max lat and lon values of the internal area)
	double ext_lat_factor;
	double ext_lon_factor;
} options_t;

void options_initialize(struct options *options);
unsigned int parse_options(int argc, char **argv, struct options *options);
void options_free(struct options *options);

#endif // OPTIONSMODULE_H_INCLUDED