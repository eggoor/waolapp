#ifndef HOST_FIELD_H
#define HOST_FIELD_H

// #include <functional>

namespace Wpp {

typedef std::function<void(size_t)> FHostDetectedCb;

enum class HostField {
    None = -1
    , DisplayName = 0
    , Hostname = 1
    , IpAddr = 2
    , MacAddr = 3
    , LastSeenOnline = 4
    , OpResult = 5
};

typedef std::function<void(size_t, HostField)> FHostChangedCb;

} // namespace Wpp

#endif // HOST_FIELD_H
