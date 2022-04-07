#pragma once

#include <QLabel>
#include <QLatin1String>
#include <QStringView>
#include <ranges>
#include <type_traits>

namespace Gui::Utils
{
template<class T>
concept BasicStringViewLike = std::ranges::contiguous_range<T> && std::ranges::sized_range<T>;

template<class T>
concept StringViewLike = BasicStringViewLike<T> && std::is_same<typename T::value_type, char>::value;

template<class T>
concept WStringViewLike = BasicStringViewLike<T> && std::is_same<typename T::value_type, wchar_t>::value;

template<class T>
concept QStringFormatArgument = requires(T t)
{
	QString().arg(t);
};

auto formatLabel(QLabel& label, StringViewLike auto arg)
{
	const auto formatted = label.text().arg(
	    QLatin1String { std::data(arg), static_cast<qsizetype>(std::size(arg)) });
	label.setText(formatted);
}

auto formatLabel(QLabel& label, WStringViewLike auto arg)
{
	const auto formatted = label.text().arg(
	    QStringView { std::data(arg), static_cast<qsizetype>(std::size(arg)) });
	label.setText(formatted);
}

auto formatLabel(QLabel& label, QStringFormatArgument auto arg)
{
	const auto formatted = label.text().arg(arg);
	label.setText(formatted);
}
}
