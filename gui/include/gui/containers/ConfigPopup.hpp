#ifndef CONFIGPOPUP_HPP
#define CONFIGPOPUP_HPP

#include <gui_generated/containers/ConfigPopupBase.hpp>

class ConfigPopup : public ConfigPopupBase
{
public:
    ConfigPopup();
    virtual ~ConfigPopup() {}

    virtual void initialize();
protected:
};

#endif // CONFIGPOPUP_HPP
