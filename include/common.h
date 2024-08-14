#ifndef COMMON_H
#define COMMON_H
class paramClass {
	public:
	int val;
	int del;
	int min;
	int max;
	bool flag_v;

	paramClass(int _min, int _max, int _inc_v, int _val, bool _flag_v) {
		min = _min;
		max = _max;
		del = _inc_v;
		val = _val;
		flag_v = _flag_v;
	}
	paramClass(int _min, int _max, int _inc_v, int _val) {
		min = _min;
		max = _max;
		del = _inc_v;
		val = _val;
	}
	paramClass(int _min, int _max, int _inc_v) {
		min = _min;
		max = _max;
		del = _inc_v;
		val = min + (max - min) / 2;
	}
	paramClass(int _min, int _max) {
		min = _min;
		max = _max;
		del = 1;
		val = min + (max - min) / 2;
	}

	void	set(int _val)	{if (_val >= min && _val <= max) val = _val;}
	int		get()			{return val;}
	int		inc()			{set(val + del); return val;}
	int		dec()			{set(val - del); return val;}
	void	flag(bool _val)	{flag_v = _val;}
	bool	flag()			{return flag_v;}
};
#endif //COMMON_H