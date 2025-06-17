#ifndef MOCK_IPERSISTOR_H
#define MOCK_IPERSISTOR_H

#include <IPersistor.h>

class MockIPersistor : public IPersistor {
public:
    MOCK_METHOD(void, persist, (const std::vector<Wpp::HostPtr>& hosts), (override));
    MOCK_METHOD(std::vector<Wpp::HostPtr>, restore, (), (override));
};

#endif // MOCK_IPERSISTOR_H
