#include "uds_client.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

static int uds_fd = -1;

int uds_client_init(const char *socket_path) {
	struct sockaddr_un addr;

	uds_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (uds_fd < 0) {
		perror("socket");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;

	strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

	if (connect(uds_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("connect");
		close(uds_fd);
		uds_fd = -1;
		return -1;
	}

	return 0;
}

int uds_client_send_packet(const DetectionPacket *packet) {
	const char *buf = (const char *)packet;
	size_t total = sizeof(*packet);
	size_t sent = 0;

	if (uds_fd < 0)
		return -1;

	while (sent < total) {
		ssize_t n = send(uds_fd, buf + sent, total - sent, 0);
		if (n < 0) {
			if (errno == EINTR)
				continue;

			perror("send");
			return -1;
		}

		if (n == 0)
			return -1;

		sent += n;
	}

	return 0;
}

int uds_client_recv(char *buf, size_t size) {
	if (uds_fd < 0)
		return -1;

	ssize_t n = recv(uds_fd, buf, size - 1, MSG_DONTWAIT);

	if (n < 0) {
		if (errno == EINTR)
			return 0;

		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return 0; // 받을 데이터 없음

		perror("recv");
		return -1;
	}

	if (n == 0)
		return -1; // 서버 종료

	buf[n] = '\0';
	return (int)n;
}

void uds_client_close(void) {
	if (uds_fd >= 0) {
		close(uds_fd);
		uds_fd = -1;
	}
}
