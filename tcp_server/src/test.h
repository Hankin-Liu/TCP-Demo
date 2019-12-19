#include <iostream>
#include <string.h>
#include <memory>
#include "actor/actor_framework.h"
#include "actor/actor_api.h"
#include "common/pkg_def.h"

using namespace actor;
//1字节对齐
#pragma pack(push, 1)
//数据包定义
struct pkg_test
{
    DEF_FUNC_CODE(99999)
    DEF_GET_SIZE(pkg_test)
    char msg[128];
    uint64_t count_{ 0 };
};
#pragma pack(pop)

//业务类,需要继承actor库的actor_framework类
class test_class : public actor_framework
{
    public:
        //重写虚函数,注册回调函数
        virtual int32_t pre_action() override;
        //重写虚函数,暂时没用,后续扩展用
        virtual int32_t post_action() override;
};
