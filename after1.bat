PATH=F:\arm-none-eabi-gcc-4_6\bin;%PATH%
checksum Debug\bin\%1.bin
copy Debug\bin\%1.bin c:\TFTP-Root\p1publisher.bin
