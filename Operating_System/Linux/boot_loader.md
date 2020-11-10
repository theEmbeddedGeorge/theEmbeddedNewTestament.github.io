## How Boot Loaders Work
### What BIOS does for us
The BIOS is the firmware in the ROM of a PC. When the PC is powered up, the BIOS is the first program that runs. All other programs must be loaded into RAM first. The BIOS contains the following parts:

  * POST (Power On Self Test). The running counter that counts the kilobytes of main memory is the most visible part of the POST.
  * The Setup Menu, that lets you set some parameters and lets you adjust the real time clock. Most modern BIOS versions let you set the boot order, the devices that BIOS checks for booting. These can be A (the first floppy disk), C (the first hard disk), CD-ROM and possibly other disks as well. The first device in the list will be tried first. Older BIOS-es have only one boot order: A, C. So the BIOS will try to boot from A first and if there is no diskette in the drive it tries to boot from C.
  * The boot sector loader. This loads the first 512-byte sector from the boot disk into RAM and jumps to it. This is where the boot loaders described in this paper start.
  * The BIOS interrupts. These are simple device drivers that programs can use to access the screen, the keyboard and disks. Boot loaders rely on them, most operating systems do not (the Linux kernel does not use BIOS interrupts once it has been started). MSDOS does use BIOS interrupts.
  
Apart from the main BIOS there are extension ROMs, which are started by the main BIOS. Every VGA card has one. Also SCSI host adapters and Ethernet cards can have an extension ROM. It is even possible to put an EPROM on an Ethernet card to boot a machine over the network without any disks.

As far as boot loading facilities are concerned, the PC BIOS is very primitive compared to that of other computer systems. The only thing it knows about disks is how to load the first 512-byte sector.

  The first sector of a diskette can be loaded at address 0000:7C00. The last two bytes of the sector are checked for the values 0x55 and 0xAA, this as a rough sanity check. If these are OK, the BIOS jumps to the address 0000:7C00.
  Booting from a hard disk is very similar to booting from a diskette. The first sector of a hard disk (often called the Master Boot Record) is loaded at 0000:7C00 and next the BIOS jumps to it. The MBR program must move itself to an address that is different from 0000:7C00 as it is supposed to load a different boot sector from a partition to address 0000:7C00 and jump to that.
  Modern BIOS versions can treat a certain file on a CD-ROM as a diskette image. They pretend to boot from a diskette by loading the first 512 bytes of the file to 0000:7C00 and jumping to it. Every attempt to access the same diskette using the BIOS routines, will be redirected to the image file on CD-ROM. Some other ways to boot a CD-ROM may also be supported (with an emulated hard disk or with no disk emulation at all).
When the boot sector is loaded, the CPU is in real mode. For those who are unfamiliar with 80x86 architecture: real mode is very limited compared to 32-bit protected mode (in which Linux runs). For example: data outside a 64K segment can only be accessed if you change a segment register and data outside the first 1MB of address space (which contains 640kB of main memory) cannot be accessed at all. As gcc does not know about real mode, programs compiled with it can only be run in real mode with some tricks and with severe memory size restrictions. This is the reason why most boot loaders (except GRUB) are written in assembly. All boot sector programs, even that of GRUB, are written in assembly1.

In theory a boot loader could do its job by directly accessing the bare metal, but 512 bytes would be a very small space for that. The boot loader has access to BIOS interrupts, which are subroutines that can be invoked by the INT instruction (software interrupts). These work in real mode only. The following routines are used by most boot loaders.

  * INT 0x10 for screen output.
  * INT 0x16 for keyboard input.
  * INT 0x13 for disk I/O. The parameters to specify sectors on disk used to have a very limited range. Originally it was only possible to specify 1024 cylinders on a hard disk, while hard disks can have more cylinders. This imposed limitations on where it was allowed to put the boot loader and any files accessed by it. You were forced to create a small partition near the start of the hard disk and put the boot loader there. There are three categories of BIOS:
BIOS versions earlier than 1995 could only access IDE disks of around 500MB, as the BIOS sector and head numbers corresponded directly to the register values on the IDE interface.
BIOS versions between 1995 and 1998 can access IDE disks up to about 8GB. They translate the cylinder, head and sector numbers from the INT 0x13 call to different values that better utilize the allowable ranges on the IDE interface.
BIOS versions of 1998 or later have a new calling interface using linear block addresses.
In any of the three categories you have BIOS-es that have bugs that cause them to stop at a lower disk size limit (a category 2 BIOS that should work up to 8GB, but stops at 2GB). In those cases it makes sense to upgrade to a new BIOS version.
  INT 0x15 is a catch-all for many BIOS functions, one of which is moving data to extended memory (the BIOS is required to switch to protected mode temporarily to do that). Other functions are for disabling the A20 gate and for determining the memory size.
### Parts of a boot loader
A boot loader typically consists of three programs:

  * The boot sector program is directly loaded by the BIOS at boot time and is only 512 bytes in size.
  * The second stage program is loaded by the boot sector program and it does everything you expect the boot loader to do.
  * The boot loader installer is not run when the system is booted, but it is used to install the boot loader and the second stage program onto the boot disk. These have to be stored in special locations, so they cannot be copied with cp.
#### boot sector program
The boot sector program can only be 512 bytes in size and not all 512 bytes are even available in all cases. The last two bytes must be 0x55 and 0xAA for the BIOS. The Master Boot Record on a hard disk contains the partition table of 64 bytes, so only the first 446 bytes can be used. If the boot sector program must exist on a DOS partition or on a DOS diskette, there must be a parameter block at the start of the boot sector.

Because of these size restrictions, boot sector programs are just about the only remaining examples of programs on the PC platform that are truly optimized for size and have to be written in assembly for this reason. Further, a boot sector program cannot do everything you want a boot loader to do. Usually a boot sector program does one of the following things (not all three in one program):

  * Load another boot sector. This is typical for a boot sector program that lives in the master boot record of a hard disk. It can find the first sector of the selected active partition and chain load that. The MBR program that came traditionally with MS-DOS has no ability to change the active partition at boot time. There are other boot sector programs that let you select a partition by pressing a key, such as the MBR program of LILO.
  * Load a second stage boot loader. It is generally not possible for a boot sector program to look into the directory for a file with a specific name and load that into memory, but exceptions exist, at least for DOS file systems2. Most boot sector programs find the second stage by sector number rather than by name. The sector numbers have to be put into the boot sector by the boot loader installer.
  * Load the kernel directly. A kernel is typically much larger than a second stage boot loader.

The boot sector program in the Linux kernel loads the kernel directly into memory without the need for a second stage boot loader. As the kernel is located in contiguous sectors on a diskette, there is no need to traverse file system data structures. However, for bZimage kernels the boot sector program cheats; it invokes a subroutine in the setup part of the kernel for loading the rest of the kernel into high memory.

The boot loader e2boot fits into the first 1kB block of an ext2 partition (it is twice as big as a boot sector program), but with some tricks it finds both the kernel and the RAM disk by name on an ext2 partition and loads them into memory.

Also the boot sector on a DOS disk does not utilize a second stage boot loader to load the MS-DOS kernel files IO.SYS and MSDOS.SYS. The structure of an MSDOS file system is simple enough to find a file with a specific name in the root directory and load it into memory, at least part of it..

#### second stage of boot loader
This is the real boot program. It contains the user interface and the kernel loader. It can be anywhere from 6.5 kilobytes (LILO) to over 100 kilobytes (GRUB) in size. It contains the following functions:

  * User interface. It is either a simple command line (old versions of LILO), a menu or both. It allows you to select any number of operating systems and to specify additional parameters to the operating system. The available options are specified by a configuration file. Modern versions of boot loaders can show their menu in a bitmap picture.
  * Operating system loader. loads the operating system into memory and runs it. Alternatively we can load another boot loader specific to another operating system and let it run. This is called chain loading.

LOADLIN is not a complete boot loader, but it has only the second stage (without the user interface). It is run from DOS, and it can make use of DOS system calls to read files from disk. What makes its task harder than that of a normal boot loader, it that it must be able to work its way out of some types of memory managers.

#### Boot loader installer
The third part of the boot loader is only run when the boot loader is installed on a disk. As opposed to the boot sector program and second stage, this is a normal Linux program. In the case of LILO the installer must be rerun each time the configuration is changed or any file has been updated. It performs the following tasks:

  * Install the boot sector. If the boot sector will be installed in the MBR of a hard disk or on a DOS file system, not all 512 bytes may be overwritten, but the partition table or the DOS parameter block must be preserved.
  * Tell the boot sector where the second stage boot loader is. Usually it writes one or more sector addresses into the boot loader.
  * Tell the second stage boot loader where all relevant information is (configuration, kernels). This is the case with LILO. LILO creates a map file that contains all relevant sector addresses and puts pointers to the map file in the boot sector and/or second stage boot loader.

Loading the operating system
### Loading the Linux kernel
#### A Linux kernel consists of the following parts:

  * The boot sector (see arch/i386/boot/bootsect.S). This is only run if Linux is booted directly from a diskette.
  * The setup part with real mode initialization code (transition to protected mode) (see arch/i386/boot/setup.S)
  * The rest of the kernel, which in turn consists of the following parts:
    * Protected mode initialization code 
    (see arch/i386/boot/compressed/head.S).
    * A decompression program (compiled C code, see 
    arch/i386/boot/compressed/misc.c and lib/inflate.c).

The real kernel, which is compressed. After decompression this kernel consists of the following parts:
  * Protected mode initialization code (see arch/i386/boot/head.S).
  * Main C routine (see init/main.c).
  * All the rest. Relevant for this discussion is the RAM disk driver (see drivers/block/rd.c). This will be further explained in section 4
  
A Linux boot loader should support the following tasks:

  * Loading the Linux kernel into memory.
    * The boot sector (which will not be run) and the setup part of the kernel are loaded near the top of low memory (usually address 9000:0000).
    * If it is not a bzImage type kernel, the rest of the kernel will be loaded in low memory at 0000:1000).
    * If it is a bzImage type kernel, the rest of the kernel will be loaded in high memory, starting at 0x100000.

The boot loader does not know or care that the rest of the kernel contains a compressed part.
  Passing a command line to the kernel. This command line can come from a configuration file or it can be entered interactively by the user.
  Loading an initial RAM disk into memory and passing it to the kernel. The initial RAM disk will be loaded near the top of high memory above the kernel.
  Starting the kernel. The boot loader must set some relevant parameters in the setup part and jump to it. From there the kernel takes control and the boot loader's part is over. Next the kernel starts as follows:
    The setup code saves relevant parameters and the command line for later.
    The setup code checks how much memory is available, performs some hardware initialization and prepares to enter protected mode.
    In protected code, the rest of the kernel is being decompressed3.
    After decompression the head.S and the main C routine will initialize all parts of the kernel. This will show a lot of messages.
    Finally the init process will be started (or first its linuxrc variant on an initial RAM disk).
#### Chain loading
Most boot loaders are designed to boot only one operating system. LILO knows only to load Linux kernels and the DOS boot sector can load only DOS. If you want to select between several different operating systems, it is not likely that you find a boot loader that can load all of them. But at least every operating system is supposed to be booted by a 512-byte boot sector that can be loaded by the BIOS and there lies the key. Any sufficiently advanced boot loader supports chain loading.

If a boot loader loads a boot sector of another operating system, it is called chain loading. This boot sector can directly be loaded from the disk or partition or it can be loaded from a file. For the other operating system it should not make a difference whether its boot sector was loaded by the BIOS or by another boot loader. In reality a boot sector of a partition is normally loaded by a master boot record program. Also in this case, it should make no difference if this is loaded by a more advanced boot loader (such as LILO) instead.

The following chain loading scenarios are possible.

  Linux boot loaders can chain load almost any other operating system.
  Linux boot loaders can be chain loaded by boot managers of other operating systems. It is possible to boot Linux from the Windows NT boot manager or the OS/2 boot manager.
  Linux boot loaders can chain load Linux boot loaders as well. This may make sense on computers with several independent Linux systems installed, where each Linux installation has its own local LILO and these can be selected by a central instance of LILO to chain load them. Instances of LILO can even exchange command lines between them.
#### Configuring the boot loader
Both LILO and GRUB have a configuration file that specifies several menu options, each representing either a Linux kernel or a different operating system to boot. For each Linux kernel a command line and an initial RAM disk can be specified. Apart from syntactic details the contents of these configuration files look remarkably similar. But there is an essential difference:

  LILO reads its configuration file at installation time. Every time the configuration file, the kernel or any initial RAM disk is changed, the LILO installer must be rerun. At boot time the configuration file is not read. The second stage boot program of LILO does not know how to find files in the file system. It relies on a map file to find data blocks of the necessary files. This map file was created by the LILO installer.
  GRUB and also SYSLINUX read their configuration files at boot time. You can install the boot loader once and just change configuration files, kernels and RAM disk images without trouble. The second stage boot program knows how to find files in the file system.
