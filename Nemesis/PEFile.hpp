#pragma once

#include "ProcessMemory.hpp"
#include "PESection.hpp"

#include <vector>
#include <Windows.h>

class Module
{
	ProcessMemory *process_memory_;
	DWORD_PTR base_address_;

public:
	const DWORD kFileAlignmentConstant = 0x200;

	BYTE *dos_stub_;
	DWORD dos_stub_size_;
	
	PIMAGE_DOS_HEADER dos_header_;
	PIMAGE_NT_HEADERS32 nt_header32_;
	PIMAGE_NT_HEADERS64 nt_header64_;

	std::vector<PESection> sections_;

private:
	//
	// Init Functions
	//
	auto ReadHeader() -> BOOL;
	auto SetHeader(PVOID header_memory, DWORD header_size) -> VOID;
	auto SetSections() -> VOID;
	auto SetSectionSize(PESection& section, DWORD_PTR section_pointer) const -> VOID;
	auto ReadSection(PESection& section, DWORD_PTR section_pointer) const -> BOOL;

public:
	Module(ProcessMemory* process_memory);
	Module(ProcessMemory *process_memory, DWORD_PTR base_address);
	~Module();

	//
	// Functions
	//
	auto Initialize() -> BOOL;

	auto SetFileAlignment() const -> VOID;
	auto SetEntryPoint(DWORD_PTR entry_point) const -> VOID;
	auto AlignSectionHeaders() -> VOID;
	auto FixHeader() -> VOID;
	auto RemoveIat() -> VOID;

	static auto GetInstructionByteCount(const BYTE* data, DWORD size) -> DWORD;
	static auto AlignValue(DWORD bad_value, DWORD align_to) -> DWORD;

	//
	// Checks
	//
	auto IsValidPeFile() const -> BOOL;
	auto Is64Bit() const -> BOOL;
	auto Is32Bit() const -> BOOL;

	//
	// Getters
	//
	auto GetImageSize() -> DWORD;
	auto GetHeaderSize() const -> DWORD;
	auto GetSectionCount() const -> WORD;
};

