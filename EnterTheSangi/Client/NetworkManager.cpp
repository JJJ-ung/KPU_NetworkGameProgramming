#include "NetworkManager.h"

NetworkMgr::NetworkMgr()
{
	WSAStartup(MAKEWORD(2, 2), &m_wsa);
	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	ZeroMemory(&m_addr, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	m_addr.sin_port = htons(SERVER_PORT);

	retval = connect(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr));
}

NetworkMgr::~NetworkMgr()
{
	closesocket(m_socket);
	WSACleanup();
}

void NetworkMgr::err_display(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	std::cout << "[" << msg << "] : " << (char*)lpMsgBuf << std::endl;
	LocalFree(lpMsgBuf);
}

void NetworkMgr::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);

	exit(1);
}

void NetworkMgr::Activate()
{

}

void NetworkMgr::do_send()
{
	// 앞에서 처리할거 다 처리한 후

	retval = send(m_socket, (char*)&m_playerPacket, sizeof(GameStatePlayer), 0);

	if (retval == SOCKET_ERROR)
		err_display("send()");
}

void NetworkMgr::do_send_customizing() //커스터마이징 정보 수신
{
	retval = send(m_socket, (char*)&m_player_color_pakcet, sizeof(cs_change_color), 0);
	
	if (retval == SOCKET_ERROR)
		err_display("send()");
}

void NetworkMgr::do_recv()
{
	char buf[BUF_SIZE];

	retval = recv(m_socket, buf, BUF_SIZE, 0);

	if (retval == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			return;
		err_display("recv()");
		return;
	}

	else if (retval == 0) {
		std::cout << "recv == 0" << std::endl;
		return;
	}

	if (buf[1] == '0'){ // 다른 캐릭터 이동
		GameStatePlayer rp;
		memcpy(&rp, &buf, sizeof(GameStatePlayer));
	}
	
	else if (buf[1] == '1') { // 커스터마이징 정보 수신
		sc_change_color rp;
		memcpy(&rp, &buf, sizeof(sc_change_color));
	}
}

int NetworkMgr::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}

