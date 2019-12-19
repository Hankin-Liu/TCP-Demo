#include <iostream>
#include <string.h>
#include <memory>
#include "actor/actor_framework.h"
#include "actor/actor_api.h"
#include "common/pkg_def.h"

using namespace actor;

#pragma pack(push, 1)
struct pkg_test
{
    DEF_FUNC_CODE(99999)
    DEF_GET_SIZE(pkg_test)
    char msg[128];
    uint64_t count_{ 0 };
};
#pragma pack(pop)

class test_class : public actor_framework
{
    public:
        virtual int32_t pre_action() override;
        virtual int32_t post_action() override;
};
