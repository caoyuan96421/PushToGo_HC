#include <gui/containers/MessagePopup.hpp>

MessagePopup::MessagePopup() : callbackOKClicked(this, &MessagePopup::okClicked)
{
	setVisible(false);
	messageArea.setWideTextAction(WIDE_TEXT_WORDWRAP_ELLIPSIS_AFTER_SPACE);

	okButton.setAction(callbackOKClicked);
}

void MessagePopup::initialize()
{
    MessagePopupBase::initialize();
}

void MessagePopup::showMessage(const char *msg){
	Unicode::strncpy(messageAreaBuffer, msg, MESSAGEAREA_SIZE);
	setVisible(true);
	invalidate();
}

void MessagePopup::okClicked(const AbstractButton &src) {
	setVisible(false);
	invalidate();
}
