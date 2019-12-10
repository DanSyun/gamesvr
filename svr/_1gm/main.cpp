#include <iostream>
#include <string>
#include "proto_base.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "base.h"
#include <unistd.h>
#include "proto_log.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    ProtoLog::Instance()->Init();
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) return 0;
    std::string ip = "127.0.0.1";
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(40000);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(fd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) return 0;

    string cmd;
    for (auto i = 1; i < argc; ++i)
    {
        cmd += argv[i];
        cmd += " ";
    }

    PBSSMsg msg;
    msg.mutable_ss_gm_command()->set_cmd(cmd);
    if (msg.ByteSize() > MAX_CS_PKG_BUF_LEN) return 0;
    char buf[MAX_CS_PKG_BUF_LEN];
    uint32 buf_len = msg.ByteSize() + sizeof(uint32);

    msg.SerializeWithCachedSizesToArray((google::protobuf::uint8*)(buf + sizeof(uint32)));
    *(uint32*)buf = htonl(buf_len);
    while (true)
    {
        int len = 0;
        len = send(fd, buf, buf_len, 0);
        if (len == -1)
        {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) continue;
            else return 0;
        }
        else
            break;
    }
    int len = recv(fd, buf, sizeof(buf), 0);
    if ((uint32)len > sizeof(uint32))
    {
        buf_len = ntohl(*(uint32*)buf);
        msg.ParseFromArray(buf + sizeof(uint32), buf_len - sizeof(uint32));
        ProtoLog::Instance()->PrintProto(msg);
    }
    close(fd);
}
