#include "ChoosersPanel.hpp"

ChoosersPanel::ChoosersPanel() :
    Box(Gtk::Orientation::HORIZONTAL),
    idChooser("id"),
    idPacketChooser("id пакета"),
    idMeasChooser("id измерения"),
    numSubChooser("номер поднесущей")
{
    append(idChooser);
    append(idPacketChooser);
    append(idMeasChooser);
    append(numSubChooser);
}

std::pair<int64_t, int64_t> ChoosersPanel::getIdLimits() const {
    return idChooser.getLimits();
}

std::pair<int64_t, int64_t> ChoosersPanel::getidPacketLimits() const {
    return idPacketChooser.getLimits();
}

std::pair<int64_t, int64_t> ChoosersPanel::getIdMeasLimits() const {
    return idMeasChooser.getLimits();
}

std::pair<int64_t, int64_t> ChoosersPanel::getNumSubLimits() const {
    return numSubChooser.getLimits();
}

ChooserLimiter::ChooserLimiter(Glib::ustring fieldName) :
    bottomBoundAdj(Gtk::Adjustment::create(0, 0, std::numeric_limits<int64_t>::max(), 1, 1)),
    bottomBoundButton(bottomBoundAdj),
    bottomBoundFrame("Нижняя граница"),

    topBoundAdj(Gtk::Adjustment::create(0, 0, std::numeric_limits<int64_t>::max(), 1, 1)),
    topBoundButton(topBoundAdj),
    topBoundFrame("Верхняя граница"),

    shouldChooseButton(Glib::ustring("Ограничить ") + fieldName + "?")
{
    bottomBoundFrame.set_child(bottomBoundButton);
    attach(bottomBoundFrame, 0, 0);

    topBoundFrame.set_child(topBoundButton);
    attach(topBoundFrame, 1, 0);

    attach(shouldChooseButton, 0, 1, 2);
}

std::pair<int64_t, int64_t> ChooserLimiter::getLimits() const {
    if(!shouldChooseButton.get_active())
        return {0, std::numeric_limits<int64_t>::max()};
    return {bottomBoundButton.get_value(), topBoundButton.get_value()};
}
