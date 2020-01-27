
#ifndef stdlib
#define stdlib
#include<stdio.h>
#include <inttypes.h>
#endif

#include <capstone/capstone.h>

// typedef enum ELFtoCapstone {
// 	CS_ARCH_ARM = 0,	// ARM architecture (including Thumb, Thumb-2)
// 	CS_ARCH_ARM64,		// ARM-64, also called AArch64
// 	CS_ARCH_MIPS,		// Mips architecture
// 	CS_ARCH_X86,		// X86 architecture (including x86 & x86-64)
// 	CS_ARCH_PPC,		// PowerPC architecture
// 	CS_ARCH_SPARC,		// Sparc architecture
// 	CS_ARCH_SYSZ,		// SystemZ architecture
// 	CS_ARCH_XCORE,		// XCore architecture
// 	CS_ARCH_MAX,
// 	CS_ARCH_ALL = 0xFFFF, // All architectures - for cs_support()
// } cs_arch;

int disassBinary(const uint8_t* binAddr, int size,cs_arch arch, cs_mode bitFormat,FILE *outputFp );