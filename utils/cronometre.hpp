#ifndef utils_cronometre_hpp
#define utils_cronometre_hpp

#include <chrono>
#include <functional>
#include <string>

namespace utils {
	template <class R, class ... Args>
	class CronoBase {
	protected:
		std::function<R(Args ...)> f;
		std::chrono::high_resolution_clock::time_point ini, fi;
		
		CronoBase(std::function<R(Args ...)> func) { f = func; }
	public:
		template<class span>
		auto elapsed() const { return std::chrono::duration_cast<span>(fi - ini).count(); }
		operator string() const {
			if (elapsed<chrono::seconds>() > 100LL)
				return std::to_string(elapsed<chrono::seconds>()) + " s";
			if (elapsed<chrono::milliseconds>() > 100LL)
				return std::to_string(elapsed<chrono::milliseconds>()) + " ms";
			if (elapsed<chrono::microseconds>() > 100LL)
				return std::to_string(elapsed<chrono::microseconds>()) + " ys";
			else
				return std::to_string(elapsed<chrono::nanoseconds>()) + " ns";
		}
	};

	template <class R, class ... Args>
	class Cronometre : public CronoBase<R, Args ...> {
	public:
		Cronometre(std::function<R(Args ...)> func) : CronoBase<R, Args ...>(func) {}
		R operator()(Args ... args) {
			R result;
			
			this->ini = std::chrono::high_resolution_clock::now();
			result = this->f(args ...);
			this->fi  = std::chrono::high_resolution_clock::now();
			
			return result;
		}
	};

	template <class ... Args>
	class Cronometre<void, Args ...> : public CronoBase<void, Args ...> {
	public:
		Cronometre(std::function<void(Args ...)> func) : CronoBase<void, Args ...>(func) {}
		void operator()(Args ... args) {
			this->ini = std::chrono::high_resolution_clock::now();
			this->f(args ...);
			this->fi = std::chrono::high_resolution_clock::now();
		}
	};
	
	template <class R, class ... Args>
	Cronometre<R, Args ...> crea_Cronometre(std::function<R(Args ...)> func) { return Cronometre<R, Args ...>(func); }
	template <class R, class ... Args>
	Cronometre<R, Args ...> crea_Cronometre(R (*fptr)(Args ...)) { return Cronometre<R, Args ...>(std::function<R(Args ...)>(fptr)); }
}

#endif
