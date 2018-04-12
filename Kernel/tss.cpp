
//****************************************************************************
//**
//**    tss.h
//**
//**	Task State Segment
//**
//****************************************************************************

//============================================================================
//    IMPLEMENTATION HEADERS
//============================================================================

#include <string.h>
#include "memory.h"
#include "gdt.h"
#include "tss.h"

//============================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE DATA
//============================================================================

tss_entry TSS;

//============================================================================
//    INTERFACE DATA
//============================================================================
//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//============================================================================

void install_tsr (uint16_t sel);

//============================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//============================================================================

void flush_tss (uint16_t sel) {

	_asm {
		cli
		mov eax, 0x2b
		ltr ax
		sti
	}

//	_asm ltr [sel]
}

//============================================================================
//    INTERFACE FUNCTIONS
//============================================================================

void tss_set_stack (uint16_t kernelSS, uint16_t kernelESP) {

	TSS.ss0 = kernelSS;
	TSS.esp0 = kernelESP;
}

void install_pagedirectory(void* pPageDirectory)
{
	TSS.ldt = (uint32_t)pPageDirectory;
}

void install_tss(uint32_t idx, uint16_t kernelSS, uint16_t kernelESP) {

	//! install TSS descriptor
	uint32_t base = (uint32_t)&TSS;

	//! install descriptor
	gdt_set_descriptor(idx, base, base + sizeof(tss_entry),
		I86_GDT_DESC_ACCESS | I86_GDT_DESC_EXEC_CODE | I86_GDT_DESC_DPL | I86_GDT_DESC_MEMORY,
		0);

	//! initialize TSS
	memset((void*)&TSS, 0, sizeof(tss_entry));

	//! set stack and segments
	TSS.ss0 = kernelSS;
	TSS.esp0 = kernelESP;
	TSS.cs = 0x0b;
	TSS.ss = 0x13;
	TSS.es = 0x13;
	TSS.ds = 0x13;
	TSS.fs = 0x13;
	TSS.gs = 0x13;

	//! flush tss
	flush_tss((uint16_t)idx * sizeof(gdt_descriptor));
}

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
//****************************************************************************
//**
//**    END[tss.cpp]
//**
//****************************************************************************
