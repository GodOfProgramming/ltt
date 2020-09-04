#pragma once

// @device is the network device to bind to (for example eth0)
// @file is the object bpf file to use
// @section is the section within the object file to load
int load_bpf_and_xdp_attach(const char* device, const char* file, const char* section);