#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/list.h>

struct birthday {
	int day,month,year;
	struct list_head list;
};
struct birthday *person,*ptr,*next;
static LIST_HEAD(birthday_list);
int init_hello_1(void)
{
	int i;
	printk(KERN_INFO "Module Loaded Successfully\n");
	for(i=0;i<5;i++)
	{
		person = kmalloc(sizeof(*person),GFP_KERNEL);
		person->day = person->month = i+1;
		person->year = 1991+i;
		INIT_LIST_HEAD(&person->list);
		list_add_tail(&person->list, &birthday_list);
	}
	list_for_each_entry(ptr, &birthday_list, list)
	{
		printk("%d\t%d\t%d\n",ptr->day,ptr->month,ptr->year);
//		list_del(&ptr->list);
//		kfree(ptr);
	}
		
	return 0;
}

void unload_hello_1(void)
{
	printk(KERN_INFO "Module Unloaded\n");
	list_for_each_entry_safe(ptr,next,&birthday_list,list) {
		list_del(&ptr->list);
		kfree(ptr);
	}

}

module_init(init_hello_1);
module_exit(unload_hello_1);
MODULE_LICENSE("GPL");
