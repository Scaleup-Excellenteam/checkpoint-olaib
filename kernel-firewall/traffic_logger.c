#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ola Ibrahim");
MODULE_DESCRIPTION("A simple Traffic logger kernel module");
MODULE_VERSION("1.00");

#define HOOK_MSG_SUCCESS "Traffic logger: Hook registered successfully\n"
#define HOOK_MSG_FAIL "Traffic logger: Hook registration failed\n"
#define IP_REGEX "^([0-9]{1,3}\\.){3}[0-9]{1,3}$"

struct nf_hook_ops HOOKS_OPTIONS = { 0 }; // for configuring hook options

static unsigned int log_incoming_traffic(void* priv, struct sk_buff* skb, const struct nf_hook_state* state);

static int __init traffic_logger_init(void);
static void __exit traffic_logger_exit(void);

/*
    ping blocker hook function
    @param priv: private data pointer
    @param skb: socket buffer pointer
    @param state: hook state pointer
    @return: unsigned int (NF_ACCEPT, NF_DROP) -> (accept packet, drop packet)
*/
static unsigned int log_incoming_traffic(void* priv, struct sk_buff* skb, const struct nf_hook_state* state)
{
    struct iphdr* iph; // IP header struct
    struct tcphdr* tcph; // TCP header struct
    if (!skb) return NF_ACCEPT; // if no packet, accept it

    iph = ip_hdr(skb); // get IP header from packet

    if (iph->protocol == IPPROTO_TCP) // if protocol is TCP
    {
        tcph = tcp_hdr(skb); // get TCP header from packet
        printk(KERN_INFO "Incoming traffic: %pI4: %d --> %pI4: %d\n",
            &iph->saddr, ntohs(tcph->source),
            &iph->daddr, ntohs(tcph->dest)
        );
    }

    return NF_ACCEPT; // accept packet
}

static int __init traffic_logger_init(void)
{
    printk(KERN_INFO "Incoming traffic initializing ...\n");

    HOOKS_OPTIONS.hook = log_incoming_traffic; // set hook function
    HOOKS_OPTIONS.pf = NFPROTO_IPV4; // set protocol family
    HOOKS_OPTIONS.hooknum = NF_INET_LOCAL_IN; // set hook number
    HOOKS_OPTIONS.priority = NF_IP_PRI_FIRST; // set hook priority

    int ret = nf_register_net_hook(&init_net, &HOOKS_OPTIONS); // register hook
    printk(KERN_INFO "%s", ret == 0 ? HOOK_MSG_SUCCESS : HOOK_MSG_FAIL);

    return ret;
}

static void __exit traffic_logger_exit(void)
{
    nf_unregister_net_hook(&init_net, &HOOKS_OPTIONS); // unregister hook
    printk(KERN_INFO "Traffic_logger: Exiting...\n");
}

module_init(traffic_logger_init);
module_exit(traffic_logger_exit);


