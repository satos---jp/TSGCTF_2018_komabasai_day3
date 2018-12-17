nasm prob.s -f elf64 -g -o out.o
objcopy out.o --set-section-flags .payload=data,code,alloc
ld out.o -o rop4 --section-start .payload=402000 --section-start .data=800000
mv rop4 `sha1sum rop4 | awk '{print $1}'`-rop4

