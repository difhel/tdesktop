/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "statistics/chart_horizontal_lines_data.h"

#include "lang/lang_tag.h"

namespace Statistic {
namespace {

constexpr auto kMinLines = int(2);
constexpr auto kMaxLines = int(6);
constexpr auto kStep = 5.;

[[nodiscard]] int Round(int maxValue) {
	const auto k = int(maxValue / kStep);
	return (k % 10 == 0) ? maxValue : ((maxValue / 10 + 1) * 10);
}

} // namespace

ChartHorizontalLinesData::ChartHorizontalLinesData(
		int newMaxHeight,
		int newMinHeight,
		bool useMinHeight) {
	if (!useMinHeight) {
		const auto v = (newMaxHeight > 100)
			? Round(newMaxHeight)
			: newMaxHeight;

		const auto step = std::max(1, int(std::ceil(v / kStep)));

		auto n = kMaxLines;
		if (v < kMaxLines) {
			n = std::max(2, v + 1);
		} else if (v / 2 < kMaxLines) {
			n = v / 2 + 1;
			if (v % 2 != 0) {
				n++;
			}
		}

		lines.resize(n);

		for (auto i = 1; i < n; i++) {
			auto &line = lines[i];
			line.absoluteValue = i * step;
			line.caption = Lang::FormatCountToShort(
				line.absoluteValue).string;
		}
	} else {
		auto n = int(0);
		const auto diff = newMaxHeight - newMinHeight;
		auto step = 0.;
		if (diff == 0) {
			newMinHeight--;
			n = kMaxLines / 2;
			step = 1.;
		} else if (diff < kMaxLines) {
			n = std::max(kMinLines, diff + 1);
			step = 1.;
		} else if (diff / 2 < kMaxLines) {
			n = diff / 2 + diff % 2 + 1;
			step = 2.;
		} else {
			step = (newMaxHeight - newMinHeight) / kStep;
			if (step <= 0) {
				step = 1;
				n = std::max(kMinLines, newMaxHeight - newMinHeight + 1);
			} else {
				n = 6;
			}
		}

		lines.resize(n);
		const auto diffAbsoluteValue = int((n - 1) * step);
		for (auto i = 0; i < n; i++) {
			auto &line = lines[i];
			const auto value = int(i * step);
			line.absoluteValue = newMinHeight + value;
			line.relativeValue = value / float64(diffAbsoluteValue);
			line.caption = (line.absoluteValue >= 10'000)
				? Lang::FormatCountToShort(line.absoluteValue).string
				: QString("%L1").arg(line.absoluteValue);
		}
	}
}

int ChartHorizontalLinesData::LookupHeight(int maxValue) {
	const auto v = (maxValue > 100) ? Round(maxValue) : maxValue;

	const auto step = int(std::ceil(v / kStep));
	return step * kStep;
}

} // namespace Statistic
