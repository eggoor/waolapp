#ifndef MOCK_E_IFACE_PROV_H
#define MOCK_E_IFACE_PROV_H

#include <gmock/gmock.h>

extern "C" {
#include "e_iface_prov.h"
}

class MockEIfaceProv {
public:
    MOCK_METHOD(e_net_iface_t* const, get_first, (), ());
};

#endif // MOCK_E_IFACE_PROV_H
