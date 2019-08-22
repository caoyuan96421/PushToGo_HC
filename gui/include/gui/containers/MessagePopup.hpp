#ifndef MESSAGEPOPUP_HPP
#define MESSAGEPOPUP_HPP

#include <gui_generated/containers/MessagePopupBase.hpp>

class MessagePopup : public MessagePopupBase
{
public:
    MessagePopup();
    virtual ~MessagePopup() {}

    virtual void initialize();

    void showMessage(const char *msg);
protected:

	touchgfx::Callback<MessagePopup, const AbstractButton &> callbackOKClicked;
	void okClicked(const AbstractButton &src);
};

#endif // MESSAGEPOPUP_HPP
