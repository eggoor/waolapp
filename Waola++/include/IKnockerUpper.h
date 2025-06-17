#ifndef IKNOCKERUPPER_H
#define IKNOCKERUPPER_H

#include "IScanner.h"

namespace Wpp {
	using KnockerUpperPtr = std::unique_ptr<class IKnockerUpper> ;
	using macs_t = std::vector<MacAddress>;

	class IKnockerUpper {
	public:
		static KnockerUpperPtr make();
		virtual ~IKnockerUpper() = default;

		virtual void wakeUp(const class Subnet& subNet, const hosts_t& hosts) = 0;
		virtual bool wakeUp(subnets_t&& subNets, const macs_t& macs) = 0;
	};
} // Wpp

#endif //IKNOCKERUPPER_H
