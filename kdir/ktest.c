#include<linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/sock.h>
#include <linux/netlink.h>

#define NETLINK_TEST 29
#define MAX_PAYLOAD 1024

static u32 pid = 0;
static struct sock *nl_sk = NULL;

void nl_data_ready(struct sk_buff *__sk);
static int netlink_sendmsg(const char *buffer, unsigned int size);

void nl_data_ready(struct sk_buff *__sk)
{
	struct nlmsghdr *nlh;
	struct sk_buff *skb;

	skb = skb_get(__sk);
	printk("skb-->len  %d\n", skb->len);
	if(skb->len >= NLMSG_SPACE(0)){
		nlh = (struct nlmsghdr *)skb->data;
		if(pid != 0) printk("pid !=0 \n");
		pid = nlh->nlmsg_pid;
		printk("net_link: send proc pid is %d\n, data is %s\n ", pid, (char *)NLMSG_DATA(nlh));
		kfree_skb(skb);
	}
	
	char str[MAX_PAYLOAD];
	strcpy(str, "hello from kernel");
	netlink_sendmsg(str, sizeof(str));
	return ;
}

static int netlink_sendmsg(const char *buffer, unsigned int size)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;

	skb = nlmsg_new(MAX_PAYLOAD, GFP_ATOMIC);
	if(!skb)
	{
		printk(KERN_ERR"failed to alloc skb\n");
		return -1;
	}

	nlh = nlmsg_put(skb, 0, 0,0, MAX_PAYLOAD, 0);
	//NETLINK_CB(skb).portid = 0;
	memcpy(NLMSG_DATA(nlh), buffer, size);

	if(netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT) < 0){
		printk(KERN_ERR"fail to send skb\n");
		return -2;
	}

	return 0;
}

static int __init netlink_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.input = nl_data_ready,
	};
	nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
	
	if(!nl_sk)
	{
		printk("netlink:can't create netlink socket\n");
	}else{
		printk("netlink:create ok\n");
	}

	return 0;
}

static void __exit netlink_exit(void)
{
	if(nl_sk != NULL){
		sock_release(nl_sk->sk_socket);
	}
}


module_init(netlink_init);
module_exit(netlink_exit);


MODULE_LICENSE("GPL");
