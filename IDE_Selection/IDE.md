## All about Embedded IDEs and their features

### ***Fully Abstracted IDE***
#### **STM32CubeIDE**
IDE for STM32 MCUs.

Field | Supported Item
----------------|-------
Host OS | Windows, Linux, macOS
Debugger Support | GDB, STLink, JLink, JTrace and more
IDE Framework | Eclipse
Compiler | GCC, Extensible
Cost | Free
License | Proprietary-freeware

#### **ARM Mbed Studio**
An IoT-focused platform that provides a very large middleware library and a consistent dev environment across many different hardare vendors.

Field | Supported Item
----------------|-------
Host OS | Windows, macOS or online (Mbed online)
Debugger Support | PyOCD for limited graphical debugging or GDB (console only)
IDE Framework | Theia
Compiler | ARM Compiler 6, GCC and IAR
Cost | Free
License | Apache2.0

#### **Arduino IDE**
Arduino IDE uses strictly structured libraries that expose a dislect of C/C++ for users to write sketches. It hides as many details about the underlying hardware as possible inside libraries.

Field | Supported Item
----------------|-------
Host OS | Windows, macOS, Linux or online 
Debugger Support | None
IDE Framework | Proprietary Java, processing
Compiler | ARM Compiler 6, GCC and IAR
Cost | Free
License | GNU

### ***Open source/free IDEs***
#### **AC6 System Workbench for STM32 (S4STM32)**
AC6 is a consulting firm that has contributed an Eclipse-based IDE that targets STM32 MCUs. System Workbench adds some support for STM-based discovery boards to help get project set up quickly. Useful if you are developing an application with one of the multi-core devices (STM32MP1 family).

Field | Supported Item
----------------|-------
Host OS | Windows, Linux, macOS
Debugger Support | GDB
IDE Framework | Eclipse
Compiler | GCC
Cost | Free
License | Proprietary-freeware

#### **Eclipse CDT and GCC**
Eclipse CDT is the de facto standard for C/C++ development. You will need to provide a compiler.

Field | Supported Item
----------------|-------
Host OS | Windows, Linux, macOS
Debugger Support | GDB
IDE Framework | Eclipse
Compiler | GCC
Cost | Free
License | Eclipse Public License (EPL)

#### **Visual Studio Code**
A text editor that provides the ability to add extensions.

Field | Supported Item
----------------|-------
Host OS | Windows, Linux, macOS
Debugger Support | GDB, ST-link, and others
IDE Framework | VScode
Compiler | Many
Cost | Free
License | MIT

### ***Proprietary IDE***
#### **ARM/Keil uVision**
Field | Supported Item
----------------|-------
Host OS | Windows
Debugger Support | Many
IDE Framework | Proprietary
Compiler | armcc, armClang, GCC
Cost | Free-$$$
License | Proprietary

#### **IAR Embedded Workbench**
Field | Supported Item
----------------|-------
Host OS | Windows
Debugger Support | Many
IDE Framework | Proprietary
Compiler | Proprietary
Cost | $$-$$$
License | Proprietary

#### **Rowley CrossWorks**
Slightly lower priced entry point than Keil and IAR. Middleware is licensed sparatly from the IDE. FreeRTOS-aware task-based debugging is not available within the IDE.

Field | Supported Item
----------------|-------
Host OS | Windows, Linux or macOS
Debugger Support | Many
IDE Framework | Proprietary
Compiler | GCC, LLVM
Cost | $-$$$
License | Proprietary

#### **SEGGER Embedded Studio**
Free of charge for non-commercial use. Middleware stack licensed sparately from IDE. FreeRTOS-aware debugging is available in the IDE.

Field | Supported Item
----------------|-------
Host OS | Windows, Linux or macOS
Debugger Support | SEGGER
IDE Framework | Proprietary
Compiler | GCC, LLVM
Cost | $$-$$$, Free for non-commercial use
License | Proprietary

#### **SysProgs Visual GDB**
Is not really an IDE but a plugin for Visual Studio code. Its puprpose is to provide a consistent UI (Visual studio) for interacting with GDB-enabled debuggers and GNU make utilities.

Field | Supported Item
----------------|-------
Host OS | Windows, Linux or macOS
Debugger Support | Yes
IDE Framework | Visual Studio/code
Compiler | GCC, ARM
Multi-core debug | Yes
Cost | $
License | Proprietary

### IDE feature comparison table

IDE | Free? | No code size limit | J-link support | FreeRTOS kernel-aware debug | Multi-platform
----------------|---|---|---|---|----
Keil uVision | V | X | V | X | X
IAR | X | N/A | V | V | X
Visual GDB | X | N/A | V | V | V
Rowley CrossWorks | X | N/A | V | V | V
VS Code | V | V | V | V | V
Eclipse CDT | V | V | V | V | V
AC6 S4STM32 | V | V | V | V | V
Arduino IDE | V | V | V | X | V
ARM MBed Studio | V | V | V | X | V
STM32CubeIDE | V | V | V | V | V
SEGGER Embedded Studio | V | V | V | V | V
   