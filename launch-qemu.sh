# run qemu, without using gdb
# added by xw, 18/6

# deleted by mingxuan 2019-5-17
# qemu-system-i386 -fda a.img -hda 80m.img -boot order=a -ctrl-grab \
# -monitor stdio

# modified by mingxuan 2019-5-17
qemu-system-i386 -hda b.img -boot order=a -ctrl-grab \
-monitor stdio
