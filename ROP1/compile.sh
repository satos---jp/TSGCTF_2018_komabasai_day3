nasm prob.s -f elf64 -g -o out.o
objcopy out.o --set-section-flags .payload=data,code,alloc
ld out.o -o rop1 --section-start .payload=402000 --section-start .bss=800000
mv rop1 `sha1sum rop1 | awk '{print $1}'`-rop1
#objddm a.out

