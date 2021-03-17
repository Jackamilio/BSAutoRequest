#include "subclass.h"

#ifdef NANA_WINDOWS

Subclass::Subclass(nana::window wd)
	: native_(reinterpret_cast<HWND>(nana::API::root(wd))),
	old_proc_(nullptr)
{
}

Subclass::~Subclass()
{
	clear();
}

void Subclass::make_before(UINT msg, std::function<bool(UINT, WPARAM, LPARAM, LRESULT*)> fn)
{
	lock_guard lock(mutex_);
	msg_table_[msg].before = std::move(fn);
	_m_subclass(true);
}

void Subclass::make_after(UINT msg, std::function<bool(UINT, WPARAM, LPARAM, LRESULT*)> fn)
{
	lock_guard lock(mutex_);
	msg_table_[msg].after = std::move(fn);
	_m_subclass(true);
}

void Subclass::umake_before(UINT msg)
{
	lock_guard lock(mutex_);
	auto i = msg_table_.find(msg);
	if (msg_table_.end() != i)
	{
		i->second.before = nullptr;
		if (nullptr == i->second.after)
		{
			msg_table_.erase(msg);
			if (msg_table_.empty())
				_m_subclass(false);
		}
	}
}

void Subclass::umake_after(UINT msg)
{
	lock_guard lock(mutex_);
	auto i = msg_table_.find(msg);
	if (msg_table_.end() != i)
	{
		i->second.after = nullptr;
		if (nullptr == i->second.before)
		{
			msg_table_.erase(msg);
			if (msg_table_.empty())
				_m_subclass(false);
		}
	}
}

void Subclass::umake(UINT msg)
{
	lock_guard lock(mutex_);
	msg_table_.erase(msg);

	if (msg_table_.empty())
		_m_subclass(false);
}

void Subclass::clear()
{
	lock_guard lock(mutex_);
	msg_table_.clear();
	_m_subclass(false);
}

void Subclass::_m_subclass(bool enable)
{
	lock_guard lock(mutex_);

	if (enable)
	{
		if (native_ && (nullptr == old_proc_))
		{
			old_proc_ = (WNDPROC)::SetWindowLongPtr(native_, -4 /* GWL_WNDPROC*/, (LONG_PTR)_m_subclass_proc);
			if (old_proc_)
				table_[native_] = this;
		}
	}
	else
	{
		if (old_proc_)
		{
			table_.erase(native_);
			::SetWindowLongPtr(native_, -4 /* GWL_WNDPROC*/, (LONG_PTR)old_proc_);
			old_proc_ = nullptr;

		}
	}
}

bool Subclass::_m_call_before(msg_pro& pro, UINT msg, WPARAM wp, LPARAM lp, LRESULT* res)
{
	return (pro.before ? pro.before(msg, wp, lp, res) : true);
}

bool Subclass::_m_call_after(msg_pro& pro, UINT msg, WPARAM wp, LPARAM lp, LRESULT* res)
{
	return (pro.after ? pro.after(msg, wp, lp, res) : true);
}

LRESULT CALLBACK Subclass::_m_subclass_proc(HWND wd, UINT msg, WPARAM wp, LPARAM lp)
{
	lock_guard lock(mutex_);

	Subclass* self = _m_find(wd);
	if (nullptr == self || nullptr == self->old_proc_)
		return 0;

	auto i = self->msg_table_.find(msg);
	if (self->msg_table_.end() == i)
		return ::CallWindowProc(self->old_proc_, wd, msg, wp, lp);

	LRESULT res = 0;
	if (self->_m_call_before(i->second, msg, wp, lp, &res))
	{
		res = ::CallWindowProc(self->old_proc_, wd, msg, wp, lp);
		self->_m_call_after(i->second, msg, wp, lp, &res);
	}

	if (WM_DESTROY == msg)
		self->clear();

	return res;
}

Subclass* Subclass::_m_find(HWND wd)
{
	lock_guard lock(mutex_);
	std::map<HWND, Subclass*>::iterator i = table_.find(wd);
	if (i != table_.end())
		return i->second;

	return 0;
}

std::recursive_mutex Subclass::mutex_;
std::map<HWND, Subclass*> Subclass::table_;

#endif
