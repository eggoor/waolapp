#ifndef NETWORKPARAMS_H
#define NETWORKPARAMS_H

#include "HostField.h"
#include "IInterfaceProvider.h"
#include "IKnockerUpper.h"
#include "IScanner.h"
#include "IVault.h"

namespace Wpp {

struct NetworkParams {
    InterfaceProviderPtr interfaceProvider;
    FScannerFactory scannerFactory;
    FVaultFactory vaultFactory;
    KnockerUpperPtr knockerUpper;
    FHostDetectedCb cbHostDetected;
    FHostChangedCb cbHostChanged;
    FScanStateChangedCb cbScanFinished;
};

} // namespace Wpp

#endif // NETWORKPARAMS_H
