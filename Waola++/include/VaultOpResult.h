#ifndef VAULTOPRESULT_H
#define VAULTOPRESULT_H

#include "HostField.h"
#include "Host.h"

namespace Wpp {

struct VaultOpResult {
    int xHost = -1;
    HostField changedFiled = HostField::None;
	bool isAdded = false;
};

} // namespace Wpp

#endif // VAULTOPRESULT_H
