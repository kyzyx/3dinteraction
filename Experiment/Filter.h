#pragma once
#include <queue>

template <class T>
class Filter
{
public:
	Filter(void) : len(1), next(nullptr) {;}
	Filter(Filter* f) : len(1), next(f) {;}
	Filter(int w, Filter* f=nullptr) : len(w), next(f) {;}
	~Filter(void) { if (next) delete next; }

	void update(T v) {
		if (next) {
			next->update(v);
			doUpdate(next->get());
		} else {
			doUpdate(v);
		}
	}
	void reset() {
		q.clear();
		if (next) next->reset();
		doReset();
	}

	virtual T get() = 0;
	virtual T predict(int k) = 0;
protected:
	virtual void doUpdate(T v) = 0;
	virtual void doReset() = 0;
	int len;
	std::deque<T> q;
	Filter* next;
};

template <class T>
class ForecastFilter : public Filter<T> {
public:
	ForecastFilter(int n, Filter<T>* f) : Filter<T>(f), time(n) {;}
	~ForecastFilter() {;}
	virtual T get() {
		return next->predict(time);
	}
	virtual T predict(int k) {
		return next->predict(k + time);
	}
protected:
	virtual void doUpdate(T v) {;}
	virtual void doReset() {;}
	ForecastFilter(void);
	int time;
};

// ------------------------------------------
// Averaging Filters
// Type must support zero, addition/subtraction, and scalar multiplication
// ------------------------------------------
template<class T>
T primitive_zero() { return T(0); }

template <class T, T (*zero)() = primitive_zero<T> >
class AvgFilter : public Filter<T> {
public:
	AvgFilter(void) : Filter() {cursum = zero();}
	AvgFilter(Filter* f) : Filter(f) {cursum = zero();}
	AvgFilter(int w) : Filter(w) {cursum = zero();}
	AvgFilter(int w, Filter* f) : Filter(w, f) {cursum = zero();}
	~AvgFilter() {;}

	virtual T get() {
		if (q.size() == 0) return zero();
		return cursum * 1.0/q.size();
	}
	virtual T predict(int k) {
		return get();
	}
protected:
	virtual void doUpdate(T v) {
		if (q.size() == len) {
			cursum -= q.back();
			q.pop_back();
		}
		cursum += v;
		q.push_front(v);
	}
	virtual void doReset() {
		cursum = zero();
	}
	T cursum;
};

template <class T, T (*zero)() = primitive_zero<T>>
class DoubleAvgFilter : public AvgFilter<T, zero> {
public:
	DoubleAvgFilter(void) : AvgFilter() {curmasum = zero();}
	DoubleAvgFilter(Filter* f) : AvgFilter(f) {curmasum = zero();}
	DoubleAvgFilter(int w) : AvgFilter(w) {curmasum = zero();}
	DoubleAvgFilter(int w, Filter* f) : AvgFilter(w, f) {curmasum = zero();}

	~DoubleAvgFilter() {;}

	virtual T get() {
		if (q.size() == 0) return zero();
		return (2*cursum - curmasum) * (1.0/q.size());
	}
	virtual T predict(int k) {
		if (q.size() < 3) return get();
		return get() + 2*k*(cursum - curmasum) * (1.0/(q.size() - 2));
	}
	
protected:
	virtual void doUpdate(T v) {
		if (q.size() == len) {
			cursum -= q.back();
			curmasum -= qma.back();
			q.pop_back();
			qma.pop_back();
		}
		cursum += v;
		q.push_front(v);
		curmasum += cursum/q.size();
		qma.push_front(cursum/q.size());
	}
	virtual void doReset() {
		AvgFilter::doReset();
		curmasum = zero();
		qma.clear();
	}
	T curmasum;
	std::deque<T> qma;
};

template <class T, T (*zero)() = primitive_zero<T>>
class ExpFilter : public Filter<T> {
public:
	ExpFilter(void) : Filter(), a(0.8) {cursum = zero();}
	ExpFilter(double alpha) : Filter(), a(alpha) {cursum = zero();}
	ExpFilter(Filter* f) : Filter(f), a(0.8) {cursum = zero();}
	ExpFilter(double alpha, Filter* f) : Filter(f), a(alpha) {cursum = zero();}
	~ExpFilter() {;}

	virtual T get() {
		return cursum;
	}
	virtual T predict(int k) {
		return get();
	}
protected:
	virtual void doUpdate(T v) {
		cursum = a*v + (1-a)*cursum;
	}
	virtual void doReset() {
		cursum = zero();
	}
	double a;
	double cursum;
};

template <class T, T (*zero)() = primitive_zero<T>>
class DoubleExpFilter : public ExpFilter<T, zero> {
public:
	DoubleExpFilter(void) : ExpFilter(), y(0.7), trend(zero()) {;}
	DoubleExpFilter(double alpha, double gamma) : ExpFilter(alpha), y(gamma), trend(zero()) {;}
	DoubleExpFilter(Filter* f) : ExpFilter(f), y(0.7), trend(zero()) {;}
	DoubleExpFilter(double alpha, double gamma, Filter* f) : ExpFilter(alpha, f), y(0.7), trend(zero()) {;}
	~DoubleExpFilter() {;}

	virtual T get() {
		return trend;
	}
	virtual T predict(int k) {
		return get() + k*trend;
	}
protected:
	virtual void doUpdate(T v) {
		T oldtrend = trend;
		trend = y*(v - cursum) + (1-y)*trend;
		cursum = a*v + (1-a)*(cursum + oldtrend);
	}
	virtual void doReset() {
		cursum = zero();
		trend = zero();
	}
	double y;
	T trend;
};