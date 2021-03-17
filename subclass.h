#pragma once

#include <map>
#include <functional>
#include <mutex>

#include <nana/gui.hpp>

#ifdef NANA_WINDOWS
#include <Windows.h>
#include <Winuser.h>

class Subclass
{
	struct msg_pro
	{
		std::function<bool(UINT, WPARAM, LPARAM, LRESULT*)> before;
		std::function<bool(UINT, WPARAM, LPARAM, LRESULT*)> after;
	};

	typedef std::lock_guard<std::recursive_mutex> lock_guard;
public:
	Subclass(nana::window wd);
	~Subclass();

	void make_before(UINT msg, std::function<bool(UINT, WPARAM, LPARAM, LRESULT*)> fn);
	void make_after(UINT msg, std::function<bool(UINT, WPARAM, LPARAM, LRESULT*)> fn);

	void umake_before(UINT msg);
	void umake_after(UINT msg);

	void umake(UINT msg);

	void clear();
private:
	void _m_subclass(bool enable);

	static bool _m_call_before(msg_pro& pro, UINT msg, WPARAM wp, LPARAM lp, LRESULT* res);
	static bool _m_call_after(msg_pro& pro, UINT msg, WPARAM wp, LPARAM lp, LRESULT* res);
private:
	static LRESULT CALLBACK _m_subclass_proc(HWND wd, UINT msg, WPARAM wp, LPARAM lp);
	static Subclass* _m_find(HWND wd);

private:
	HWND native_;
	WNDPROC old_proc_;
	std::map<UINT, msg_pro> msg_table_;

	static std::recursive_mutex mutex_;
	static std::map<HWND, Subclass*> table_;
};

#endif
