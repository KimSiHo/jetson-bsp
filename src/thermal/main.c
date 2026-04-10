#include <stdio.h>
#include <stdlib.h>

#include <linux/thermal.h>

#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>

#define DEBUG 0

#define LOG(fmt, ...) do { if (DEBUG) printf(fmt, ##__VA_ARGS__); } while (0)
#define INFO(fmt, ...) printf(fmt, ##__VA_ARGS__)
#define ERR(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)

static int cb_msg(struct nl_msg *msg, void *arg)
{
    struct nlmsghdr *nlh = nlmsg_hdr(msg);
    struct genlmsghdr *ghdr = nlmsg_data(nlh);
    struct nlattr *tb[THERMAL_GENL_ATTR_MAX + 1];

    LOG("========================================\n");
    LOG("[RECV] Message Incoming\n");
    LOG("nlmsg_type : %u\n", nlh->nlmsg_type);
    LOG("nlmsg_len=%u\n", nlh->nlmsg_len);
    LOG("cmd=%u\n", ghdr->cmd);
    LOG("========================================\n");

    if (genlmsg_parse(nlh, 0, tb, THERMAL_GENL_ATTR_MAX, NULL) < 0) {
        ERR("Failed to parse generic netlink attributes\n");
        return NL_SKIP;
    }

    switch (ghdr->cmd) {

    case THERMAL_GENL_SAMPLING_TEMP:
    {
       if (!tb[THERMAL_GENL_ATTR_TZ_ID] || !tb[THERMAL_GENL_ATTR_TZ_TEMP]) {
           ERR("THERMAL_GENL_SAMPLING_TEMP unexpected format\n");
           break;
       }

       uint32_t tz_id = nla_get_u32(tb[THERMAL_GENL_ATTR_TZ_ID]);
       uint32_t temp  = nla_get_u32(tb[THERMAL_GENL_ATTR_TZ_TEMP]);

       if (tz_id != 0)
           break;

       INFO("[SAMPLING_TEMP] Thermal Zone ID : %u\n", tz_id);
       INFO("Temperature : %.3f C (raw: %u)\n", (float)temp / 1000.0, temp);
       break;
    }

    case THERMAL_GENL_EVENT_TZ_CREATE:
        INFO("[TZ_CREATE]\n");
        break;

    case THERMAL_GENL_EVENT_TZ_DELETE:
        INFO("[TZ_DELETE]\n");
        break;

    case THERMAL_GENL_EVENT_TZ_ENABLE:
        INFO("[TZ_ENABLE]\n");
        break;

    case THERMAL_GENL_EVENT_TZ_DISABLE:
        INFO("[TZ_DISABLE]\n");
        break;

    case THERMAL_GENL_EVENT_TZ_TRIP_UP:
        INFO("[TRIP_UP]\n");

        if (tb[THERMAL_GENL_ATTR_TZ_ID])
            INFO("TZ ID   : %u\n", nla_get_u32(tb[THERMAL_GENL_ATTR_TZ_ID]));

        if (tb[THERMAL_GENL_ATTR_TZ_TRIP_ID])
            INFO("TRIP ID : %u\n", nla_get_u32(tb[THERMAL_GENL_ATTR_TZ_TRIP_ID]));

        break;

    case THERMAL_GENL_EVENT_TZ_TRIP_DOWN:
        INFO("[TRIP_DOWN]\n");

        if (tb[THERMAL_GENL_ATTR_TZ_ID])
            INFO("TZ ID   : %u\n", nla_get_u32(tb[THERMAL_GENL_ATTR_TZ_ID]));

        if (tb[THERMAL_GENL_ATTR_TZ_TRIP_ID])
            INFO("TRIP ID : %u\n", nla_get_u32(tb[THERMAL_GENL_ATTR_TZ_TRIP_ID]));

        break;

    case THERMAL_GENL_EVENT_CDEV_STATE_UPDATE:
        INFO("[CDEV_STATE_UPDATE]\n");

        if (tb[THERMAL_GENL_ATTR_CDEV_ID])
            INFO("CDEV ID : %u\n", nla_get_u32(tb[THERMAL_GENL_ATTR_CDEV_ID]));

        if (tb[THERMAL_GENL_ATTR_CDEV_CUR_STATE])
            INFO("STATE   : %u\n", nla_get_u32(tb[THERMAL_GENL_ATTR_CDEV_CUR_STATE]));

        break;

    default:
        INFO("[UNKNOWN CMD] %u\n", ghdr->cmd);
        break;
    }

    return NL_OK;
}

int main(void)
{
    struct nl_sock *sock;
    int family_id;
    int sampling_grp;
    int event_grp;
    int ret;

    sock = nl_socket_alloc();
    if (!sock) {
        ERR("nl_socket_alloc failed\n");
        return -1;
    }

    ret = genl_connect(sock);
    if (ret < 0) {
        ERR("genl_connect failed: %s\n", nl_geterror(ret));
        return -1;
    }

    /* multicast listener에서는 필수 */
    nl_socket_disable_seq_check(sock);

    family_id = genl_ctrl_resolve(sock, "thermal");
    if (family_id < 0) {
        ERR("failed to resolve thermal family\n");
        return -1;
    }

    INFO("thermal family id = %d\n", family_id);

    sampling_grp = genl_ctrl_resolve_grp(sock, "thermal", "sampling");
    event_grp = genl_ctrl_resolve_grp(sock, "thermal", "event");

    INFO("sampling group = %d\n", sampling_grp);
    INFO("event group    = %d\n", event_grp);

    ret = nl_socket_add_membership(sock, sampling_grp);
    INFO("join sampling ret = %d\n", ret);

    ret = nl_socket_add_membership(sock, event_grp);
    INFO("join event ret    = %d\n", ret);

    nl_socket_modify_cb(sock, NL_CB_VALID, NL_CB_CUSTOM, cb_msg, NULL);

    INFO("waiting...\n");

    while (1) {
        ret = nl_recvmsgs_default(sock);

        if (ret < 0) {
            ERR("recv error: %s (%d)\n", nl_geterror(ret), ret);
        }
    }

    nl_socket_free(sock);

    return 0;
}
