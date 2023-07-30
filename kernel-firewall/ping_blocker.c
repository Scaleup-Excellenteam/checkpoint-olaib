#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/icmp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ola Ibrahim");
MODULE_DESCRIPTION("A simple ping blocker linux firewall");
MODULE_VERSION("1.00");

#define HOOK_MSG_SUCCESS "Ping Blocker: Hook registered successfully\n"
#define HOOK_MSG_FAIL "Ping Blocker: Hook registration failed\n"
#define IP_REGEX "^([0-9]{1,3}\\.){3}[0-9]{1,3}$"

struct nf_hook_ops HOOKS_OPTIONS = { 0 };

static unsigned int ping_blocker_hook(void* priv, struct sk_buff* skb, const struct nf_hook_state* state);

static int __init firewall_init(void);
static void __exit firewall_exit(void);

/*
    ping blocker hook function
    @param priv: private data pointer
    @param skb: socket buffer pointer
    @param state: hook state pointer
    @return: unsigned int (NF_ACCEPT, NF_DROP) -> (accept packet, drop packet)
*/
static unsigned int ping_blocker_hook(void* priv, struct sk_buff* skb, const struct nf_hook_state* state)
{
    struct iphdr* iph; // IP header struct
    struct icmphdr* icmph; // ICMP header struct(icmp packet)
    if (!skb) return NF_ACCEPT; // if no packet, accept it

    iph = ip_hdr(skb); // get IP header from packet

    if (iph->protocol == IPPROTO_ICMP) // if protocol is ICMP
    {
        icmph = icmp_hdr(skb); // get ICMP header from packet
        if (icmph->type == ICMP_ECHO) // if ICMP type is ECHO
        {
            printk(KERN_INFO "Ping Blocker: Blocked ICMP packet from %pI4 to %pI4\n", &iph->saddr, &iph->daddr);
            return NF_DROP; // drop packet
        }
    }

    return NF_ACCEPT; // accept packet
}

static int __init firewall_init(void)
{
    printk(KERN_INFO "Ping Blocker: Initializing...\n");

    HOOKS_OPTIONS.hook = ping_blocker_hook; // set hook function
    HOOKS_OPTIONS.pf = NFPROTO_IPV4; // set protocol family
    HOOKS_OPTIONS.hook_ops_type = NF_HOOK_OP_NF_TABLES; // set hook type
    HOOKS_OPTIONS.hooknum = NF_INET_LOCAL_IN;
    HOOKS_OPTIONS.priority = NF_IP_PRI_FIRST;

    int ret = nf_register_net_hook(&init_net, &HOOKS_OPTIONS); // register hook
    printk(KERN_INFO "%s", ret == 0 ? HOOK_MSG_SUCCESS : HOOK_MSG_FAIL);

    return ret;
}

static void __exit firewall_exit(void)
{
    nf_unregister_net_hook(&init_net, &HOOKS_OPTIONS); // unregister hook
    printk(KERN_INFO "Ping Blocker: Exiting...\n");
}

module_init(firewall_init);
module_exit(firewall_exit);
