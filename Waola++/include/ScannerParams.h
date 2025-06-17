#ifndef SCANNERPARAMS_H
#define SCANNERPARAMS_H

namespace Wpp {

typedef std::function<void(const class ArpResult&)> FArpResultCb;
typedef std::function<void(const class L7Result&)> FL7ResultCb;
typedef std::function<void(bool)> FScanStateChangedCb;

struct ScannerParams {
    FArpResultCb cbHostDetected;
	FL7ResultCb cbL7Resolved;
	FScanStateChangedCb cbScanStateChanged;
};

} // namespace Wpp

#endif // SCANNERPARAMS_H
