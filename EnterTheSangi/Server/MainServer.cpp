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

    //Ŭ���̾�Ʈ���� �ϳ��� �Ҵ�Ǵ� ������.
    for (int i = 0; i < MAX_CLIENTS; ++i)
        m_client_threads.emplace_back(&MainServer::ClientThread,this);
    for (auto& th : m_client_threads)
        th.join();

    //accept���� ���� ������. �ֽ����忡�� accpet ��� ���� ���� ������ ������ ����.
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
            //timeout �ʿ�
        }
        //���� ������ suspend thread
        //��� Ŭ���̾�Ʈ�κ��� recv Ȥ�� timeout�� ���� ����
        //���� resume thread
        DoSend();

    }
};

void MainServer::AccpetThread()
{
    for (;;)
    {
        if (m_can_connect == false) //data race �߻����� -> ���� ���� �ʿ�
            continue;
        //accpet()
        SOCKET client_socket;
        SOCKADDR_IN client_addr;
        int addr_len=sizeof(client_addr);
        client_socket = accept(m_listen_socket, (SOCKADDR*)&client_addr, &addr_len);
        if (client_socket == INVALID_SOCKET)
        {
            //free�� Ŭ���̾�Ʈ�� id�� ã�´�
            //�ش� Ŭ���̾�Ʈ�� ���� ����, ���� �ּ� ����ü ������ �ѱ��
            //�ش� Ŭ���̾�Ʈ�� state�� ������Ʈ�Ѵ�
            //m_can_connect�� ������Ʈ�Ѵ�
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

