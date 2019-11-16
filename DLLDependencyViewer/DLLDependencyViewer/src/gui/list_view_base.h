#pragma once


class list_view_base
{
public:
	list_view_base();
	list_view_base(list_view_base const&) = delete;
	list_view_base(list_view_base&&) noexcept = delete;
	list_view_base& operator=(list_view_base const&) = delete;
	list_view_base& operator=(list_view_base&&) noexcept = delete;
	~list_view_base();
};