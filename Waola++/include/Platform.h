#ifndef PLATFORM_H
#define PLATFORM_H

#if defined(__linux__) || defined(__APPLE__)
	#include "DepsX.h"
	#if defined(__linux__)
		#include "DepsLin.h"
	#elif defined(__APPLE__)
		#include "DepsMac.h"
	#endif

	#if __cplusplus < 201402L
	namespace std {
		template <class _Tp, class... _Args>
		inline unique_ptr<_Tp> make_unique(_Args&&... __args) {
			return unique_ptr<_Tp>(new _Tp(forward<_Args>(__args)...));
		}

		// Specialization for array types
		template <class _Tp>
		inline unique_ptr<_Tp> make_unique(size_t __n) {
			return unique_ptr<_Tp>(new typename std::remove_extent<_Tp>::type[__n]());
		}
	}
	#endif

#elif defined(_WIN32)
	#include "DepsWin.h"
#endif

namespace Wpp {
#ifndef arrsiz
	#define arrsiz(a) (sizeof(a) / sizeof(*a))
#endif

#if __cplusplus >= 201703L
	using byte = std::byte;
#else
	using byte = unsigned char;
#endif
} // namespace Wpp

#endif // PLATFORM_H
