#ifndef HOST_H
#define HOST_H

#include "HostSaved.h"
#include "OpResult.h"

namespace Wpp {

typedef std::shared_ptr<class Host> HostPtr;
typedef std::vector<HostPtr> hosts_t;

class Host: public HostSaved
{
    using HostSaved::HostSaved;
public:
    Wpp::OpResult OpResult = orUndefined;
};

} // namespace Wpp

#endif // HOST_H
