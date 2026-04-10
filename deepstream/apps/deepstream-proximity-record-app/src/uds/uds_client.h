#ifndef UDS_CLIENT_H
#define UDS_CLIENT_H

#include <stddef.h>

#include "deepstream_proximity_protocol.h"

int uds_client_init(const char *socket_path);
int uds_client_send_packet(const DetectionPacket *packet);
int uds_client_recv(char *buf, size_t size);
void uds_client_close(void);

#endif
