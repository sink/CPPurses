#ifndef EVENT_HPP
#define EVENT_HPP

#include "object.hpp"

namespace twf {

class Event {
   public:
    enum Type {
        None,
        Timer,
        MouseButtonPress,
        MouseButtonRelease,
        MouseButtonDblClick,
        MouseButtonTplClick,
        MouseMove,
        KeyPress,
        KeyRelease,
        FocusIn,
        FocusOut,
        Enter,
        Leave,
        Paint,
        Move,
        Resize,
        Create,
        Destroy,
        Show,
        Hide,
        Close,
        Quit,
        ParentChange,
        ParentAboutToChange,
        ThreadChange,
        WindowActive,
        WindowDeactive,
        ShowToParent,
        HideToParent,
        Wheel,
        WindowTitleChange,
        WindowIconChange,
        ApplicationWindowIconChange,
        ApplicationFontChange,
        ApplicationLayoutDirectionChange,
        ApplicationPaletteChange,
        PaletteChange,
        ClipBoard,
        Speech,
        SockAct,
        WinEventAct,
        DeferredDelete,
        DragEnter,
        DragMove,
        DragLeave,
        Drop,
        DragResponse,
        ChildAdded,
        ChildPolished,
        ChildRemoved,
        ShowWindowRequest,
        PolishRequest,
        Polish,
        LayoutRequest,
        UpdateRequest,
        UpdateLater,

        ContextMenu,
        InputMethod,
        AccessibilityPrepare,
        LocaleChange,
        LanguageChange,
        LauoutDirectionChange,
        Style,
        OkRequest,
        HelpRequest,

        IconDrag,

        FontChange,
        EnabledChange,
        ActivationChange,
        StyleChange,
        IconTextChange,
        ModifiedChange,
        MouseTrackingChange,
        ToolTip,
        WhatsThis,
        StatusTip,

        ActionChanges,
        ActionAdded,
        ActionRemoved,

        FileOpen,

        Shortcut,
        ShortcutOverride,

        WhatsThisClicked,

        ToolBarChange,

        ApplicationActivate,
        ApplicationDeactivate,

        QueryWhatsThis,
        EnterWhatsThisMode,
        LeaveWhatsThisMode,

        ZOrderChange,

        HoverEnter,
        HoverLeave,
        HoverMove,

        AccessibilityHelp,
        AccessibilityDescription
    };

    explicit Event(Type type) : type_{type} {}

    virtual ~Event() = default;

    Type type() const { return type_; }

   protected:
    Type type_;
};

inline bool operator==(const Event& x, const Event& y) {
    return x.type() == y.type();
}

}  // namespace twf
#endif  // EVENT_HPP