#include "populated_glyph_stack.hpp"

#include <cstddef>
#include <string>
#include <vector>

#include <cppurses/painter/color.hpp>
#include <cppurses/painter/glyph.hpp>
#include <cppurses/painter/glyph_string.hpp>

#include "glyph_select.hpp"

namespace {
using namespace cppurses;

auto const box_drawing = Glyph_string{
    L"─ ━ │ ┃ ┄ ┅ ┆ ┇ ┈ ┉ ┊ ┋ ┌ ┍ ┎ ┏ ┐ ┑ ┒ ┓ └ ┕ ┖ ┗ ┘ ┙ ┚ ┛ ├ ┝ ┞ ┟ ┠ ┡ ┢ ┣ "
    L"┤ ┥ ┦ ┧ ┨ ┩ ┪ ┫ ┬ ┭ ┮ ┯ ┰ ┱ ┲ ┳ ┴ ┵ ┶ ┷ ┸ ┹ ┺ ┻ ┼ ┽ ┾ ┿ ╀ ╁ ╂ ╃ ╄ ╅ ╆ ╇ "
    L"╈ ╉ ╊ ╋ ╌ ╍ ╎ ╏ ═ ║ ╒ ╓ ╔ ╕ ╖ ╗ ╘ ╙ ╚ ╛ ╜ ╝ ╞ ╟ ╠ ╡ ╢ ╣ ╤ ╥ ╦ ╧ ╨ ╩ ╪ ╫ "
    L"╬ ╭ ╮ ╯ ╰ ╱ ╲ ╳ ╴ ╵ ╶ ╷ ╸ ╹ ╺ ╻ ╼ ╽ ╾ ╿ "};

auto const blocks = Glyph_string{
    L"▀ ▁ ▂ ▃ ▄ ▅ ▆ ▇ █ ▉ ▊ ▋ ▌ ▍ ▎ ▏ ▐ ░ ▒ ▓ ▔ ▕ ▖ ▗ ▘ ▙ ▚ ▛ ▜ ▝ ▞ ▟ "};

auto const tai_xuan_jing = Glyph_string{
    L"𝌀 𝌁 𝌂 𝌃 𝌄 𝌅 𝌆 𝌇 𝌈 𝌉 𝌊 𝌋 𝌌 𝌍 𝌎 𝌏 𝌐 𝌑 𝌒 𝌓 𝌔 𝌕 𝌖 𝌗 𝌘 𝌙 𝌚 𝌛 𝌜 𝌝 𝌞 𝌟 𝌠 𝌡 𝌢 𝌣 "
    L"𝌤 𝌥 𝌦 𝌧 𝌨 𝌩 𝌪 𝌫 𝌬 𝌭 𝌮 𝌯 𝌰 𝌱 𝌲 𝌳 𝌴 𝌵 𝌶 𝌷 𝌸 𝌹 𝌺 𝌻 𝌼 𝌽 𝌾 𝌿 𝍀 𝍁 𝍂 𝍃 𝍄 𝍅 𝍆 𝍇 "
    L"𝍈 𝍉 𝍊 𝍋 𝍌 𝍍 𝍎 𝍏 𝍐 𝍑 𝍒 𝍓 𝍔 𝍕 𝍖 "};

auto const dwarf_fortress = Glyph_string{
    L"☺ ☻ ♥ ♦ ♣ ♠ • ◘ ○ ◙ ♂ ♀ ♪ ♫ ☼ ► ◄ ↕ ‼ ¶ § ▬ ↨ ↑ ↓ → ← ∟ ↔ ▲ ▼ ! \" # $ % "
    L"& ' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F G H I "
    L"J K L M N O P Q R S T U V W X Y Z [ ] ^ _ ` a b c d e f g h i j k l m n "
    L"o p q r s t u v w x y z { | } ~ ⌂ Ç ü é â ä à å ç ê ë è ï î ì Ä Å É æ Æ "
    L"ô ö ò û ù ÿ Ö Ü ¢ £ ¥ ₧ ƒ á í ó ú ñ Ñ ª º ¿ ⌐ ¬ ½ ¼ ¡ « » ░ ▒ ▓ │ ┤ ╡ ╢ "
    L"╖ ╕ ╣ ║ ╗ ╝ ╜ ╛ ┐ └ ┴ ┬ ├ ─ ┼ ╞ ╟ ╚ ╔ ╩ ╦ ╠ ═ ╬ ╧ ╨ ╤ ╥ ╙ ╘ ╒ ╓ ╫ ╪ ┘ ┌ "
    L"█ ▄ ▌ ▐ ▀ α ß Γ π Σ σ µ τ Φ Θ Ω δ ∞ φ ε ∩ ≡ ± ≥ ≤ ⌠ ⌡ ÷ ≈ ° ∙ · √ ⁿ ² ■"};

auto const latin_1 = Glyph_string{
    L"¡ ¢ £ ¤ ¥ ¦ § ¨ © ª « ¬ ­ ® ¯ ° ± ² ³ ´ µ ¶ · ¸ ¹ º » ¼ ½ ¾ ¿ À Á Â Ã Ä "
    L"Å Æ Ç È É Ê Ë Ì Í Î Ï Ð Ñ Ò Ó Ô Õ Ö × Ø Ù Ú Û Ü Ý Þ ß à á â ã ä å æ ç è "
    L"é ê ë ì í î ï ð ñ ò ó ô õ ö ÷ ø ù ú û ü ý þ ÿ"};

auto const unicode_symbols = Glyph_string{
    L"‐ ‑ ‒ – — ― ‖ ‗ ‘ ’ ‚ ‛ “ ” „ ‟ † ‡ • ‣ ․ ‥ … ‧ ‰ ‱ ′ ″ ‴ ‵ ‶ ‷ ‸ ‹ › ※ "
    L"‼ ‽ ‾ ‿ ⁀ ⁁ ⁂ ⁃ ⁄ ⁅ ⁆ ⁇ ⁈ ⁉ ⁊ ⁋ ⁌ ⁍ ⁎ ⁏ ⁐ ⁑ ⁒ ⁓ ⁔ ⁕ ⁖ ⁗ ⁘ ⁙ ⁚ ⁛ ⁜ ⁝ ⁞ "};

auto const currency = Glyph_string{
    L"₠ ₡ ₢ ₣ ₤ ₥ ₦ ₧ ₨ ₩ ₪ ₫ € ₭ ₮ ₯ ₰ ₱ ₲ ₳ ₴ ₵ ₶ ₷ ₸ ₹ ₺ "
    L"₻ "
    L"₼ "
    L"₽ "
    L"₾ "
    L"₿"};

auto const arrows = Glyph_string{
    L"← ↑ → ↓ ↔ ↕ ↖ ↗ ↘ ↙ ↚ ↛ ↜ ↝ ↞ ↟ ↠ ↡ ↢ ↣ ↤ ↥ ↦ ↧ ↨ ↩ ↪ ↫ ↬ ↭ ↮ ↯ ↰ ↱ ↲ ↳ "
    L"↴ ↵ ↶ ↷ ↸ ↹ ↺ ↻ ↼ ↽ ↾ ↿ ⇀ ⇁ ⇂ ⇃ ⇄ ⇅ ⇆ ⇇ ⇈ ⇉ ⇊ ⇋ ⇌ ⇍ ⇎ ⇏ ⇐ ⇑ ⇒ ⇓ ⇔ ⇕ ⇖ ⇗ "
    L"⇘ ⇙ ⇚ ⇛ ⇜ ⇝ ⇞ ⇟ ⇠ ⇡ ⇢ ⇣ ⇤ ⇥ ⇦ ⇧ ⇨ ⇩ ⇪ ⇫ ⇬ ⇭ ⇮ ⇯ ⇰ ⇱ ⇲ ⇳ ⇴ ⇵ ⇶ ⇷ ⇸ ⇹ ⇺ ⇻ "
    L"⇼ ⇽ ⇾ ⇿"};

auto const math_operators = Glyph_string{
    L"∀ ∁ ∂ ∃ ∄ ∅ ∆ ∇ ∈ ∉ ∊ ∋ ∌ ∍ ∎ ∏ ∐ ∑ − ∓ ∔ ∕ ∖ ∗ ∘ ∙ √ ∛ ∜ ∝ ∞ ∟ ∠ ∡ "
    L"∢ ∣ ∤ ∥ ∦ ∧ ∨ ∩ ∪ ∫ ∮ ∱ ∲ ∳ ∴ ∵ ∶ ∷ ∸ ∹ ∺ ∻ ∼ ∽ ∾ ∿ ≀ ≁ ≂ ≃ ≄ ≅ ≆ ≇ ≈ ≉ "
    L"≊ ≋ ≌ ≍ ≎ ≏ ≐ ≑ ≒ ≓ ≔ ≕ ≖ ≗ ≘ ≙ ≚ ≛ ≜ ≝ ≞ ≟ ≠ ≡ ≢ ≣ ≤ ≥ ≦ ≧ ≨ ≩ ≪ ≫ "
    L"≬ ≭ ≮ ≯ ≰ ≱ ≲ ≳ ≴ ≵ ≶ ≷ ≸ ≹ ≺ ≻ ≼ ≽ ≾ ≿ ⊀ ⊁ ⊂ ⊃ ⊄ ⊅ ⊆ ⊇ ⊈ ⊉ ⊊ ⊋ ⊌ ⊍ ⊎ ⊏ "
    L"⊐ ⊑ ⊒ ⊓ ⊔ ⊕ ⊖ ⊗ ⊘ ⊙ ⊚ ⊛ ⊜ ⊝ ⊞ ⊟ ⊠ ⊡ ⊢ ⊣ ⊤ ⊥ ⊦ ⊧ ⊨ ⊩ ⊪ ⊫ ⊬ ⊭ ⊮ ⊯ ⊰ ⊱ ⊲ ⊳ "
    L"⊴ ⊵ ⊶ ⊷ ⊸ ⊹ ⊺ ⊻ ⊼ ⊽ ⊾ ⊿ ⋀ ⋁ ⋂ ⋃ ⋄ ⋅ ⋆ ⋇ ⋈ ⋉ ⋊ ⋋ ⋌ ⋍ ⋎ ⋏ ⋐ ⋑ ⋒ ⋓ ⋔ ⋕ ⋖ ⋗ "
    L"⋘ ⋙ ⋚ ⋛ ⋜ ⋝ ⋞ ⋟ ⋠ ⋡ ⋢ ⋣ ⋤ ⋥ ⋦ ⋧ ⋨ ⋩ ⋪ ⋫ ⋬ ⋭ ⋮ ⋯ ⋰ ⋱ ⋲ ⋳ ⋴ ⋵ ⋶ ⋷ ⋸ ⋹ ⋺ ⋻ "
    L"⋼ ⋽ ⋾ ⋿"};

auto const geometric = Glyph_string{
    L"■ □ ▢ ▣ ▤ ▥ ▦ ▧ ▨ ▩ ▪ ▫ ▬ ▭ ▮ ▯ ▰ ▱ ▲ △ ▴ ▵ ▶ ▷ ▸ ▹ ► ▻ ▼ ▽ ▾ ▿ ◀ ◁ ◂ ◃ "
    L"◄ ◅ ◆ ◇ ◈ ◉ ◊ ○ ◌ ◍ ◎ ● ◐ ◑ ◒ ◓ ◔ ◕ ◖◗ ◘ ◙ ◚ ◛ ◜ ◝ ◞ ◟ ◠ ◡ ◢ ◣ ◤ ◥ ◦ ◧ ◨ "
    L"◩ ◪ ◫ ◬ ◭ ◮ ◯ ◰ ◱ ◲ ◳ ◴ ◵ ◶ ◷ ◸ ◹ ◺ ◻ ◼ ◽ ◾ ◿"};

auto generate_color_blocks() -> Glyph_string
{
    auto const colors = std::vector<Color>{
        Color::Black,  Color::Dark_red,   Color::Dark_blue,  Color::Dark_gray,
        Color::Brown,  Color::Green,      Color::Red,        Color::Gray,
        Color::Blue,   Color::Orange,     Color::Light_gray, Color::Light_green,
        Color::Violet, Color::Light_blue, Color::Yellow,     Color::White};
    auto const rectangles = std::wstring{L"░▒▓"};

    auto str = Glyph_string{};
    for (wchar_t rect : rectangles) {
        for (auto i = std::size_t{0}; i < colors.size(); ++i) {
            for (auto j = std::size_t{i + 1}; j < colors.size(); ++j) {
                str.append(
                    Glyph{rect, background(colors[i]), foreground(colors[j])});
            }
        }
    }
    for (Color c : colors) {
        str.append(Glyph{L'█', foreground(c)});
    }
    return str;
}

auto const color_blocks = generate_color_blocks();

}  // namespace

namespace demos {
namespace glyph_paint {

Populated_glyph_stack::Populated_glyph_stack()
{
    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Latin-1", latin_1).glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Blocks", blocks).glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Color Blocks", color_blocks)
            .glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Box Drawing", box_drawing)
            .glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Tai Xuan Jing", tai_xuan_jing)
            .glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Unicode Sym.", unicode_symbols)
            .glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Currency", currency).glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Dwarf Fortress", dwarf_fortress)
            .glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Arrows", arrows).glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Math Operators", math_operators)
            .glyph_selected);

    signal_refs_.emplace_back(
        this->make_page<Glyph_select>("Geometric", geometric).glyph_selected);
}

void Populated_glyph_stack::make_connections(sig::Slot<void(Glyph)> slot)
{
    for (auto& signal_ref : signal_refs_) {
        signal_ref.get().connect(slot);
    }
}

}  // namespace glyph_paint
}  // namespace demos
