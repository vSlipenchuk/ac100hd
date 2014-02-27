27feb2014: changed board-panel as in patch board-paz00-panel.c-1280x720
           compile it as v6 kernel



doc from: http://bis.org.pl/en:ac100

$ git clone git://gitorious.org/~marvin24/ac100/marvin24s-kernel.git -b chromeos-ac100-3.0

HEAD of the repository is far less stable (I sometimes have trouble booting it).

The current commit as for this document is: c3f97e1841f5d0ed44255a22b0acb1382a4844b3

$ make ARCH=arm CROSS_COMPILE=cortex_a9_neon-linux-gnueabi- paz00_defconfig

For my own usage I change some of the kernel settings:

    SLUB instead of SLAB allocator
    CONFIG_NEON = y
    CONFIG_R8712U = m (needed for my additional USB Wi-Fi card)
    CONFIG_DEBUG_KERNEL = n
    CONFIG_SECURITY = n

$ make ARCH=arm CROSS_COMPILE=cortex_a9_neon-linux-gnueabi- -j4