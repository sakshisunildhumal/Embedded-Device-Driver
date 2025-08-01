#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x3726c6aa, "module_layout" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xe62ca738, "gpio_to_desc" },
	{ 0xc5850110, "printk" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x4bc68d47, "gpiod_direction_input" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xb048a0f9, "gpiod_unexport" },
	{ 0xfe990052, "gpio_free" },
	{ 0xb72e3d5f, "gpiod_export" },
	{ 0xf9c63046, "gpiod_to_irq" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xb2d48a2e, "queue_work_on" },
	{ 0xdf9208c0, "alloc_workqueue" },
	{ 0xc1514a3b, "free_irq" },
};

MODULE_INFO(depends, "");

