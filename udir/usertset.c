#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
//#include <uapi/linux/netlink.h>

//#define NETLINK_TEST 29
#define MAX_PAYLOAD 1024

#define MY_MSG_TYPE (0x10 + 2)
struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

/*
   "./include/uapi/linux/netlink.h"
   struct nlmsghdr{
		__u32		nlmsg_len;  //length of message including
		__u16		nlmsg_type; //message content
		__u16		nlmsg_flags;//additional flags
		__u32		nlmsg_seq; //sequence number
		__u32		mlmsg_pid; //sending process port id

   }

	struct sockaddr_nl{
		__kernel_sa_family_t    nl_family; //AF_BETLINK
		unsigned short			nl_pad;    //zero
		__u32					nl_pid;    //port ID
		__u32					nl_groups; //multicast groups mask
	}	
   
   struct msghdr{
		void			*msg_name	//optional address
		socklen_t		msg_namelen //sizeof address
		struct iovec	msg_iov		//scatter/gather array
		size_t			msg_iovlen  //elements in msg_iov
		void			*msg_control //ancillary data,
		size_t			msg_controllen //ancillary data buffer len
		int				msg_flags	 //flags on received message
	}
*/
int main(int argc, char **argv)
{
	
	//char msgh[] = {0xde, 0xad, 0xef,  0x90, 0x0d, 0xbe, 0xef};
	printf("sock fd before\n");
	sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
	//sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
	if(sock_fd<0)
	{
		perror("socket error");
		exit(1);
	}
	memset(&msg, 0, sizeof(msg));
	memset(&src_addr, 0, sizeof(src_addr));
	
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();
	src_addr.nl_pad = 0;
	src_addr.nl_groups = 0; //unicast
	
	bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;
	dest_addr.nl_groups = 0;
	
	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_flags = 0;
	nlh->nlmsg_type = MY_MSG_TYPE;
	
	strcpy(NLMSG_DATA(nlh), "Hello World");


	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	int n;
	if((n=sendmsg(sock_fd, (struct msghdr *)&msg, 0)) < 0){
		perror("sendmsg error");
		exit(1);
	}else{
		printf("n ---->%d\n", n);
	}

	//while(1){
		memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
		if((n=recvmsg(sock_fd,  &msg, 0)) > 0){
			printf("Received message %s\n",(char *)NLMSG_DATA(nlh));
		//	printf("receive msg is %s\n", (char *)NLMSG_DATA((struct msghdr)msg.msg_iov.iov_base));
		}else if(n < 0){
			perror("recvmsg error");
			exit(1);
		}else{
			printf("peer has performed\n");
		}
	//}
	close(sock_fd);
}

