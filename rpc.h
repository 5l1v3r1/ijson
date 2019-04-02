
#ifndef RPC_H
#define RPC_H

#include <map>
#include <deque>

#include "server.h"
#include "utils.h"


using namespace std;

#define NET_START 0
#define NET_HEADER 1
#define NET_READ_BODY 2
#define NET_REQUEST_COMPLETED 3

#define STATUS_NET 21
#define STATUS_WAIT_JOB 22
#define STATUS_WAIT_RESPONSE 23


class Connect : public IConnect {
private:
    int step;
    int content_length;
    int http_version;  // 10, 11
    bool keep_alive;
    Buffer buffer;
    Buffer send_buffer;

    Buffer path;
    //Buffer method;
    Buffer name;
public:
    int status;
    Buffer body;
    Buffer id;

    Connect(int fd, TcpServer *server) : IConnect(fd, server) {
        step = NET_START;
        status = STATUS_NET;
    }
    void on_recv(char *buf, int size);
    int read_method(Slice &line);
    void read_header(Slice &data);

    void header_completed();
    void send(const char *http_status);
    void send(const char *http_status, Buffer *body);
    void send(const char *http_status, Buffer *id,  Buffer *body);
    void on_send();
};

class MethodLine {
public:
    std::deque<Connect*> workers;
    std::deque<Connect*> clients;
};

class RpcServer : public TcpServer {
public:
    std::map<std::string, MethodLine*> methods;
    std::map<std::string, Connect*> wait_response;
    
    bool counter_active;
    long counter;
    long counter_start;
    
    RpcServer() : TcpServer() {
        counter_active = true;
        counter = 0;
        counter_start = 0;
    };

    IConnect* on_connect(int fd) {
        return new Connect(fd, this);
    };
    
    void add_worker(Buffer &name, Connect *worker);
    int client_request(string &name, Connect *client);
    int worker_result(Buffer *id, Connect *worker);
};


#endif /* RPC_H */
