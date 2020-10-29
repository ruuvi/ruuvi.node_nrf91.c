
/* auto-generated by gen_syscalls.py, don't edit */
#ifndef Z_INCLUDE_SYSCALLS_DMA_H
#define Z_INCLUDE_SYSCALLS_DMA_H


#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <syscall.h>

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_dma_start(struct device * dev, u32_t channel);
static inline int dma_start(struct device * dev, u32_t channel)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&dev, *(uintptr_t *)&channel, K_SYSCALL_DMA_START);
	}
#endif
	compiler_barrier();
	return z_impl_dma_start(dev, channel);
}


extern int z_impl_dma_stop(struct device * dev, u32_t channel);
static inline int dma_stop(struct device * dev, u32_t channel)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		return (int) arch_syscall_invoke2(*(uintptr_t *)&dev, *(uintptr_t *)&channel, K_SYSCALL_DMA_STOP);
	}
#endif
	compiler_barrier();
	return z_impl_dma_stop(dev, channel);
}


#ifdef __cplusplus
}
#endif

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif

#endif
#endif /* include guard */
