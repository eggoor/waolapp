#include "KnockerUpper.h"

namespace Wpp {
	void KnockerUpper::platformStartUp()
	{}

	void KnockerUpper::platformShutDown()
	{}

	int KnockerUpper::getSocketError() {
		return errno;
	}
} // namespace Wpp
