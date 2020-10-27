# How ARM64 Kernel Excpetion Mode Works
### Exception Vetor Table of ARM aarch32 
![Exception_Vectors.jpg](http://kcchao.wdfiles.com/local--files/arm/Exception_Vectors.jpg)


### Exception Vetor Table of ARM aarch64
![Exception_Handling_Guide_6.jpg?revision=6b7bc331-414b-40d4-b2c4-bb73cb2a48cf&hash=48187D388A2C7794506C4012C5FF73D089A8C331&hash=48187D388A2C7794506C4012C5FF73D089A8C331&la=en](https://developer.arm.com/-/media/Arm%20Developer%20Community/Images/Tutorial%20Guide%20Diagrams%20and%20Screenshots/Learn%20the%20Architecture/Exception%20model/Exception_Handling_Guide_6.jpg?revision=6b7bc331-414b-40d4-b2c4-bb73cb2a48cf&hash=48187D388A2C7794506C4012C5FF73D089A8C331&hash=48187D388A2C7794506C4012C5FF73D089A8C331&la=en)


### The following source codes are from Linux ARM64 3.18.102.

##### When Hardware generates an exception, kernel tries to find the entry on the vector table.

```C
ENTRY(vectors)
	ventry	el1_sync_invalid		// Synchronous EL1t
	ventry	el1_irq_invalid			// IRQ EL1t
	ventry	el1_fiq_invalid			// FIQ EL1t
	ventry	el1_error_invalid		// Error EL1t

	ventry	el1_sync			// Synchronous EL1h
	ventry	el1_irq				// IRQ EL1h
	ventry	el1_fiq_invalid			// FIQ EL1h
	ventry	el1_error_invalid		// Error EL1h

	ventry	el0_sync			// Synchronous 64-bit EL0
	ventry	el0_irq				// IRQ 64-bit EL0
	ventry	el0_fiq_invalid			// FIQ 64-bit EL0
	ventry	el0_error_invalid		// Error 64-bit EL0

#ifdef CONFIG_COMPAT
	ventry	el0_sync_compat			// Synchronous 32-bit EL0
	ventry	el0_irq_compat			// IRQ 32-bit EL0
	ventry	el0_fiq_invalid_compat		// FIQ 32-bit EL0
	ventry	el0_error_invalid_compat	// Error 32-bit EL0
#else
	ventry	el0_sync_invalid		// Synchronous 32-bit EL0
	ventry	el0_irq_invalid			// IRQ 32-bit EL0
	ventry	el0_fiq_invalid			// FIQ 32-bit EL0
	ventry	el0_error_invalid		// Error 32-bit EL0
#endif
END(vectors)
```

##### For example, when the el0 synchrous abort happens, kernel searches el0_syn entry and find matched handler.

```C
/*
 * EL0 mode handlers.
 */
	.align	6
el0_sync:
	kernel_entry 0
	mrs	x25, esr_el1			// read the syndrome register
	lsr	x24, x25, #ESR_EL1_EC_SHIFT	// exception class
	cmp	x24, #ESR_EL1_EC_SVC64		// SVC in 64-bit state
	b.eq	el0_svc
	cmp	x24, #ESR_EL1_EC_DABT_EL0	// data abort in EL0
	b.eq	el0_da
	cmp	x24, #ESR_EL1_EC_IABT_EL0	// instruction abort in EL0
	b.eq	el0_ia
	cmp	x24, #ESR_EL1_EC_FP_ASIMD	// FP/ASIMD access
	b.eq	el0_fpsimd_acc
	cmp	x24, #ESR_EL1_EC_FP_EXC64	// FP/ASIMD exception
	b.eq	el0_fpsimd_exc
	cmp	x24, #ESR_EL1_EC_SYS64		// configurable trap
	b.eq	el0_undef
	cmp	x24, #ESR_EL1_EC_SP_ALIGN	// stack alignment exception
	b.eq	el0_sp_pc
	cmp	x24, #ESR_EL1_EC_PC_ALIGN	// pc alignment exception
	b.eq	el0_sp_pc
	cmp	x24, #ESR_EL1_EC_UNKNOWN	// unknown exception in EL0
	b.eq	el0_undef
	cmp	x24, #ESR_EL1_EC_BREAKPT_EL0	// debug exception in EL0
	b.ge	el0_dbg
	b	el0_inv

#ifdef CONFIG_COMPAT
	.align	6
el0_sync_compat:
	kernel_entry 0, 32
	mrs	x25, esr_el1			// read the syndrome register
	lsr	x24, x25, #ESR_EL1_EC_SHIFT	// exception class
	cmp	x24, #ESR_EL1_EC_SVC32		// SVC in 32-bit state
	b.eq	el0_svc_compat
	cmp	x24, #ESR_EL1_EC_DABT_EL0	// data abort in EL0
	b.eq	el0_da
	cmp	x24, #ESR_EL1_EC_IABT_EL0	// instruction abort in EL0
	b.eq	el0_ia
	cmp	x24, #ESR_EL1_EC_FP_ASIMD	// FP/ASIMD access
	b.eq	el0_fpsimd_acc
	cmp	x24, #ESR_EL1_EC_FP_EXC32	// FP/ASIMD exception
	b.eq	el0_fpsimd_exc
	cmp	x24, #ESR_EL1_EC_UNKNOWN	// unknown exception in EL0
	b.eq	el0_undef
	cmp	x24, #ESR_EL1_EC_CP15_32	// CP15 MRC/MCR trap
	b.eq	el0_undef
	cmp	x24, #ESR_EL1_EC_CP15_64	// CP15 MRRC/MCRR trap
	b.eq	el0_undef
	cmp	x24, #ESR_EL1_EC_CP14_MR	// CP14 MRC/MCR trap
	b.eq	el0_undef
	cmp	x24, #ESR_EL1_EC_CP14_LS	// CP14 LDC/STC trap
	b.eq	el0_undef
	cmp	x24, #ESR_EL1_EC_CP14_64	// CP14 MRRC/MCRR trap
	b.eq	el0_undef
	cmp	x24, #ESR_EL1_EC_BREAKPT_EL0	// debug exception in EL0
	b.ge	el0_dbg
	b	el0_inv
el0_svc_compat:
	/*
	 * AArch32 syscall handling
	 */
	adr	stbl, compat_sys_call_table	// load compat syscall table pointer
	uxtw	scno, w7			// syscall number in w7 (r7)
	mov     sc_nr, #__NR_compat_syscalls
	b	el0_svc_naked

	.align	6
el0_irq_compat:
	kernel_entry 0, 32
	b	el0_irq_naked
#endif

el0_da:
	/*
	 * Data abort handling
	 */
	mrs	x26, far_el1
	// enable interrupts before calling the main handler
	enable_dbg_and_irq
	ct_user_exit
	clear_address_tag x0, x26
	mov	x1, x25
	mov	x2, sp
	adr	lr, ret_to_user
	b	do_mem_abort
el0_ia:
	/*
	 * Instruction abort handling
	 */
	mrs	x26, far_el1
	// enable interrupts before calling the main handler
	enable_dbg_and_irq
	ct_user_exit
	mov	x0, x26
	orr	x1, x25, #1 << 24		// use reserved ISS bit for instruction aborts
	mov	x2, sp
	adr	lr, ret_to_user
	b	do_mem_abort
el0_fpsimd_acc:
	/*
	 * Floating Point or Advanced SIMD access
	 */
	enable_dbg
	ct_user_exit
	mov	x0, x25
	mov	x1, sp
	adr	lr, ret_to_user
	b	do_fpsimd_acc
el0_fpsimd_exc:
	/*
	 * Floating Point or Advanced SIMD exception
	 */
	enable_dbg
	ct_user_exit
	mov	x0, x25
	mov	x1, sp
	adr	lr, ret_to_user
	b	do_fpsimd_exc
el0_sp_pc:
	/*
	 * Stack or PC alignment exception handling
	 */
	mrs	x26, far_el1
	// enable interrupts before calling the main handler
	enable_dbg_and_irq
	ct_user_exit
	mov	x0, x26
	mov	x1, x25
	mov	x2, sp
	adr	lr, ret_to_user
	b	do_sp_pc_abort
el0_undef:
	/*
	 * Undefined instruction
	 */
	// enable interrupts before calling the main handler
	enable_dbg_and_irq
	ct_user_exit
	mov	x0, sp
	adr	lr, ret_to_user
	b	do_undefinstr
el0_dbg:
	/*
	 * Debug exception handling
	 */
	tbnz	x24, #0, el0_inv		// EL0 only
	mrs	x0, far_el1
	mov	x1, x25
	mov	x2, sp
	bl	do_debug_exception
	enable_dbg
	ct_user_exit
	b	ret_to_user
el0_inv:
	enable_dbg
	ct_user_exit
	mov	x0, sp
	mov	x1, #BAD_SYNC
	mrs	x2, esr_el1
	bl	bad_el0_sync
	b	ret_to_user
ENDPROC(el0_sync)
```

##### For example, assume data abort is triggered. Kernel finds data abort handling and calls do_mem_abort function. LR register is written with ret_to_user function. Therefore when do_mem_abort function is completed, ret_to_user function is called.
```C
el0_da:
	/*
	 * Data abort handling
	 */
	mrs	x26, far_el1
	// enable interrupts before calling the main handler
	enable_dbg_and_irq
	ct_user_exit
	clear_address_tag x0, x26
	mov	x1, x25
	mov	x2, sp
	adr	lr, ret_to_user
	b	do_mem_abort
```


##### For data abort, there are various fault handler functions that are defined in struct fault info. Kernel can call corresponding function according to strcut fault info address plus offset from esr register.


```C
/*
 * Dispatch a data abort to the relevant handler.
 */
asmlinkage void __exception do_mem_abort(unsigned long addr, unsigned int esr,
					 struct pt_regs *regs)
{
	const struct fault_info *inf = fault_info + (esr & 63);
	struct siginfo info;

	if (!inf->fn(addr, esr, regs))
		return;

	pr_alert("Unhandled fault: %s (0x%08x) at 0x%016lx/n",
		 inf->name, esr, addr);

	info.si_signo = inf->sig;
	info.si_errno = 0;
	info.si_code  = inf->code;
	info.si_addr  = (void __user *)addr;
	arm64_notify_die("", regs, &info, esr);
}
```

```C
static struct fault_info {
	int	(*fn)(unsigned long addr, unsigned int esr, struct pt_regs *regs);
	int	sig;
	int	code;
	const char *name;
} fault_info[] = {
	{ do_bad,		SIGBUS,  0,		"ttbr address size fault"	},
	{ do_bad,		SIGBUS,  0,		"level 1 address size fault"	},
	{ do_bad,		SIGBUS,  0,		"level 2 address size fault"	},
	{ do_bad,		SIGBUS,  0,		"level 3 address size fault"	},
	{ do_translation_fault,	SIGSEGV, SEGV_MAPERR,	"input address range fault"	},
	{ do_translation_fault,	SIGSEGV, SEGV_MAPERR,	"level 1 translation fault"	},
	{ do_translation_fault,	SIGSEGV, SEGV_MAPERR,	"level 2 translation fault"	},
	{ do_page_fault,	SIGSEGV, SEGV_MAPERR,	"level 3 translation fault"	},
	{ do_bad,		SIGBUS,  0,		"reserved access flag fault"	},
	{ do_page_fault,	SIGSEGV, SEGV_ACCERR,	"level 1 access flag fault"	},
	{ do_page_fault,	SIGSEGV, SEGV_ACCERR,	"level 2 access flag fault"	},
	{ do_page_fault,	SIGSEGV, SEGV_ACCERR,	"level 3 access flag fault"	},
	{ do_bad,		SIGBUS,  0,		"reserved permission fault"	},
	{ do_page_fault,	SIGSEGV, SEGV_ACCERR,	"level 1 permission fault"	},
	{ do_page_fault,	SIGSEGV, SEGV_ACCERR,	"level 2 permission fault"	},
	{ do_page_fault,	SIGSEGV, SEGV_ACCERR,	"level 3 permission fault"	},
	{ do_bad,		SIGBUS,  0,		"synchronous external abort"	},
	{ do_bad,		SIGBUS,  0,		"asynchronous external abort"	},
	{ do_bad,		SIGBUS,  0,		"unknown 18"			},
	{ do_bad,		SIGBUS,  0,		"unknown 19"			},
	{ do_bad,		SIGBUS,  0,		"synchronous abort (translation table walk)" },
	{ do_bad,		SIGBUS,  0,		"synchronous abort (translation table walk)" },
	{ do_bad,		SIGBUS,  0,		"synchronous abort (translation table walk)" },
	{ do_bad,		SIGBUS,  0,		"synchronous abort (translation table walk)" },
	{ do_bad,		SIGBUS,  0,		"synchronous parity error"	},
	{ do_bad,		SIGBUS,  0,		"asynchronous parity error"	},
	{ do_bad,		SIGBUS,  0,		"unknown 26"			},
	{ do_bad,		SIGBUS,  0,		"unknown 27"			},
	{ do_bad,		SIGBUS,  0,		"synchronous parity error (translation table walk" },
	{ do_bad,		SIGBUS,  0,		"synchronous parity error (translation table walk" },
	{ do_bad,		SIGBUS,  0,		"synchronous parity error (translation table walk" },
	{ do_bad,		SIGBUS,  0,		"synchronous parity error (translation table walk" },
	{ do_bad,		SIGBUS,  0,		"unknown 32"			},
	{ do_bad,		SIGBUS,  BUS_ADRALN,	"alignment fault"		},
	{ do_bad,		SIGBUS,  0,		"debug event"			},
	{ do_bad,		SIGBUS,  0,		"unknown 35"			},
	{ do_bad,		SIGBUS,  0,		"unknown 36"			},
	{ do_bad,		SIGBUS,  0,		"unknown 37"			},
	{ do_bad,		SIGBUS,  0,		"unknown 38"			},
	{ do_bad,		SIGBUS,  0,		"unknown 39"			},
	{ do_bad,		SIGBUS,  0,		"unknown 40"			},
	{ do_bad,		SIGBUS,  0,		"unknown 41"			},
	{ do_bad,		SIGBUS,  0,		"unknown 42"			},
	{ do_bad,		SIGBUS,  0,		"unknown 43"			},
	{ do_bad,		SIGBUS,  0,		"unknown 44"			},
	{ do_bad,		SIGBUS,  0,		"unknown 45"			},
	{ do_bad,		SIGBUS,  0,		"unknown 46"			},
	{ do_bad,		SIGBUS,  0,		"unknown 47"			},
	{ do_bad,		SIGBUS,  0,		"unknown 48"			},
	{ do_bad,		SIGBUS,  0,		"unknown 49"			},
	{ do_bad,		SIGBUS,  0,		"unknown 50"			},
	{ do_bad,		SIGBUS,  0,		"unknown 51"			},
	{ do_bad,		SIGBUS,  0,		"implementation fault (lockdown abort)" },
	{ do_bad,		SIGBUS,  0,		"unknown 53"			},
	{ do_bad,		SIGBUS,  0,		"unknown 54"			},
	{ do_bad,		SIGBUS,  0,		"unknown 55"			},
	{ do_bad,		SIGBUS,  0,		"unknown 56"			},
	{ do_bad,		SIGBUS,  0,		"unknown 57"			},
	{ do_bad,		SIGBUS,  0,		"implementation fault (coprocessor abort)" },
	{ do_bad,		SIGBUS,  0,		"unknown 59"			},
	{ do_bad,		SIGBUS,  0,		"unknown 60"			},
	{ do_bad,		SIGBUS,  0,		"unknown 61"			},
	{ do_bad,		SIGBUS,  0,		"unknown 62"			},
	{ do_bad,		SIGBUS,  0,		"unknown 63"			},
};

```

##### If kernel fails to find any matched fault hander function, unhandled fault happens and kernel crashes the machine. This behavior is able to be modified so the machine can keep alive.