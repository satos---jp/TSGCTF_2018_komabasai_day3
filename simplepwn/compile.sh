gcc tes.c -fno-stack-protector -o simplepwn
mv simplepwn `sha1sum simplepwn | awk '{print $1}'`-simplepwn
