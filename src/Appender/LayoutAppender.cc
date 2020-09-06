#include "Appender/LayoutAppender.h"
#include "Layout/BasicLayout.h"

namespace Logger {

LayoutAppender::LayoutAppender(const std::string & name, bool async_log) : AppenderSkeleton(name, async_log), layout_(new BasicLayout()) {
}

LayoutAppender::~LayoutAppender() {
	delete layout_;
}

bool LayoutAppender::RequiresLayout() const {
	return true;
}

void LayoutAppender::SetLayout(Layout * layout) {
	if (layout_ != layout) {
		Layout * old_layout = layout_;
		layout_ = layout ? layout : new BasicLayout();
		delete old_layout;
	}
}

}