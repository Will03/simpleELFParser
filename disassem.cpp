# include"disassem.h"

int dissamBinary(const uint8_t* binAddr, int size,cs_arch arch, cs_mode bitFormat )
{
	csh handle;
	cs_insn *insn;
	size_t count;

    // Register a handler for define the arch
	if (cs_open(arch, bitFormat, &handle) != CS_ERR_OK)
		return -1;

    //
	count = cs_disasm(handle,binAddr , size, 0x1000, 0, &insn);
	if (count > 0) {
		size_t j;
		for (j = 0; j < count; j++) {
			printf("0x%"PRIx64":\t%s\t\t%s\n", insn[j].address, insn[j].mnemonic,
					insn[j].op_str);
		}

		cs_free(insn, count);
	} else
		printf("ERROR: Failed to disassemble given code!\n");

	cs_close(&handle);

    return 0;
}