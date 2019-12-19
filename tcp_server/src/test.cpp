#include <vector>
#include <set>
#include "test.h"
using namespace std;

std::set<uint32_t> tcp_ids;
uint32_t send_data = 0;
bool is_create_timer = false;

//定时器回调函数
void on_timer(uint64_t val)
{
    pkg_test test;
    test.count_ = send_data;
    byte_t* data1 = reinterpret_cast<byte_t*>(&test);
    for (auto& tcp_id : tcp_ids) {
        //actor库的发包接口
        send_package(data1, sizeof(test), 99999, tcp_id);
        ++send_data;
    }
}

//TCP连接成功回调函数,不同的连接tcp_chan_id不同
void handler_tcp_connect(const byte_t* data, uint32_t len, uint32_t tcp_chan_id, void* user_data)
{
    cout << "actor 1 on connect, tcp_chan_id = " << tcp_chan_id << endl;
    tcp_ids.insert(tcp_chan_id);
    if (! is_create_timer) {
        //启动定时器发送数据
        timer_callback callback_func = std::bind(&on_timer, std::placeholders::_1);
        //actor库的创建定时器接口
        create_timer(1, base::event::timer_event::CLOCK_TYPE::MONOTONIC,
                1, 0, callback_func);
        is_create_timer = true;
    }
}

//TCP断开连接回调函数
void handler_tcp_close(const byte_t* data, uint32_t len, uint32_t tcp_chan_id, void* user_data)
{
    cout << "actor 1 on close, tcp_chan_id = " << tcp_chan_id << endl;
    tcp_ids.erase(tcp_chan_id);
}

//功能码=99999的数据包回调函数
void handler_actor1(const byte_t* data, uint32_t len, uint32_t tcp_chan_id, void* user_data)
{
    pkg_test* pkg = (pkg_test*)data;
    uint32_t recv_data = pkg->count_;
    cout << recv_data << endl;
}

int32_t test_class::pre_action()
{
   // reg handler
    // TCP连接成功 -> handler_tcp_connect
    handler_t h_actor1_tcp_connect = std::bind(&handler_tcp_connect, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3, std::placeholders::_4);
    reg_handler(1, static_cast<fc_t>(FUNC_CODE::FC_TCP_CONNECT), h_actor1_tcp_connect);

    // TCP连接断开 -> handler_tcp_close
    handler_t h_actor1_tcp_close = std::bind(&handler_tcp_close, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3, std::placeholders::_4);
    reg_handler(1, static_cast<fc_t>(FUNC_CODE::FC_TCP_CLOSE), h_actor1_tcp_close);

    // 99999 -> handler_actor1
    handler_t h_actor1_pkg_test = std::bind(&handler_actor1, std::placeholders::_1, std::placeholders::_2,
            std::placeholders::_3, std::placeholders::_4);
    reg_handler(1, 99999, h_actor1_pkg_test);

    return 0;
}

int32_t test_class::post_action()
{
    return 0;
}

// main函数封装
MAIN(test_class)
