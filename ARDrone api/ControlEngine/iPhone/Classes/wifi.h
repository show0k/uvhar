/*
 *  wifi.h
 *  ARDroneEngine
 *
 *  Created by f.dhaeyer on 05/05/10.
 *  Copyright 2010 Parrot SA. All rights reserved.
 *
 */
#define MAXADDRS			32
#define LOCALHOST			0x00000000
#define NETWORK_BASE_ADDR	0xc0a80100	// 192.168.1.0

// Function prototypes
unsigned long deviceIPAddress(const char *itfName, char *hw_address);
void InitAddresses();
void FreeAddresses();
void GetIPAddresses();
void GetHWAddresses();