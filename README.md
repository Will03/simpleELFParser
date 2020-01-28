# ELFParser

## requirement
- libjson
    - apt install libjson-c-dev
- capstone
    - [linux or windows](http://s4.ce.sharif.edu/blog/2019/01/17/capstone/)

## install
- git clone {this project}
- make ( build executable file )
- make test ( test the parser's functions )

## how to use
```
Usage: ELFParser [FilePath] [ResultPath] [disassemblePath]

ex: 
./ELFHeaderParser ./test/sample/arm_sample ./arm.json ./arm.asm

./ELFHeaderParser ./test/sample/mips32_sample ./mips.json ./mips.asm
```
## result

### json file
``` json
first sample with non-strip symbol
{
    "name":"sampleA",
    "type":"executable file",
    "machine":"x86-64",
    "bit-format":"64-bit",
    "endian":"LSB",
    "link-type":"dynamic-link",
    "symbol":["putchar@@GLIBC_2.2.5","pthread_create@@GLIBC_2.2.5","stdout@@GLIBC_2.2.5","puts@@GLIBC_2.2.5","inet_ntoa@@GLIBC_2.2.5","fclose@@GLIBC_2.2.5","htons@@GLIBC_2.2.5","printf@@GLIBC_2.2.5","htonl@@GLIBC_2.2.5","memset@@GLIBC_2.2.5","sendto@@GLIBC_2.2.5","close@@GLIBC_2.2.5","inet_addr@@GLIBC_2.2.5","signal@@GLIBC_2.2.5","fprintf@@GLIBC_2.2.5","malloc@@GLIBC_2.2.5","fflush@@GLIBC_2.2.5","recvfrom@@GLIBC_2.2.5","ntohs@@GLIBC_2.2.5","fopen@@GLIBC_2.2.5","perror@@GLIBC_2.2.5","bzero@@GLIBC_2.2.5","atoi@@GLIBC_2.2.5","sprintf@@GLIBC_2.2.5","exit@@GLIBC_2.2.5","fwrite@@GLIBC_2.2.5","ntohl@@GLIBC_2.2.5","sleep@@GLIBC_2.2.5","stderr@@GLIBC_2.2.5","socket@@GLIBC_2.2.5"],
    "dynamic link library":["libpthread.so.0","libc.so.6"]
}

second sample with striped symbol
{
    "name":"sampleB",
    "type":"executable file",
    "machine":"ARM",
    "bit-format":"32-bit",
    "endian":"LSB",
    "link-type":"static-link",
    "symbol":[],
    "dynamic link library":[]
}

third sample with wrong type
{
    "name":"111.txt",
    "type":"Not ELF file"
}
```
### Disassemble file
```
... ...
xor		ebp, ebp
mov		r9, rdx
pop		rsi
mov		rdx, rsp
and		rsp, 0xfffffffffffffff0
push		rax
push		rsp
mov		r8, 0x401830
... ...
```


## limit
- only test on ubuntu os
- only avaliable for parsing arm, mips, x86, x86_64 architecture and LSB bit-set ELF file
