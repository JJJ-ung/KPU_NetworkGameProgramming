#include "MainServer.h"

explicit MainServer::MainServer() {};
MainServer::~MainServer() {};

void MainServer::Init(const int server_port)
{
    int rt;

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    //socket()
    m_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listen_socket == INVALID_SOCKET) {
        //err_quit("socket()")
    };

    //bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(server_port);
    rt=bind(m_listen_socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (rt == SOCKET_ERROR) {
        //err_quit("bind()")
    };


    for (int i = 0; i < MAX_CLIENTS; ++i)
        m_clients[i].SetID(i);

    m_can_connect = true;
};

void MainServer::Activate()
{
    int rt;

    //listen()
    rt = listen(m_listen_socket, SOMAXCONN);
    if (rt == SOCKET_ERROR) {
        //err_quit("listen()")
    };

    //클라이언트별로 하나씩 할당되는 스레드.
    for (int i = 0; i < MAX_CLIENTS; ++i)
        m_client_threads.emplace_back(&MainServer::ClientThread,this);
    for (auto& th : m_client_threads)
        th.join();

    //accept만을 위한 스레드. 주스레드에서 accpet 상관 없이 서버 연산을 돌리기 위함.
    for (int i = 0; i < 1; ++i)
        m_accpet_threads.emplace_back(&MainServer::AccpetThread, this);
    for (auto& th : m_accpet_threads)
        th.join();

    //accept()

};

void MainServer::ClientThread() 
{
    for (;;)
    {
        //In Robby




        //In Game

        while (true)
        {
            DoRecv();
            //timeout 필요
        }
        //수신 성공시 suspend thread
        //모든 클라이언트로부터 recv 혹은 timeout시 서버 연산
        //이후 resume thread
        DoSend();

    }
};

void MainServer::AccpetThread()
{
    for (;;)
    {
        if (m_can_connect == false) //data race 발생가능 -> 추후 보완 필요
            continue;
        //accpet()
        SOCKET client_socket;
        SOCKADDR_IN client_addr;
        int addr_len=sizeof(client_addr);
        client_socket = accept(m_listen_socket, (SOCKADDR*)&client_addr, &addr_len);
        if (client_socket == INVALID_SOCKET)
        {
            //free인 클라이언트의 id를 찾는다
            //해당 클라이언트에 소켓 정보, 소켓 주소 구조체 정보를 넘긴다
            //해당 클라이언트의 state를 업데이트한다
            //m_can_connect를 업데이트한다
        }
    }
};

void MainServer::DoSend()
{

};

void MainServer::DoRecv()
{

};

void MainServer::DoAccept()
{

};

