#pragma once


#define TVERSION							((WORD) 0x2044)	// Protocol version

#define SM_BASE								(0x1300)		// System message base for server
#define MW_BASE								(0x2300)		// Map server <-> World server message base
#define DM_BASE								(0x9300)		// DB message base for server
#define CS_LOGIN							(0x4300)		// Login server <-> Client message base
#define CS_MAP								(0x5300)		// Map server <-> Client message base
#define CT_CONTROL							(0x6300)		// Control Server Base
#define CT_PATCH							(0x7000)		// Patch Server <-> Client
#define RW_RELAY							(0x8300)		// Relay Server <-> Client
