#include "animated_widget.hpp"

#include <cppurses/painter/painter.hpp>
#include <cppurses/system/detail/fps_to_period.hpp>
#include <cppurses/system/events/mouse.hpp>
#include <cppurses/system/events/paint_event.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/focus_policy.hpp>

namespace demos {
namespace animation {

Animated_bit::Animated_bit(int rate, bool ani) {
    this->focus_policy = cppurses::Focus_policy::Strong;
    if (ani) {
        this->enable_animation(cppurses::detail::fps_to_period(rate));
    }
}

bool Animated_bit::timer_event() {
    int next_x = coords_.x + (1 * x_direction);
    const int width = static_cast<int>(this->width());
    if (next_x >= width || next_x < 0) {
        x_direction *= -1;
        next_x = coords_.x + (1 * x_direction);
        if (next_x >= width) {
            next_x = width != 0 ? width - 1 : 0;
        }
    }
    int next_y = coords_.y + (1 * y_direction);
    const int height = static_cast<int>(this->height());
    if (next_y >= height || next_y < 0) {
        y_direction *= -1;
        next_y = coords_.y + (1 * y_direction);
        if (next_y >= height) {
            next_y = height != 0 ? height - 1 : 0;
        }
    }
    coords_.x = next_x;
    coords_.y = next_y;
    return Widget::timer_event();
}

bool Animated_bit::paint_event() {
    cppurses::Painter p{*this};
    p.put(glyph_, coords_);
    return Widget::paint_event();
}

bool Animated_bit::mouse_press_event(const cppurses::Mouse::State& mouse) {
    coords_ = mouse.local;
    this->update();
    return Widget::mouse_press_event(mouse);
}

}  // namespace animation
}  // namespace demos
