#pragma once

#include "ofxiOSExtras.h"
#include "ofxImGui.h"

class InputTextButton;

@interface TextField : UITextField<UIKeyInput>
{
@public
    InputTextButton* inputTextButton;
}
- (void)textFieldEditingChanged:(id)sender;

@end

class InputTextButton
{
    
public:
    InputTextButton();
    void setup(string initialString);
    void draw();
    void onTextEdit(string);
    TextField* textField;
    string text;
};
