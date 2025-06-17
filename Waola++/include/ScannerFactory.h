#ifndef SCANNER_FACTORY_H
#define SCANNER_FACTORY_H

#include "ScannerParams.h"

namespace Wpp {

typedef std::unique_ptr<class IScanner> ScannerPtr;
typedef std::function<ScannerPtr(const ScannerParams&)> FScannerFactory;

class ScannerFactory {
public:
    static ScannerPtr create(const ScannerParams& params);
};

} // namespace Wpp

#endif // SCANNER_FACTORY_H
